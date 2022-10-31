/***************************************************************************
 *   Copyright (C) 2014-2022
 *   by Thomas Maier <balagi@justmail.de>
 *
 *   Copyright: See COPYING file that comes with this distribution         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 ***************************************************************************/

#include "common.h"

/*
 * - DCC Cutout is generated only at the end of the packet
 * - At the end of a cutout, shortcut detection may be triggered once or two times.
 * - Current/shortcut detection: MAX_SHORTCUT_CNT shortcuts must have occured in TIMER_SHORT_CUT time
 * - Shortcut detection is disabled in the TIMER_STARTUP time after DCC startup
 * - Shortcut detection is disabled in the first half of the first DCC bit after a cutout
 *
 * Ausgabe signals:
 * PC0  IN1 bridge a and b input1
 * PC1  IN2 bridge a and b input2
 * PC2  EN  Brücke ein
 *
 * PD0  Abschaltung (Kurzschluss) wenn auf Low LED an
 * PD1  Overflow (Strom zu hoch)  wenn auf Low LED an
 * PD2  NOTAUS  wenn auf Low LED an
 * PD3  ON      wenn auf Low LED an
 *
 * Input signals:
 * PA0  Strom-Sensor Brücke a
 * PA1  Spannungs-Sensor Versorungsspannung
 * PA2  Strom-Sensor Brücke b
 *
 * PC3  Strom zu hoch: L6206 OCD hat ausgelöst, ist auf 0
 * PC4  DCC Input Signal
 * PC5  Notaus Input Signal: wenn High --> Notaus
 */

// defines ...........................

#define bo_PRODUCT_ID   0x0300
#define bo_VENDOR_ID    0x1234
#define bo_FIRMWARE_VERSION 0x0100
#define bo_DEVICE_DESC  "Booster:1"

//APP_FIRMWARE_HEADER(bo_PRODUCT_ID, bo_VENDOR_ID, bo_FIRMWARE_VERSION)

#define bo_TIMER_PRESCALER   64
#define bo_TIMER_FREQ_HZ     100
#define bo_TIMER_PERIOD      (F_CPU / (bo_TIMER_PRESCALER * bo_TIMER_FREQ_HZ))  // 5000

#define bo_LED_PORT  PORTD
#define bo_LED_SHORTCUT_b    0
#define bo_LED_CUR_OV_b      1
#define bo_LED_NOTAUS_b      2
#define bo_LED_ON_b			 3

#define bo_DCCM_PORT  PORTC
// out
#define bo_DCCM_EN_b   2
#define bo_DCCM_IN1_b  0
#define bo_DCCM_IN2_b  1
#define bo_DCCM_CUTOUT_b 7
// in
#define bo_CUR_OV_b   3
#define bo_DCC_IN_b   4
#define bo_NOTAUS_b   5

// ADC: PA0,2: Strom PA1: Voltage
#define bo_MEASURE_PORT PORTA // PORT for ADC
#define bo_CUR_a	0 // current bridge 1
#define bo_CUR_b	2 // current bridge 2
#define bo_Vcc		1 // supply voltage for bridge

#define bo_TIMER_STARTUP  63 // bei 16ms Timer //100 // 1 s

#define bo_SWITCH_NOTAUS_b  0

#define bo_BOOSTER_FLG_ON_b      0
#define bo_BOOSTER_FLG_NOTAUS_b  1
#define bo_BOOSTER_FLG_CUR_OV_b  2

#define bo_DCC_IN_PORT bo_DCCM_PORT

#define bo_TIMER_SHORT_CUT 35000 // 70ms
#define bo_MAX_SHORTCUT_CNT 5000 // * ca.13us * 5100 = 70ms

#define bo_DCC_WATCHDOG_VAL 4 // 4*16ms = 64ms

#define bo_BOOSTER_DEFAULT_SHORTCUT_INTERVAL  35000 // 70ms
#define bo_BOOSTER_DEFAULT_SHORTCUT_LIMIT     4300

#define bo_DEC_STATE_OFF      0
#define bo_DEC_STATE_FIRST    1
#define bo_DEC_STATE_PREAMBLE 2
#define bo_DEC_STATE_STARTBIT 3
#define bo_DEC_STATE_BIT_H1   4
#define bo_DEC_STATE_BIT_H2   5
#define bo_DEC_STATE_STARTSTOPBIT 6

// declaration .........................
static void bo_do_dec_parse_packet(void);
void bo_dec_init(void);
void bo_dec_start(void);
void bo_dcc_sensors_shortcut_off(void);
void bo_dcc_sensors_shortcut_on(void);
void bo_booster_init(void);


// structs ...........................

struct bo_dccdec {
	uint8_t state;
	uint8_t preamble;
	uint8_t buf[10];
	uint8_t bufsize;
	uint8_t bits;
	uint8_t bitbuf;
	uint8_t lasthbit;
	uint8_t xor;
	uint8_t cutout;
};

struct bo_v_t {
	uint16_t g_dcc_shortcut_cnt;
	uint16_t g_shortcutnummax;
	uint16_t g_booster_flags;
	uint16_t g_timer_startup;
	uint8_t g_switches;
	uint8_t g_switches_t;
	uint8_t g_timer;
	uint16_t g_advals[4];
	struct timer   timer_startup;
	struct timer   timer_dcc_watchdog;
	uint16_t       shortcut_cnt;
	uint16_t       shortcut_limit;
	uint16_t       shortcut_interval;
	uint16_t       shortcut_nummax;
	struct {
		unsigned write_shortcut_limit:1;
		unsigned write_shortcut_interval:1;
	} eeprom_flags;
	struct bo_dccdec dccdec;
};

