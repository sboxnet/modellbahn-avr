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

#include "common.h"

/*
 * - DCC Cutout is generated only at the end of the packet
 * - At the end of a cutout, shortcut detection may be triggered once or two times.
 * - Current/shortcut detection: MAX_SHORTCUT_CNT shortcuts must have occured in TIMER_SHORT_CUT time
 * - Shortcut detection is disabled in the TIMER_STARTUP time after DCC startup
 * - Shortcut detection is disabled in the first half of the first DCC bit after a cutout
 *
 * Timers:
 *  TCC0_CCB_vect    Kurzschlusserkennung
 *  TCC1_CCA_vect    Periodic Timer
 *  TCD0_CCC_vect    Cutout Generator
 *  PORTC_INT0_vect  Strom zu hoch
 *  PORTC_INT1_vect  DCC Input Signal
 *  TCD1_CCA_vect    DCC Decoder
 *
 * Ausgabe signals:
 * PC0  IN1 bridge a and b input1
 * PC1  IN2 bridge a and b input2
 * PC2  EN  Brücke ein
 *
 * PD0  Abschaltung (Kurzschluss)
 * PD1  Overflow (Strom zu hoch)
 * PD2  NOTAUS
 * PD3  ON
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

#define bo_PRODUCT_ID   0x0003
#define bo_VENDOR_ID    0x0001
#define bo_FIRMWARE_VERSION 0x0100
#define bo_DEVICE_DESC  "Booster:1"

//APP_FIRMWARE_HEADER(PRODUCT_ID, VENDOR_ID, FIRMWARE_VERSION)

#define bo_TIMER_PRESCALER   64
#define bo_TIMER_FREQ_HZ     100
#define bo_TIMER_PERIOD      (F_CPU / (bo_TIMER_PRESCALER * bo_TIMER_FREQ_HZ))  // 5000

#define bo_LED_PORT  PORTD
#define bo_LED_SHORTCUT_b    0
#define bo_LED_CUR_OV_b      1
#define bo_LED_NOTAUS_b      2
#define bo_LED_ON			 3

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

#define bo_MEASURE_PORT PORTA // PORT fuer die ADC Messungen
#define bo_CUR_a	0
#define bo_CUR_b	1
#define bo_Vcc		2

#define bo_TIMER_STARTUP  100 // 1 s

#define bo_SWITCH_NOTAUS_b  0

#define bo_BOOSTER_FLG_ON_b      0
#define bo_BOOSTER_FLG_NOTAUS_b  1
#define bo_BOOSTER_FLG_CUR_OV_b  2

#define bo_DCC_IN_PORT bo_DCCM_PORT

#define bo_TIMER_SHORT_CUT 35000 // 70ms
#define bo_MAX_SHORTCUT_CNT 5000 // * ca.13us * 5100 = 70ms

#define bo_DCC_WATCHDOG_VAL 4 // 4*16ms = 64ms

#define bo_DEC_STATE_OFF      0
#define bo_DEC_STATE_FIRST    1
#define bo_DEC_STATE_PREAMBLE 2
#define bo_DEC_STATE_STARTBIT 3
#define bo_DEC_STATE_BIT_H1   4
#define bo_DEC_STATE_BIT_H2   5
#define bo_DEC_STATE_STARTSTOPBIT 6

// declaration .........................
static void bo_do_dec_parse_packet(void);
void bo_dec_init(uint8_t evmux);
void bo_dec_start(void);
void bo_dcc_sensors_shortcut_off(void);
void bo_dcc_sensors_shortcut_on(void);


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
		//port_setbit(bo_DCC_CUTOUT_TEST_PORT, bo_DCC_CUTOUT_TEST_b); // cutout test point
	}
}

void bo_dcc_sensors_off(void) {
    PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT0LVL_gm) | PORT_INT0LVL_OFF_gc;
    PORTC.INT0MASK = 0; // L6206 Stromüberwachung
    
    TCC0.INTCTRLB &= ~TC0_CCBINTLVL_gm;
    
    bo_v.g_dcc_shortcut_cnt = 0;
}

void bo_dcc_sensors_init(void) {
    port_set(bo_LED_PORT, Bit(bo_LED_NOTAUS_b)|Bit(bo_LED_SHORTCUT_b));

    clrbit(bo_v.g_booster_flags, bo_BOOSTER_FLG_CUR_OV_b);

    // L6206 Stromüberwachung
    PORTC.INT0MASK = Bit(bo_CUR_OV_b);
    PORTC.INTFLAGS = Bit(PORT_INT0IF_bp); // clear interrupt flags;
    PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT0LVL_gm) | PORT_INT0LVL_OFF_gc;
    PORTC.PIN3CTRL = PORT_ISC_FALLING_gc;
    
    bo_v.g_dcc_shortcut_cnt = 0;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        TCC0.CCB = TCC0.CNT + bo_TIMER_SHORT_CUT;
        TCC0.INTFLAGS = Bit(TC0_CCBIF_bp);
        TCC0.INTCTRLB = (TCC0.INTCTRLB & ~TC0_CCBINTLVL_gm) | TC_CCBINTLVL_LO_gc;
    }
}

void bo_dcc_sensors_shortcut_on(void) {
    if ((PORTC.INTCTRL & PORT_INT0LVL_gm) == PORT_INT0LVL_OFF_gc) {
        PORTC.INTFLAGS = Bit(PORT_INT0IF_bp);
        PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT0LVL_gm) | PORT_INT0LVL_LO_gc;
    }
}

void bo_dcc_sensors_shortcut_off(void) {
    PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT0LVL_gm) | PORT_INT0LVL_OFF_gc;
}

void bo_dcc_power_off_all(void) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        bo_dcc_sensors_off();

        port_clr(bo_DCCM_PORT, Bit(bo_DCCM_EN_b)|Bit(bo_DCCM_IN1_b)|Bit(bo_DCCM_IN2_b));
                
        PORTC.INTFLAGS = Bit(PORT_INT1IF_bp);
        PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT1LVL_gm) | PORT_INT1LVL_OFF_gc;

        TCC0.INTCTRLB &= ~TC0_CCBINTLVL_gm;

        bo_v.g_timer_startup = 0;
    }
}

void bo_dcc_power_on_track(void) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        PORTC.INT1MASK = Bit(bo_DCC_IN_b);
        PORTC.PIN4CTRL = PORT_ISC_BOTHEDGES_gc;
        PORTC.INTFLAGS = Bit(PORT_INT1IF_bp);
        PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT1LVL_gm) | PORT_INT1LVL_HI_gc;
                
        bo_dcc_sensors_init();
        
        bo_v.g_timer_startup = bo_TIMER_STARTUP;
		
		bo_dec_start();

        if (bit_is_set(port_in(bo_DCC_IN_PORT), bo_DCC_IN_b)) {
            port_setbit(bo_DCCM_PORT, bo_DCCM_IN1_b);
            port_clrbit(bo_DCCM_PORT, bo_DCCM_IN2_b);
        } else {
            port_clrbit(bo_DCCM_PORT, bo_DCCM_IN1_b);
            port_setbit(bo_DCCM_PORT, bo_DCCM_IN2_b);
        }
        port_setbit(bo_DCCM_PORT, bo_DCCM_EN_b);
    }
}

void bo_dcc_notaus(void) {
    bo_dcc_power_off_all();
    setbit(bo_v.g_booster_flags, bo_BOOSTER_FLG_NOTAUS_b);
    port_clrbit(bo_LED_PORT, bo_LED_NOTAUS_b);
}

void bo_booster_init(void) {
    bo_dcc_power_off_all();
	bo_dec_init(EVSYS_CHMUX_PORTC_PIN4_gc);
}

static NOINLINE void bo_dcc_signal_disable(void) {
    port_clrbit(bo_DCCM_PORT, bo_DCCM_EN_b);
    _delay_us(2);
}

static NOINLINE void bo_dcc_signal_enable(void) {
    _delay_us(1);
    port_setbit(bo_DCCM_PORT, bo_DCCM_EN_b);
}

static void bo_read_switches(void) {
    uint8_t sw;

    sw = 0;
    if (bit_is_set(port_in(bo_DCCM_PORT), bo_NOTAUS_b)) {
       setbit(sw, bo_SWITCH_NOTAUS_b);
    }
    
    debounce_keys(&bo_v.g_switches, &bo_v.g_switches_t, sw);
}

static void bo_measure_task(void) {
    if (ADCA.INTFLAGS == 0x03) {
        int16_t ch0;
        int16_t ch1;
        // channel 0 and 1 conversion finished
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            ch0 = (int16_t)ADCA.CH0.RES;
            ch1 = (int16_t)ADCA.CH1.RES;
        }
        if (ch0 < 0)
            ch0 = 0;
        if (ch1 < 0)
            ch1 = 0;
        bo_v.g_advals[0] = ch0;
        bo_v.g_advals[1] = ch1;
        ADCA.INTFLAGS = 0x03; // clear intflags
        ADCA.CTRLA |= Bit(3)|Bit(2); // start conversions
    }
}

static void bo_notaus_task(void) {
    if (bit_is_set(bo_v.g_switches, bo_SWITCH_NOTAUS_b)) {
        bo_dcc_notaus();
    }
}

static uint8_t bo_read_production_signature_row(uint8_t offset) {
    uint8_t value;
    NVM.CMD = NVM_CMD_READ_CALIB_ROW_gc;
    value = pgm_read_byte(offset);
    NVM.CMD = NVM_CMD_NO_OPERATION_gc;
    return value;
}

/* void bo_do_init_system(void)
Booster init.
*/
void bo_do_init_system(void) {
	// LED Port
    port_out(bo_LED_PORT) = 0xff; // zuerst mal aus
	uint8_t m = Bit(bo_LED_NOTAUS_b)|Bit(bo_LED_CUR_OV_b)|Bit(bo_LED_NOTAUS_b)|Bit(bo_LED_ON);
    port_dirout(bo_LED_PORT, m);
    PORTCFG_MPCMASK = m; // PD3..0
    bo_LED_PORT.PIN0CTRL = PORT_OPC_TOTEM_gc;
	
    // bo_DCCM_PORT (PORTC)
	// out
    port_out(bo_DCCM_PORT) = 0;
	m = Bit(bo_DCCM_CUTOUT_b)|Bit(bo_DCCM_EN_b)|Bit(bo_DCCM_IN1_b)|Bit(bo_DCCM_IN2_b);
    port_dirout(bo_DCCM_PORT, m);
    PORTCFG_MPCMASK = m;
    PORTC.PIN0CTRL = PORT_OPC_TOTEM_gc;
	// in
	m = Bit(bo_CUR_OV_b)|Bit(bo_DCC_IN_b)|Bit(bo_NOTAUS_b);
    port_dirin(bo_DCCM_PORT, m);
    PORTCFG_MPCMASK = m;
    PORTC.PIN3CTRL = PORT_OPC_PULLUP_gc; // Eingabe als pullup
    
	// ADC
    port_dirin(bo_MEASURE_PORT, 0xff); // alles PORTA als Eingabe
    PORTCFG_MPCMASK = 0xff; // all pins
    PORTA.PIN0CTRL = PORT_OPC_PULLDOWN_gc;
    PORTCFG_MPCMASK = Bit(bo_Vcc)|Bit(bo_CUR_a)|Bit(bo_CUR_b);
    PORTA.PIN0CTRL = PORT_OPC_TOTEM_gc;
    
    // configure sleep mode: idle sleep mode, sleep mode allowed
    SLEEP.CTRL = SLEEP_SMODE_IDLE_gc|Bit(SLEEP_SEN_bp);
        
    // Periodic Timer
    TCC1.CTRLB = TC_WGMODE_NORMAL_gc;
    TCC1.CTRLD = 0;
    TCC1.CTRLE = 0;
    TCC1.INTCTRLA = 0;
    TCC1.INTCTRLB = TC_CCAINTLVL_LO_gc; // low interrupt level
    TCC1.INTFLAGS = 0xff;				// clear int flags
    TCC1.PER= 0xffff;					// infinite  period
    TCC1.CCA = TCC1.CNT + bo_TIMER_PERIOD; // 100 Hz
    TCC1.CTRLA = TC_CLKSEL_DIV64_gc;

    // ADC: use signed mode (unsigned mode may be broken)
    //      see Atmel Xmega Errata
    ADCA.CTRLB = Bsv(ADC_CONMODE_bp,1)/*signed mode*/ |Bsv(ADC_FREERUN_bp,0)/* not freerun */ |ADC_RESOLUTION_12BIT_gc /*12bit result right adjusted*/;
    ADCA.REFCTRL = ADC_REFSEL_INT1V_gc;
    ADCA.EVCTRL = 0;
    ADCA.PRESCALER = ADC_PRESCALER_DIV128_gc;
    ADCA.INTFLAGS = 0x0f; // clear int flags
    ADCA.CALL = bo_read_production_signature_row(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0));
    ADCA.CALH = bo_read_production_signature_row(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1));
    ADCA.CTRLA = Bit(ADC_ENABLE_bp);
	
    // ADC Channel 0: current
    ADCA.CH0.CTRL = ADC_CH_GAIN_1X_gc|ADC_CH_INPUTMODE_SINGLEENDED_gc;
    ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
    ADCA.CH0.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc|ADC_CH_INTLVL_OFF_gc;
    // ADC Channel 1: current
    ADCA.CH0.CTRL = ADC_CH_GAIN_1X_gc|ADC_CH_INPUTMODE_SINGLEENDED_gc;
    ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc;
    ADCA.CH0.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc|ADC_CH_INTLVL_OFF_gc;
    // ADC Channel 2: voltage
    ADCA.CH1.CTRL = ADC_CH_GAIN_1X_gc|ADC_CH_INPUTMODE_SINGLEENDED_gc;
    ADCA.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN2_gc;
    ADCA.CH1.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc|ADC_CH_INTLVL_OFF_gc;
    // start conversions
    ADCA.CTRLA |= Bit(4)|Bit(3)|Bit(2);
    
    g_com.productid = bo_PRODUCT_ID;
    g_com.vendorid = bo_VENDOR_ID;
    g_com.firmware_version = bo_FIRMWARE_VERSION;
    g_com.capabilities = CAP_DCC_BOOSTER;
    g_com.cap_class = 0;
    g_com.dev_desc_P = PSTR(bo_DEVICE_DESC);

    bo_v.g_booster_flags = 0;

    bo_booster_init();
}