struct bo_v_t bo_v = { 0 };


// functions ..............................

static void bo_do_dec_parse_packet(void) {
	//timer_set(&g_booster.timer_dcc_watchdog, bo_DCC_WATCHDOG_VAL);
	
	if (bo_v.dccdec.buf[0] != 0 // broadcast or reset
			&& bo_v.dccdec.buf[0] != 0xff) { // idle
		// produce cutout with TCD0.CCC
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			TCD0.CCC = TCD0.CNT + (28/2);
		}
		TCD0.INTFLAGS = Bit(TC0_CCCIF_bp);
		TCD0.INTCTRLB = (TCD0.INTCTRLB & ~TC0_CCCINTLVL_gm)|TC_CCCINTLVL_LO_gc;
		bo_v.dccdec.cutout = 1;
		port_setbit(bo_DCCM_PORT, bo_DCCM_CUTOUT_b); // cutout test point
	}
}

// utils --------------------------------------------------------

static uint8_t bo_read_production_signature_row(uint8_t offset) {
	uint8_t value;
	NVM.CMD = NVM_CMD_READ_CALIB_ROW_gc;
	value = pgm_read_byte(offset);
	NVM.CMD = NVM_CMD_NO_OPERATION_gc;
	return value;
}
// init ---------------------------------------------------------

void bo_init_led_port(void){
	// LED port: active low
    port_out(bo_LED_PORT) = 0xff;	// all off
    uint8_t m = Bit(bo_LED_NOTAUS_b)|Bit(bo_LED_CUR_OV_b)|Bit(bo_LED_NOTAUS_b)|Bit(bo_LED_ON_b);  // led bits
    port_dirout(bo_LED_PORT, m);	// dir output
    PORTCFG_MPCMASK = m;			// PD3..0
    bo_LED_PORT.PIN0CTRL = PORT_OPC_TOTEM_gc; // output as totem pole
}
void bo_init_dccm_port(void) {
	// bo_DCCM_PORT (PORTC)
	// 
	// out: cutout marker, enable driver, driver IN1 and IN2
	port_out(bo_DCCM_PORT) = 0;		// all off (=0)
	uint8_t m = Bit(bo_DCCM_CUTOUT_b)|Bit(bo_DCCM_EN_b)|Bit(bo_DCCM_IN1_b)|Bit(bo_DCCM_IN2_b);
	port_dirout(bo_DCCM_PORT, m);	// as output
	PORTCFG_MPCMASK = m;
	PORTC.PIN0CTRL = PORT_OPC_TOTEM_gc;	// as totem pole
	
	// in: current overflow, dcc in, notaus
	m = Bit(bo_CUR_OV_b)|Bit(bo_DCC_IN_b)|Bit(bo_NOTAUS_b);
	port_dirin(bo_DCCM_PORT, m);	// as input
	PORTCFG_MPCMASK = m;
	PORTC.PIN3CTRL = PORT_OPC_PULLUP_gc; // as pull up
}
void bo_init_dcc_in(void) {
	// DCC input
	
	// PORTC Interrupt 1 PC4 on
    PORTC.INT1MASK = Bit(bo_DCC_IN_b);
	// on PC4 both edges
    PORTC.PIN4CTRL = PORT_ISC_BOTHEDGES_gc;
	// set on: interrupt 1 on PORTC
    PORTC.INTFLAGS = Bit(PORT_INT1IF_bp);
	// MED PortC Interrupt 
	PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT1LVL_gm) | PORT_INT1LVL_MED_gc;
}
void bo_init_adc() {
	// ADC: PA0,2: Strom PA1: Voltage
	port_dirin(bo_MEASURE_PORT, 0xff); // all PORTA input
	PORTCFG_MPCMASK = 0xff; // all pins
	PORTA.PIN0CTRL = PORT_OPC_PULLDOWN_gc; //all pull down
	PORTCFG_MPCMASK = Bit(bo_Vcc)|Bit(bo_CUR_a)|Bit(bo_CUR_b);
	PORTA.PIN0CTRL = PORT_OPC_TOTEM_gc;
	
	// ADC: use signed mode (unsigned mode may be broken)
	//      see Atmel Xmega Errata
	ADCA.CTRLB = Bsv(ADC_CONMODE_bp,1)/*signed mode*/ |Bsv(ADC_FREERUN_bp,0)/* not freerun */ |ADC_RESOLUTION_12BIT_gc /*12bit result right adjusted*/;
	ADCA.REFCTRL = ADC_REFSEL_INT1V_gc;			// 1V reference
	ADCA.EVCTRL = 0;							// no event
	ADCA.PRESCALER = ADC_PRESCALER_DIV128_gc;	// 128 adc prescaler
	ADCA.INTFLAGS = 0x0f;						// clear int flags
	ADCA.CALL = bo_read_production_signature_row(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0)); // set call with NVM_PROD_SIGNATURES_t:ADCACAL0
	ADCA.CALH = bo_read_production_signature_row(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1)); // set calh with NVM_PROD_SIGNATURES_t:ADCACAL1
	ADCA.CTRLA = Bit(ADC_ENABLE_bp);			// enable ADC
	
    // ADC Channel 0: current PA0
    ADCA.CH0.CTRL = ADC_CH_GAIN_1X_gc|ADC_CH_INPUTMODE_SINGLEENDED_gc;
    ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
    ADCA.CH0.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc|ADC_CH_INTLVL_OFF_gc;
    // ADC Channel 1: voltage PA1
    ADCA.CH2.CTRL = ADC_CH_GAIN_1X_gc|ADC_CH_INPUTMODE_SINGLEENDED_gc;
    ADCA.CH2.MUXCTRL = ADC_CH_MUXPOS_PIN2_gc;
    ADCA.CH2.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc|ADC_CH_INTLVL_OFF_gc;
    // ADC Channel 2: current PA2
    ADCA.CH1.CTRL = ADC_CH_GAIN_1X_gc|ADC_CH_INPUTMODE_SINGLEENDED_gc;
    ADCA.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc;
    ADCA.CH1.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc|ADC_CH_INTLVL_OFF_gc;
    // start conversions
    ADCA.CTRLA |= Bit(4)|Bit(3)|Bit(2);
}

void bo_init_timers(void) {
	// Timers for booster:
	/*
	TCC1.CCA	 DCC Decoder
    TCD0.CCA	 Periodic timer  every 10ms ~ 100Hz
    TCD0.CCB	 Kurzschluss Erkennung (shortcut detector)
    TCD0.CCC	 cutout generator
	PORTC.INT0 Stromabschaltung
	PORTC.INT1 DCC Input Signal
*/
	// DCC decoder
	TCC1.CTRLA = TC_CLKSEL_OFF_gc;
	TCC1.CTRLB = Bit(TC1_CCAEN_bp)|TC_WGMODE_NORMAL_gc; // use CCA for capture
	TCC1.CTRLD = TC_EVACT_CAPT_gc|TC_EVSEL_CH0_gc;		// Event Action: CH0, Input Capture
	TCC1.CTRLE = TC1_BYTEM_bm;							// Byte Mode
	TCC1.INTCTRLA = 0;
	TCC1.INTCTRLB = 0;
	TCC1.INTFLAGS = 0xff;
	TCC1.PER = 0xffff;
	TCC1.CNT = 0;
	TCC1.CTRLFSET = TC_CMD_RESTART_gc;
	
	// Periodic Timer: CCA:periodic timer, CCB:Kurzschluss Erkennung (shortcut detector), CCC: cutout generator
	TCD0.CTRLB = Bit(TC0_CCAEN_bp)|Bit(TC0_CCBEN_bp)|Bit(TC0_CCCEN_bp)|TC_WGMODE_NORMAL_gc;
	TCD0.CTRLD = 0;
	TCD0.CTRLE = 0; //TC1_BYTEM_bm;							// Byte Mode
	TCD0.INTCTRLA = 0;
	TCD0.INTCTRLB = TC_CCAINTLVL_LO_gc|TC_CCBINTLVL_LO_gc|TC_CCCINTLVL_LO_gc;
	// low interrupt level:
	// CCB wird in bo_dcc_sensors_init() aktiviert und in bo_dcc_sensors_off() deaktvierte
	// CCC wird in bo_do_dec_parse_packet() aktiviert und in in der ISR(TCD0_CCC_vect) wieder deaktiviert
	TCD0.INTFLAGS = 0xff;				// clear int flags
	TCD0.PER= 0xffff;					// infinite  period
	TCD0.CCA = TCD0.CNT + bo_TIMER_PERIOD; // 100 Hz
	TCD0.CTRLA = TC_CLKSEL_DIV64_gc;
}
/* void bo_do_init_system(void)
Booster init.
*/
void bo_do_init_system(void) {
	bo_init_led_port();
	bo_init_dccm_port();
	bo_init_dcc_in();
	bo_init_adc();
	bo_init_timers();
   
    // configure sleep mode: idle sleep mode, sleep mode allowed
    SLEEP.CTRL = SLEEP_SMODE_IDLE_gc|Bit(SLEEP_SEN_bp);

    g_com.productid = bo_PRODUCT_ID;
    g_com.vendorid = bo_VENDOR_ID;
    g_com.firmware_version = bo_FIRMWARE_VERSION;
    g_com.capabilities = CAP_DCC_BOOSTER;
    g_com.cap_class = 0;
    g_com.dev_desc_P = PSTR(bo_DEVICE_DESC);

    bo_v.g_booster_flags = 0;

    bo_booster_init();
}
//---------------------------------------------------------------

// DCC sensors off: L6206 Stromüberwachung and short cut timer
void bo_dcc_sensors_off(void) {
    PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT0LVL_gm) | PORT_INT0LVL_OFF_gc;
    PORTC.INT0MASK = 0; // L6206 Stromüberwachung
    
    TCD0.INTCTRLB = TCD0.INTCTRLB & ~TC0_CCBINTLVL_gm; // int for short cut timer off
    
    bo_v.g_dcc_shortcut_cnt = 0;
}

// DCC sensors init: current overflow (OV) portc int0 (pc3) falling edge and shortcut
void bo_dcc_sensors_init(void) {
    port_set(bo_LED_PORT, Bit(bo_LED_NOTAUS_b)|Bit(bo_LED_SHORTCUT_b)|Bit(bo_LED_CUR_OV_b)); // notaus, shortcut, current overflow: leds off

    clrbit(bo_v.g_booster_flags, bo_BOOSTER_FLG_CUR_OV_b); // booster flags current ov off

    // L6206 Stromüberwachung current: portc int0 (pc3) falling edge as portc.int0
    PORTC.INT0MASK = Bit(bo_CUR_OV_b);		// for pc3 (bit 3)
    PORTC.INTFLAGS = Bit(PORT_INT0IF_bp);	// clear portc.int0 interrupt flag
    PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT0LVL_gm) | PORT_INT0LVL_OFF_gc; // portc.int0 off
    PORTC.PIN3CTRL = PORT_ISC_FALLING_gc;	// falling edge
    
	// short cutsensor will be enabled in bo_dcc_sensors_shortcut_on()
    bo_v.g_dcc_shortcut_cnt = 0;
}