uint8_t bo_do_msg(struct sboxnet_msg_header *pmsg) {
    switch (pmsg->cmd) {
        case SBOXNET_CMD_LOCO_POWER: {
            if (pmsg->opt.len != 1) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            uint8_t flags = pmsg->data[0];
            if (flags & 0x01) { // on
                if (bit_is_set(bo_v.g_switches, bo_SWITCH_NOTAUS_b)) {
                    return SBOXNET_ACKRC_LOCO_NOTAUS;
                } else {
                    clrbit(bo_v.g_booster_flags, bo_BOOSTER_FLG_NOTAUS_b);
                    port_setbit(bo_LED_PORT, bo_LED_NOTAUS_b);
                }

                if (bit_is_clear(bo_v.g_booster_flags, bo_BOOSTER_FLG_ON_b)) {
                    bo_dcc_power_on_track();
                }
                setbit(bo_v.g_booster_flags, bo_BOOSTER_FLG_ON_b);
            } else { // off
                bo_dcc_power_off_all();
                clrbit(bo_v.g_booster_flags, bo_BOOSTER_FLG_ON_b);
            }
            pmsg->opt.len = 0;
            return SBOXNET_ACKRC_OK;
        }

    }
    return SBOXNET_ACKRC_CMD_UNKNOWN;
}