void bo_dcc_sensors_shortcut_on(void) {
    bo_v.g_dcc_shortcut_cnt = 0;	// shortcut count =0
	// timer tcd0.ccb: short cut timer
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
	    TCD0.CCB = TCD0.CNT + bo_TIMER_SHORT_CUT;	// tcd0.ccb is shortcut timer
	    TCD0.INTFLAGS = Bit(TC0_CCBIF_bp);			// tcd0 ccb interrupt flag reset
	    TCD0.INTCTRLB = (TCD0.INTCTRLB & ~TC0_CCBINTLVL_gm) | TC_CCBINTLVL_LO_gc; // and LO Level ccb interrupt on
	}
}

// shortcut sensor int off pc3.int0
void bo_dcc_sensors_shortcut_off(void) {
    PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT0LVL_gm) | PORT_INT0LVL_OFF_gc;
}

//-------------------------------------------------------

/* void bo_dcc_power_off_all(void)
all off: DCC Signal, sensors, ON LED off
*/
void bo_dcc_power_off_all(void) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		// Sensoren aus
        bo_dcc_sensors_off();

		// DCC Enable, IN1 und IN2 auf Low
        port_clr(bo_DCCM_PORT, Bit(bo_DCCM_EN_b)|Bit(bo_DCCM_IN1_b)|Bit(bo_DCCM_IN2_b));
        
		// setze PORTC INT1 (DCC Input Signal) zurück
        PORTC.INTFLAGS = Bit(PORT_INT1IF_bp);
        PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT1LVL_gm) | PORT_INT1LVL_OFF_gc; // DCC Input Signal Interrupt aus

        TCD0.INTCTRLB = TCD0.INTCTRLB & ~TC0_CCBINTLVL_gm; // TCD0 CCB Kurzschlusserkennung aus

        bo_v.g_timer_startup = 0; // Startup Timer reseten
		
		// LED ON off
		port_setbit(bo_LED_PORT, bo_LED_ON_b);
		// NOTAUS off
		port_setbit(bo_LED_PORT, bo_LED_NOTAUS_b);
    }
}

/* void bo_dcc_power_on_track(void)
DCC signal on track and start DCC decoder.
*/
void bo_dcc_power_on_track(void) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		// PORTC Interrupt 1 PC4 on
        PORTC.INT1MASK = Bit(bo_DCC_IN_b);
		// PC4(=DCC in) both edges
        PORTC.PIN4CTRL = PORT_ISC_BOTHEDGES_gc;
		// PORTC INT 1 on:
        PORTC.INTFLAGS = Bit(PORT_INT1IF_bp);  // clear PORTC.INT1 flag
		// Portc.INT1 as MED Int enable
        PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT1LVL_gm) | PORT_INT1LVL_MED_gc;

		// DCC sensors enable
        bo_dcc_sensors_init();
        
		// startup timer start
        bo_v.g_timer_startup = bo_TIMER_STARTUP;
		// DCC decoder start
		bo_dec_start();

		// is DCC signal H?
        if (bit_is_set(port_in(bo_DCC_IN_PORT), bo_DCC_IN_b)) {
			// bridge: IN1 to H
            port_setbit(bo_DCCM_PORT, bo_DCCM_IN1_b);
			// bridge: IN2 to L
            port_clrbit(bo_DCCM_PORT, bo_DCCM_IN2_b);
        } else {
			
			// bridge: IN1 to L
            port_clrbit(bo_DCCM_PORT, bo_DCCM_IN1_b);
			// bridge: IN2 to H
            port_setbit(bo_DCCM_PORT, bo_DCCM_IN2_b);
        }
		// bridge enable
        port_setbit(bo_DCCM_PORT, bo_DCCM_EN_b);
		
		// set LED ON to on
		port_clrbit(bo_LED_PORT, bo_LED_ON_b);
    }
}

/* void bo_dcc_notaus(void)
NOTAUS on: DCC signal off, set Notaus flag, NOTAUS LED on
 */
void bo_dcc_notaus(void) {
    bo_dcc_power_off_all(); // track power off
    setbit(bo_v.g_booster_flags, bo_BOOSTER_FLG_NOTAUS_b); // set NOTAUS flag 
    port_clrbit(bo_LED_PORT, bo_LED_NOTAUS_b); // LED NOTAUS on
}

/* void bo_booster_init(void)
Booster init.
*/
void bo_booster_init(void) {
    bo_dcc_power_off_all(); // all off
	bo_dec_init();			// init DCC decoder: PIN4 of PORTC
	
	timer_register(&bo_v.timer_startup, TIMER_RESOLUTION_16MS);	// startup timer: 16ms resolution
	//timer_register(&bo_v.timer_dcc_watchdog, TIMER_RESOLUTION_16MS);
	
	// read shortcut limit from eeprom
	volatile uint16_t a0 = offsetof(struct Eeprom_t, booster.shortcut_limit);
	bo_v.shortcut_limit = e2prom_get_word(a0);
	if (bo_v.shortcut_limit == 0xffff) {
		bo_v.shortcut_limit = bo_BOOSTER_DEFAULT_SHORTCUT_LIMIT;
	}
	// read shortcut interval from eeprom
	volatile uint16_t a1 = offsetof(struct Eeprom_t, booster.shortcut_interval);
	bo_v.shortcut_interval = e2prom_get_word(a1);
	if (bo_v.shortcut_interval == 0xffff) {
		bo_v.shortcut_interval = bo_BOOSTER_DEFAULT_SHORTCUT_INTERVAL;
	}
	bo_v.shortcut_nummax = 0;	// max shortcut number
	bo_v.eeprom_flags.write_shortcut_limit = 0;		// has shortcut limit be written?
	bo_v.eeprom_flags.write_shortcut_interval = 0;	// has shortcut interval be written?
}

/* void bo_dcc_signal_disable(void)
disable booster.
*/
static NOINLINE void bo_dcc_signal_disable(void) {
	// EN to L
    port_clrbit(bo_DCCM_PORT, bo_DCCM_EN_b);
    _delay_us(2);
}

/* void bo_dcc_signal_enable(void)
enable booster.
*/
static NOINLINE void bo_dcc_signal_enable(void) {
    _delay_us(1);
	// EN to H
    port_setbit(bo_DCCM_PORT, bo_DCCM_EN_b);
}
/* void bo_read_switches(void)
booster switches read: NOTAUS
*/
static void bo_read_switches(void) {
    uint8_t sw;

    sw = 0;
    if (bit_is_set(port_in(bo_DCCM_PORT), bo_NOTAUS_b)) {	// when switch is open
       setbit(sw, bo_SWITCH_NOTAUS_b);						// then NOTAUS
    }
    
    debounce_keys(&bo_v.g_switches, &bo_v.g_switches_t, sw);	// debounce keys
}

/* void bo_measure_task(void)
get ad values: ADC: PA0,2: Strom PA1: Voltage
called in bo_do_main
*/
static void bo_measure_task(void) {
    if (ADCA.INTFLAGS == 0x07) {
        int16_t ch0;
        int16_t ch1;
		int16_t ch2;
        // channel 0,1 and 2 conversion finished
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            ch0 = (int16_t)ADCA.CH0.RES;
            ch1 = (int16_t)ADCA.CH1.RES;
			ch2 = (int16_t)ADCA.CH2.RES;
        }
        if (ch0 < 0)
            ch0 = 0;
        if (ch1 < 0)
            ch1 = 0;
		if (ch2 < 0)
			ch2 = 0;
        bo_v.g_advals[0] = ch0;
        bo_v.g_advals[1] = ch1;
		bo_v.g_advals[2] = ch2;
        ADCA.INTFLAGS = 0x07; // clear intflags
        ADCA.CTRLA |= Bit(4)|Bit(3)|Bit(2); // start conversions
    }
}

/*  void bo_notaus_task(void)
test if NOTAUS is active
called in bo_do_main
*/
static void bo_notaus_task(void) {
    if (bit_is_set(bo_v.g_switches, bo_SWITCH_NOTAUS_b)) {
        bo_dcc_notaus();
    }
}

/* uint8_t bo_do_msg(struct sboxnet_msg_header *pmsg)
handle sboxnet msg: pmsg
messages handled:
- SBOXNET_CMD_LOCO_POWER [on]: booster on/off
*/
uint8_t bo_do_msg(struct sboxnet_msg_header *pmsg) {
    switch (pmsg->cmd) {
		// booster on/off
        case SBOXNET_CMD_LOCO_POWER: {
			// is one parameter is given [flag]?
			// flag & 0x01 != 0: ON
			// flag & 0x01 == 0: OFF
            if (pmsg->opt.len != 1) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            uint8_t flags = pmsg->data[0];
            if (flags & 0x01) { // on
				// if NOTAUS flag is set, then NOTAUS
                if (bit_is_set(bo_v.g_switches, bo_SWITCH_NOTAUS_b)) {
                    return SBOXNET_ACKRC_LOCO_NOTAUS;
                } else {
					// else reset NOTAUS flag
                    clrbit(bo_v.g_booster_flags, bo_BOOSTER_FLG_NOTAUS_b);
					// and NOTAUS LED off
                    port_setbit(bo_LED_PORT, bo_LED_NOTAUS_b);
                }
				// if booster is off?
				if (bit_is_clear(bo_v.g_booster_flags, bo_BOOSTER_FLG_ON_b)) {
					// then DCC on track
                    bo_dcc_power_on_track();
                }
				// set booster flags
                setbit(bo_v.g_booster_flags, bo_BOOSTER_FLG_ON_b);
            } else { // else off
                bo_dcc_power_off_all();
                clrbit(bo_v.g_booster_flags, bo_BOOSTER_FLG_ON_b);
            }
            pmsg->opt.len = 0;
            return SBOXNET_ACKRC_OK;
        }

    }
    return SBOXNET_ACKRC_CMD_UNKNOWN;
}