uint8_t bo_do_reg_read(uint16_t reg, uint16_t* pdata) {
    switch(reg) {

        case R_BOOSTER_FLAGS: *pdata = bo_v.g_booster_flags; return 0;
        case R_ADCVAL_NUM: *pdata = 2; return 0;
        case R_ADCVAL_0: *pdata = bo_v.g_advals[0]; return 0;
        case R_ADCVAL_1: *pdata = bo_v.g_advals[1]; return 0;
        
        case 202: *pdata = bo_v.g_shortcutnummax; bo_v.g_shortcutnummax = 0; return 0;

    }    
    return SBOXNET_ACKRC_REG_INVALID;
};

uint8_t bo_do_reg_write(uint16_t reg, uint16_t data, uint16_t mask) {
	return 0;
}


void bo_do_setup(void) {
}

void bo_do_main(void) {
	// Sparnnung und Strom Messung
    bo_measure_task();
	// NOTAUS abfragen
    bo_notaus_task();
}

void bo_do_before_bldr_activate(void) {

    bo_dcc_power_off_all();

    
    PORTC.INT0MASK = 0;
    PORTC.INT1MASK = 0;
    PORTC.INTFLAGS = 0xff; // clear interrupt flags;
    PORTC.INTCTRL = 0;

    TCC0.INTCTRLA = 0;
    TCC0.INTCTRLB = 0;
    TCC0.CTRLA = 0; // stop timer
    TCC0.INTFLAGS = 0xff; // clear interrupt flags

    TCC1.INTCTRLA = 0;
    TCC1.INTCTRLB = 0;
    TCC1.CTRLA = 0; // stop timer
    TCC1.INTFLAGS = 0xff; // clear interrupt flags
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

/* Uses:
 * 
 * TCD1
 * - sets: timer 500 kHz / 2us, normal mode, 8bit
 * - CCA: DCC decoder. input capture, event 0
 */




// declaration
static void bo_do_dec_parse_packet(void);

//struct dccdec g_dccdec;


void bo_dec_init(uint8_t evmux) { // e.g.: EVSYS_CHMUX_PORTC_PIN4_gc
    bo_v.dccdec.state = bo_DEC_STATE_OFF;
    bo_v.dccdec.preamble = 0;
    bo_v.dccdec.bufsize = 0;
    bo_v.dccdec.bits = 0;
    bo_v.dccdec.lasthbit = 0;
    bo_v.dccdec.xor = 0;
    bo_v.dccdec.cutout = 0;
    
    EVSYS.CH0MUX = evmux;
    EVSYS.CH0CTRL = 0;
    
    TCD1.CTRLA = TC_CLKSEL_OFF_gc;
    TCD1.CTRLB = Bit(TC1_CCAEN_bp)|TC_WGMODE_NORMAL_gc;
    TCD1.CTRLD = TC_EVACT_CAPT_gc|TC_EVSEL_CH0_gc;
    TCD1.CTRLE = TC1_BYTEM_bm;
    TCD1.INTCTRLA = 0;
    TCD1.INTCTRLB = 0;
    TCD1.INTFLAGS = 0xff;
    TCD1.PER = 0xffff;
}

void bo_dec_start(void) {
    bo_v.dccdec.state = bo_DEC_STATE_FIRST;
    bo_v.dccdec.preamble = 0;
    bo_v.dccdec.bufsize = 0;
    bo_v.dccdec.cutout = 0;
    
    TCD1.CTRLFSET = TC_CMD_RESTART_gc;
    TCD1.INTFLAGS = 0xff;
    TCD1.INTCTRLB = TC_CCAINTLVL_LO_gc;
    TCD1.CTRLA = TC_CLKSEL_DIV64_gc;
}

/*
static void bo_dec_stop(void) {
    bo_v.dccdec.state = bo_DEC_STATE_OFF;
    TCD1.INTCTRLB = 0;
    TCD1.INTFLAGS = 0xff;
    TCD1.CTRLA = TC_CLKSEL_OFF_gc;
}
*/
static void bo_dec_parse_packet(void) {
    if (bo_v.dccdec.bufsize >= 3 && bo_v.dccdec.xor == 0) {
//port_tglbit(PORTC, 6);
        bo_do_dec_parse_packet();
    }
}

static void bo_dec_halfbit(uint8_t hb) {
    switch(bo_v.dccdec.state) {
        case bo_DEC_STATE_PREAMBLE: {
            if (hb) {
                if (bo_v.dccdec.preamble < 100) {
                    bo_v.dccdec.preamble++;
                }
            } else {
                if (bo_v.dccdec.preamble >= 20) {
                    bo_v.dccdec.state = bo_DEC_STATE_STARTBIT;
                } else {
                    bo_v.dccdec.preamble = 0;
                }
            }
            break;
        }
        case bo_DEC_STATE_STARTBIT: {
            if (hb) {
                goto bo_dec_reset;
            }
            bo_v.dccdec.state = bo_DEC_STATE_BIT_H1;
            bo_v.dccdec.bufsize = 0;
            bo_v.dccdec.bits = 0;
            bo_v.dccdec.xor = 0;
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

ISR(TCD1_CCA_vect) {
    if (bo_v.dccdec.state == bo_DEC_STATE_OFF) {
        return;
    }
    TCD1.CTRLFSET = TC_CMD_RESTART_gc;
    if(bo_v.dccdec.state == bo_DEC_STATE_FIRST) {
        bo_v.dccdec.state = bo_DEC_STATE_PREAMBLE;
        bo_v.dccdec.preamble = 0;
        TCD1.INTFLAGS = Bit(TC1_OVFIF_bp);
    } else {
        uint8_t hb = 0;
        if (bit_is_clear(TCD1.INTFLAGS, TC1_OVFIF_bp) && TCD1.CCA < (87/2) ) {
            hb = 1;
        }
        TCD1.INTFLAGS = Bit(TC1_OVFIF_bp);
        bo_dec_halfbit(hb);
    }
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
			//AWEXC.OUTOVEN = Bit(3)|Bit(2)|Bit(1)|Bit(0);
			port_clr(bo_DCCM_PORT, Bit(bo_DCCM_EN_b)|Bit(bo_DCCM_IN1_b)|Bit(bo_DCCM_IN2_b));
			//port_clrbit(DCC_CUTOUT_TEST_PORT, DCC_CUTOUT_TEST_b); // cutout test point
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

ISR(TCC0_CCB_vect) { // shortcut detector
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		TCC0.CCB = TCC0.CNT + bo_TIMER_SHORT_CUT;
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

// every 10ms ~ 100Hz
ISR(TCC1_CCA_vect) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		TCC1.CCA += bo_TIMER_PERIOD;
	}
	
	bo_v.g_timer++;
	
	if (bo_v.g_timer_startup) {
		bo_v.g_timer_startup--;
	}
	bo_read_switches();
}

ISR(PORTC_INT1_vect) { // DCC Input Signal
	if (bit_is_set(port_in(bo_DCC_IN_PORT), bo_DCC_IN_b)) {
		bo_dcc_signal_disable();
		port_setbit(bo_DCCM_PORT, bo_DCCM_IN1_b);
		port_clrbit(bo_DCCM_PORT, bo_DCCM_IN2_b);
		} else {
		bo_dcc_signal_disable();
		port_clrbit(bo_DCCM_PORT, bo_DCCM_IN1_b);
		port_setbit(bo_DCCM_PORT, bo_DCCM_IN2_b);
	}
	bo_dcc_signal_enable();
	
	if (bo_v.g_timer_startup == 0) {
		bo_dcc_sensors_shortcut_on();
	}
}