/* uint8_t bo_do_reg_read(uint16_t reg, uint16_t* pdata)
read a register
*/
uint8_t bo_do_reg_read(uint16_t reg, uint16_t* pdata) {
    switch(reg) {
		// booster flags
        case R_BOOSTER_FLAGS: *pdata = bo_v.g_booster_flags; return 0;
		// read number of ADC values
        case R_ADCVAL_NUM: *pdata = 3; return 0;
		// read value of adc channel 0: bridge 1 current
        case R_ADCVAL_0: *pdata = bo_v.g_advals[0]; return 0;
		// read value of adc channel 1: bridge supply voltage
        case R_ADCVAL_1: *pdata = bo_v.g_advals[1]; return 0;
		// read value of adc channel 2: bridge 2 current
        case R_ADCVAL_2: *pdata = bo_v.g_advals[2]; return 0;
		// read shortcut limit
        case R_BOOSTER_SHORTCUT_LIMIT:  *pdata = bo_v.shortcut_limit; return 0;
		// read max shortcut count
        case R_BOOSTER_SHORTCUT_CNT:    *pdata = bo_v.shortcut_nummax; bo_v.shortcut_nummax = 0; return 0;
		// read shortcut interval
        case R_BOOSTER_SHORTCUT_INTERVAL: *pdata = bo_v.shortcut_interval; return 0;
    }    
    return SBOXNET_ACKRC_REG_INVALID;
};

/* uint8_t bo_do_reg_write(uint16_t reg, uint16_t data, uint16_t mask)
write a register
*/
uint8_t bo_do_reg_write(uint16_t reg, uint16_t data, uint16_t mask) {
	switch(reg) {
        case R_BOOSTER_SHORTCUT_LIMIT: {
	        bo_v.shortcut_limit = data;
	        bo_v.eeprom_flags.write_shortcut_limit = 1;
	        return 0;
        }
        case R_BOOSTER_SHORTCUT_INTERVAL: {
	        bo_v.shortcut_interval = data;
	        bo_v.eeprom_flags.write_shortcut_interval = 1;
	        return 0;
        }
	}
	return SBOXNET_ACKRC_REG_INVALID;
}
__attribute__((optimize("O0")))
void bo_do_setup(void) {
}

/* void bo_do_main(void)
main booster: do measure, notaus, and write changed eeprom values
*/
void bo_do_main(void) {
	// Sparnnung und Strom Messung
    bo_measure_task();
	// NOTAUS abfragen
    bo_notaus_task();
	
	if (bo_v.eeprom_flags.write_shortcut_limit && eeprom_is_ready()) {
		eeprom_update_word((uint16_t*)offsetof(struct Eeprom_t, booster.shortcut_limit), bo_v.shortcut_limit);
		bo_v.eeprom_flags.write_shortcut_limit = 0;
	}
	if (bo_v.eeprom_flags.write_shortcut_interval && eeprom_is_ready()) {
		eeprom_update_word((uint16_t*)offsetof(struct Eeprom_t, booster.shortcut_interval), bo_v.shortcut_interval);
		bo_v.eeprom_flags.write_shortcut_interval = 0;
	}
}

void bo_do_before_bldr_activate(void) {
    bo_dcc_power_off_all();

    PORTC.INT0MASK = 0;
    PORTC.INT1MASK = 0;
    PORTC.INTFLAGS = 0xff; // clear interrupt flags;
    PORTC.INTCTRL = 0;
}

/*---------------------------------------------------------*/

/***************************************************************************
 *   Copyright (C) 2014-2015
 *   by Thomas Maier <balagi@justmail.de>
 *
 *   Copyright: See COPYING file that comes with this distribution         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 ***************************************************************************/


/* event system
   src: EVSYS_CHMUX_PORTC_PIN4_gc --> dst: TCC1.CTRLD = TC_EVACT_CAPT_gc|TC_EVSEL_CH0_gc;
   when tcc1 is started from 0,
   CCA contains number of ticks till capture is made at rise or fall of pc4=dcc-input
   TCC1: 32.000.000/64 = 500.000 = 2us
   
*/


// declaration
static void bo_do_dec_parse_packet(void);



void bo_dec_init() { // e.g.: EVSYS_CHMUX_PORTC_PIN4_gc
    bo_v.dccdec.state = bo_DEC_STATE_OFF;
    bo_v.dccdec.preamble = 0;
    bo_v.dccdec.bufsize = 0;
    bo_v.dccdec.bits = 0;
    bo_v.dccdec.lasthbit = 0;
    bo_v.dccdec.xor = 0;
    bo_v.dccdec.cutout = 0;
    
    EVSYS.CH0MUX = EVSYS_CHMUX_PORTC_PIN4_gc; // event source multiplexer: src PORTC PIN4 DCC Input --> dest TCC1.CCA
    EVSYS.CH0CTRL = 0;
}

void bo_dec_start(void) {
    bo_v.dccdec.state = bo_DEC_STATE_FIRST;
    bo_v.dccdec.preamble = 0;
    bo_v.dccdec.bufsize = 0;
    bo_v.dccdec.cutout = 0;
    
	TCC1.CCA = 87/2; // dcc high 52..64us: dcc low 90us..10ms: is CCA lower then 87/2 --> DCC H
	 
    TCC1.CTRLFSET = TC_CMD_RESTART_gc;		// start timer from begin
    TCC1.INTFLAGS = 0xff;
    TCC1.INTCTRLB = TC_CCAINTLVL_LO_gc;		// Interrupt Level low
    TCC1.CTRLA = TC_CLKSEL_DIV64_gc;		// start timer with /64 = 32Mhz / 64 = 500kHz = 2us Step
}

/*
static void bo_dec_stop(void) {
    bo_v.dccdec.state = bo_DEC_STATE_OFF;
    TCD0.INTCTRLB = 0;
    TCD0.INTFLAGS = 0xff;
    TCD0.CTRLA = TC_CLKSEL_OFF_gc;
}
*/
static void bo_dec_parse_packet(void) {
    if (bo_v.dccdec.bufsize >= 3 && bo_v.dccdec.xor == 0) {
//port_tglbit(PORTC, 6);
        bo_do_dec_parse_packet();
    }
}

static void bo_dec_halfbit(uint8_t hb) {
	// hb is the half bit
	
    switch(bo_v.dccdec.state) {
		// if in preamble?
        case bo_DEC_STATE_PREAMBLE: {
            if (hb) {
				// hb is 1, then wait till there where up to 100 hb==1
                if (bo_v.dccdec.preamble < 100) {
                    bo_v.dccdec.preamble++;
                }
            } else {
				// hb == 0
                if (bo_v.dccdec.preamble >= 20) {
					// greater then 20 preamble half bits? then it is a DCC startbit
                    bo_v.dccdec.state = bo_DEC_STATE_STARTBIT;
                } else {
					// else begin again
                    bo_v.dccdec.preamble = 0;
                }
            }
            break;
        }
		// start bit
        case bo_DEC_STATE_STARTBIT: {
            if (hb) {
				// is half bit is 1, then dec reset
				// first bit must be a 0
                goto bo_dec_reset;
            }
			// first bit is a 0, start
            bo_v.dccdec.state = bo_DEC_STATE_BIT_H1;  // first half bit after start bit
            bo_v.dccdec.bufsize = 0;	// bits in buffer
            bo_v.dccdec.bits = 0;		// number of bits in buffer
            bo_v.dccdec.xor = 0;		// xor state
            break;
        }
        case bo_DEC_STATE_BIT_H1: {
            bo_v.dccdec.lasthbit = hb;
            if (bo_v.dccdec.bits < 8) {
                bo_v.dccdec.state = bo_DEC_STATE_BIT_H2;
            } else {
                if (bo_v.dccdec.bufsize < sizeof(bo_v.dccdec.buf)) {
                    bo_v.dccdec.buf[bo_v.dccdec.bufsize++] = bo_v.dccdec.bitbuf;
                }
                bo_v.dccdec.xor ^= bo_v.dccdec.bitbuf;
                bo_v.dccdec.state = bo_DEC_STATE_STARTSTOPBIT;
            }
            break;
        }
        case bo_DEC_STATE_BIT_H2: {
            if (hb != bo_v.dccdec.lasthbit) {
                goto bo_dec_reset;
            }
            bo_v.dccdec.bitbuf <<= 1;
            if (hb) {
                bo_v.dccdec.bitbuf |= 0x01;
            }
            bo_v.dccdec.bits++;
            bo_v.dccdec.state = bo_DEC_STATE_BIT_H1;
            break;
        }
        case bo_DEC_STATE_STARTSTOPBIT: {
            if (hb != bo_v.dccdec.lasthbit) {
                goto bo_dec_reset;
            }
            if (hb) { // end of packet bit ?
                bo_dec_parse_packet();
                goto bo_dec_reset;
                
            } else { // stop bit
                bo_v.dccdec.state = bo_DEC_STATE_BIT_H1;
                bo_v.dccdec.bits = 0;
            }
            break;
        }
    }
    return;
   
   bo_dec_reset:
    bo_v.dccdec.state = bo_DEC_STATE_PREAMBLE;
    bo_v.dccdec.preamble = 0;
    bo_v.dccdec.bufsize = 0;
    bo_v.dccdec.xor = 0;
}

// ISR ......................

ISR(TCC1_CCA_vect) { // DCC Decoder
	switch (g_v.module) {
		case MODULE_GBM:
		{
			// if state is OFF, do nothing
			if (gm_v.dccdec.state == gm_DEC_STATE_OFF) {
				return;
			}
			// else restart TCC1
			TCC1.CTRLFSET = TC_CMD_RESTART_gc;
	
			if (gm_v.dccdec.state == gm_DEC_STATE_FIRST) {
				// if first bit? then preamble begin
		
				gm_v.dccdec.state = gm_DEC_STATE_PREAMBLE;
				// number of bits in preamble
				gm_v.dccdec.preamble = 0;
				// clear OVF interrupt bit to be sure
				TCC1.INTFLAGS = Bit(TC1_OVFIF_bp);
			} else {
				// do work
		
				// init DCC Low
				uint8_t hb = 0;
				// if no OVF Interrupt (no overflow, CCA contains tick since 0) and CCA is "DCC Low"
				if (bit_is_clear(TCC1.INTFLAGS, TC1_OVFIF_bp) && TCC1.CCA < (87/2) ) {
					// then it is a high (half) bit
					hb = 1;
				}
				// clear OVF interrupt bit to be sure
				TCC1.INTFLAGS = Bit(TC1_OVFIF_bp);

				// decode half bit
				gm_dec_halfbit(hb);
			}
			break;
		}
		case MODULE_BOOSTER:
		{
			// if state is OFF, do nothing
			if (bo_v.dccdec.state == bo_DEC_STATE_OFF) {
				return;
			}
			// else restart TCC1
			TCC1.CTRLFSET = TC_CMD_RESTART_gc;
			
			if(bo_v.dccdec.state == bo_DEC_STATE_FIRST) {
				// if first bit? then preamble begin
				
				bo_v.dccdec.state = bo_DEC_STATE_PREAMBLE;
				// number of bits in preamble
				bo_v.dccdec.preamble = 0;
				// clear OVF interrupt bit to be sure
				TCC1.INTFLAGS = Bit(TC1_OVFIF_bp);
				} else {
				// do work
				
				// init DCC Low
				uint8_t hb = 0;
				// if no OVF Interrupt (no overflow, CCA contains tick since 0) and CCA is "DCC Low"
				if (bit_is_clear(TCC1.INTFLAGS, TC1_OVFIF_bp) && TCC1.CCA < (87/2) ) {
					// then it is a high (half) bit
					hb = 1;
				}
				// clear OVF interrupt bit to be sure
				TCC1.INTFLAGS = Bit(TC1_OVFIF_bp);

				// decode half bit
				bo_dec_halfbit(hb);
			}
			break;
		}
	}
}

// every 10ms ~ 100Hz
ISR(TCD0_CCA_vect) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		volatile uint16_t tcca = TCD0.CCA;
		//volatile uint16_t tcnt = TCD0.CNT;
		volatile uint16_t tcca2 = tcca + bo_TIMER_PERIOD;
		TCD0.CCA = tcca2;
	}
	
	// Timertick erhöhen
	bo_v.g_timer++;
	// Startup Timer verringern wenn <> 0
	if (bo_v.g_timer_startup) {
		bo_v.g_timer_startup--;
	}
	// Taster einlesen (NOTAUS)
	bo_read_switches();
}

ISR(TCD0_CCB_vect) { // Kurzschluss Erkennung (shortcut detector)
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		TCD0.CCB = TCD0.CNT + bo_TIMER_SHORT_CUT;
	}
	if (bo_v.g_dcc_shortcut_cnt >= bo_MAX_SHORTCUT_CNT) {
		bo_dcc_power_off_all();
		setbit(bo_v.g_booster_flags, bo_BOOSTER_FLG_CUR_OV_b);
		port_clrbit(bo_LED_PORT, bo_LED_SHORTCUT_b);
		} else {
		port_setbit(bo_LED_PORT, bo_LED_SHORTCUT_b);
	}
	bo_v.g_dcc_shortcut_cnt = 0;
}

// dcc detector cutout generator
ISR(TCD0_CCC_vect) {
	switch (bo_v.dccdec.cutout) {
		case 1: { // cutout enable
			port_set(bo_DCCM_PORT, Bit(bo_DCCM_EN_b)); // EN* on
			port_clr(bo_DCCM_PORT, Bit(bo_DCCM_IN1_b)|Bit(bo_DCCM_IN2_b)); // 2 lower MOSFETs on, connect to 0V
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
				TCD0.CCC = TCD0.CNT + (432/2);
			}
			bo_v.dccdec.cutout = 2;
			bo_dcc_sensors_shortcut_off(); // disable shortcut sensors
			break;
		}
		case 2: { //cutout disable
			port_clr(bo_DCCM_PORT, Bit(bo_DCCM_EN_b)|Bit(bo_DCCM_IN1_b)|Bit(bo_DCCM_IN2_b));
			port_clrbit(bo_DCCM_PORT, bo_DCCM_CUTOUT_b); // cutout test point
			if (bo_v.dccdec.state != bo_DEC_STATE_OFF) {
				bo_v.dccdec.state = bo_DEC_STATE_FIRST;
			}
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
				TCD0.CCC = TCD0.CNT + (2*58/2);
			}
			bo_v.dccdec.cutout = 3;
			break;
		};
		case 3:
		default: {
			TCD0.INTCTRLB = (TCD0.INTCTRLB & ~TC0_CCCINTLVL_gm)|TC_CCCINTLVL_OFF_gc;
			bo_v.dccdec.cutout = 0;
			if (bo_v.g_timer_startup <= 0) {
				bo_dcc_sensors_shortcut_on(); // enable shortcut sensors after first preamble bit to avoid
				// shortcut detections during powerup after cutout
			}
		}
	}
}

ISR(PORTC_INT0_vect) { // L6206 current
	port_clrbit(bo_LED_PORT, bo_LED_SHORTCUT_b);
	
	if (bo_v.g_dcc_shortcut_cnt <= bo_MAX_SHORTCUT_CNT) {
		bo_v.g_dcc_shortcut_cnt++;
	}
	if (bo_v.g_dcc_shortcut_cnt > bo_v.g_shortcutnummax ) {
		bo_v.g_shortcutnummax = bo_v.g_dcc_shortcut_cnt;
	}
}


ISR(PORTC_INT1_vect) { // DCC Input Signal
	if (bo_v.dccdec.cutout == 0) { // NO cutout? then normal operation
		// ist DCC H?
		volatile uint8_t x = bit_is_set(port_in(bo_DCC_IN_PORT), bo_DCC_IN_b);
		if (x) { //bit_is_set(port_in(bo_DCC_IN_PORT), bo_DCC_IN_b)) {
			// zuerst aus
			//bo_dcc_signal_disable();
			// dann IN1 H
			port_setbit(bo_DCCM_PORT, bo_DCCM_IN1_b);
			// und IN2 L
			port_clrbit(bo_DCCM_PORT, bo_DCCM_IN2_b);
		} else {
			// zuerst aus
			//bo_dcc_signal_disable();
			// dann IN1 L
			port_clrbit(bo_DCCM_PORT, bo_DCCM_IN1_b);
			// unt IN2 H
			port_setbit(bo_DCCM_PORT, bo_DCCM_IN2_b);
		}
		// EN H
		bo_dcc_signal_enable();
		if (bo_v.g_timer_startup == 0) {
			// der Startup Timer 0 ist, dann Kurzschluss Sensor ein
			bo_dcc_sensors_shortcut_on();
		}
	}
}
