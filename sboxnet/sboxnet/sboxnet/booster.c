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
 */


#define PRODUCT_ID   0x0003
#define VENDOR_ID    0x0001
#define FIRMWARE_VERSION 0x0100
#define DEVICE_DESC  "Booster:1"

//APP_FIRMWARE_HEADER(PRODUCT_ID, VENDOR_ID, FIRMWARE_VERSION)


#define TIMER_PRESCALER   64
#define TIMER_FREQ_HZ     100
#define TIMER_PERIOD      (F_CPU / (TIMER_PRESCALER * TIMER_FREQ_HZ))  // 5000



static uint8_t g_timer = 0;



#define LED_PORT  PORTD
#define LED_NOTAUS_b      0
#define LED_CUR_OV_b      1
#define LED_CUR_SHORT_b   2

#define DCCM_PORT  PORTC
#define DCCM_EN_b   2
#define DCCM_IN1_b  0
#define DCCM_IN2_b  1
#define DCCM_CUTOUT_b 7

#define CUR_OV_PORT  PORTC
#define CUR_OV_b   3

#define NOTAUS_PORT  PORTC
#define NOTAUS_b   5

#define TIMER_STARTUP  100 // 1 s
static uint8_t g_timer_startup = 0;

static uint16_t g_advals[2] = { 0, 0 };

#define SWITCH_NOTAUS_b  0
static uint8_t g_switches = 0;
static uint8_t g_switches_t = 0;

#define BOOSTER_FLG_ON_b      0
#define BOOSTER_FLG_NOTAUS_b  1
#define BOOSTER_FLG_CUR_OV_b  2
static uint8_t g_booster_flags = 0;



#define DCC_IN_PORT PORTC
#define DCC_IN_b   4






#define TIMER_SHORT_CUT 35000 // 70ms
#define MAX_SHORTCUT_CNT 5000 // * ca.13us * 5100 = 70ms
uint16_t g_dcc_shortcut_cnt = 0;

uint16_t g_shortcutnummax = 0;

#define DCC_WATCHDOG_VAL 4 // 4*16ms = 64ms

#include "dccdec.hh"

static void do_dec_parse_packet(void) {
	timer_set(&g_booster.timer_dcc_watchdog, DCC_WATCHDOG_VAL);
	
	if (g_dccdec.buf[0] != 0 // broadcast or reset
	&& g_dccdec.buf[0] != 0xff) { // idle
		// produce cutout with TCD0.CCC
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			TCD0.CCC = TCD0.CNT + (28/2);
		}
		TCD0.INTFLAGS = Bit(TC0_CCCIF_bp);
		TCD0.INTCTRLB = (TCD0.INTCTRLB & ~TC0_CCCINTLVL_gm)|TC_CCCINTLVL_LO_gc;
		g_dccdec.cutout = 1;
		port_setbit(DCC_CUTOUT_TEST_PORT, DCC_CUTOUT_TEST_b); // cutout test point
	}
}

// dcc detector cutout generator
ISR(TCD0_CCC_vect) {
	switch (g_dccdec.cutout) {
		case 1: { // cutout enable
			port_set(DCCM_PORT, Bit(DCCM_EN1_b)|Bit(DCCM_EN2_b)); // EN* on
			port_clr(DCCM_PORT, Bit(DCCM_IN1_b)|Bit(DCCM_IN2_b)); // 2 lower MOSFETs on, connect to 0V
			AWEXC.OUTOVEN = 0;                                    // output override enable off
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
				TCD0.CCC = TCD0.CNT + (432/2);
			}
			g_dccdec.cutout = 2;
			booster_sensors_shortcut_off(); // disable shortcut sensors
			break;
		}
		case 2: { //cutout disable
			AWEXC.OUTOVEN = Bit(3)|Bit(2)|Bit(1)|Bit(0);
			port_clr(DCCM_PORT, Bit(DCCM_EN1_b)|Bit(DCCM_EN2_b)|Bit(DCCM_IN1_b)|Bit(DCCM_IN2_b));
			port_clrbit(DCC_CUTOUT_TEST_PORT, DCC_CUTOUT_TEST_b); // cutout test point
			if (g_dccdec.state != DEC_STATE_OFF) {
				g_dccdec.state = DEC_STATE_FIRST;
			}
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
				TCD0.CCC = TCD0.CNT + (2*58/2);
			}
			g_dccdec.cutout = 3;
			break;
		};
		case 3:
		default: {
			TCD0.INTCTRLB = (TCD0.INTCTRLB & ~TC0_CCCINTLVL_gm)|TC_CCCINTLVL_OFF_gc;
			g_dccdec.cutout = 0;
			if (g_booster.timer_startup.value <= 0) {
				booster_sensors_shortcut_on(); // enable shortcut sensors after first preamble bit to avoid
				// shortcut detections during powerup after cutout
			}
		}
	}
}

void dcc_sensors_off(void) {
    PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT0LVL_gm) | PORT_INT0LVL_OFF_gc;
    PORTC.INT0MASK = 0; // L6206 Stromüberwachung
    
    TCC0.INTCTRLB &= ~TC0_CCBINTLVL_gm;
    
    g_dcc_shortcut_cnt = 0;
}

void dcc_sensors_init(void) {
    port_set(LED_PORT, Bit(LED_NOTAUS_b)|Bit(LED_CUR_SHORT_b));

    clrbit(g_booster_flags, BOOSTER_FLG_CUR_OV_b);

    // L6206 Stromüberwachung
    PORTC.INT0MASK = Bit(CUR_OV_b);
    PORTC.INTFLAGS = Bit(PORT_INT0IF_bp); // clear interrupt flags;
    PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT0LVL_gm) | PORT_INT0LVL_OFF_gc;
    PORTC.PIN3CTRL = PORT_ISC_FALLING_gc;
    
    g_dcc_shortcut_cnt = 0;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        TCC0.CCB = TCC0.CNT + TIMER_SHORT_CUT;
        TCC0.INTFLAGS = Bit(TC0_CCBIF_bp);
        TCC0.INTCTRLB = (TCC0.INTCTRLB & ~TC0_CCBINTLVL_gm) | TC_CCBINTLVL_LO_gc;
    }
}

void dcc_sensors_shortcut_on(void) {
    if ((PORTC.INTCTRL & PORT_INT0LVL_gm) == PORT_INT0LVL_OFF_gc) {
        PORTC.INTFLAGS = Bit(PORT_INT0IF_bp);
        PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT0LVL_gm) | PORT_INT0LVL_LO_gc;
    }
}

void dcc_sensors_shortcut_off(void) {
    PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT0LVL_gm) | PORT_INT0LVL_OFF_gc;
}

void dcc_power_off_all(void) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        dcc_sensors_off();

        port_clr(DCCM_PORT, Bit(DCCM_EN_b)|Bit(DCCM_IN1_b)|Bit(DCCM_IN2_b));
                
        PORTC.INTFLAGS = Bit(PORT_INT1IF_bp);
        PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT1LVL_gm) | PORT_INT1LVL_OFF_gc;

        TCC0.INTCTRLB &= ~TC0_CCBINTLVL_gm;

        g_timer_startup = 0;
    }
}

void dcc_power_on_track(void) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        PORTC.INT1MASK = Bit(DCC_IN_b);
        PORTC.PIN4CTRL = PORT_ISC_BOTHEDGES_gc;
        PORTC.INTFLAGS = Bit(PORT_INT1IF_bp);
        PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT1LVL_gm) | PORT_INT1LVL_HI_gc;
                
        dcc_sensors_init();
        
        g_timer_startup = TIMER_STARTUP;

        if (bit_is_set(port_in(DCC_IN_PORT), DCC_IN_b)) {
            port_setbit(DCCM_PORT, DCCM_IN1_b);
            port_clrbit(DCCM_PORT, DCCM_IN2_b);
        } else {
            port_clrbit(DCCM_PORT, DCCM_IN1_b);
            port_setbit(DCCM_PORT, DCCM_IN2_b);
        }
        port_setbit(DCCM_PORT, DCCM_EN_b);
    }
}

void dcc_notaus(void) {
    dcc_power_off_all();
    setbit(g_booster_flags, BOOSTER_FLG_NOTAUS_b);
    port_clrbit(LED_PORT, LED_NOTAUS_b);
}

ISR(PORTC_INT0_vect) { // L6206 current
    port_clrbit(LED_PORT, LED_CUR_SHORT_b);
    
    if (g_dcc_shortcut_cnt <= MAX_SHORTCUT_CNT) {
        g_dcc_shortcut_cnt++;
    }
    if (g_dcc_shortcut_cnt > g_shortcutnummax ) {
        g_shortcutnummax = g_dcc_shortcut_cnt;
    }
}

ISR(TCC0_CCB_vect) { // shortcut detector
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        TCC0.CCB = TCC0.CNT + TIMER_SHORT_CUT;
    }
    if (g_dcc_shortcut_cnt >= MAX_SHORTCUT_CNT) {
        dcc_power_off_all();
        setbit(g_booster_flags, BOOSTER_FLG_CUR_OV_b);
        port_clrbit(LED_PORT, LED_CUR_SHORT_b);
    } else {
        port_setbit(LED_PORT, LED_CUR_SHORT_b);
    }
    g_dcc_shortcut_cnt = 0;
}

void booster_init(void) {
    dcc_power_off_all();
}

static NOINLINE void dcc_signal_disable(void) {
    port_clrbit(DCCM_PORT, DCCM_EN_b);
    _delay_us(2);
}

static NOINLINE void dcc_signal_enable(void) {
    _delay_us(1);
    port_setbit(DCCM_PORT, DCCM_EN_b);
}

ISR(PORTC_INT1_vect) { // DCC Input Signal
    if (bit_is_set(port_in(DCC_IN_PORT), DCC_IN_b)) {
        dcc_signal_disable();
        port_setbit(DCCM_PORT, DCCM_IN1_b);
        port_clrbit(DCCM_PORT, DCCM_IN2_b);
    } else {
        dcc_signal_disable();
        port_clrbit(DCCM_PORT, DCCM_IN1_b);
        port_setbit(DCCM_PORT, DCCM_IN2_b);
    }
    dcc_signal_enable();
    
    if (g_timer_startup == 0) {
        dcc_sensors_shortcut_on();
    }
}

static void read_switches(void) {
    uint8_t sw;

    sw = 0;
    if (bit_is_set(port_in(NOTAUS_PORT), NOTAUS_b)) {
       setbit(sw, SWITCH_NOTAUS_b);
    }
    
    debounce_keys(&g_switches, &g_switches_t, sw);
}


// every 10ms ~ 100Hz
ISR(TCC0_CCA_vect) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        TCC0.CCA += TIMER_PERIOD;
    }
    
    g_timer++;
        
    if (g_timer_startup) {
        g_timer_startup--;
    }
    read_switches();
}

static void measure_task(void) {
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
        g_advals[0] = ch0;
        g_advals[1] = ch1;
        ADCA.INTFLAGS = 0x03; // clear intflags
        ADCA.CTRLA |= Bit(3)|Bit(2); // start conversions
    }
}

static void notaus_task(void) {
    if (bit_is_set(g_switches, SWITCH_NOTAUS_b) || bit_is_set(g_v.dev_state, DEV_STATE_FLG_WATCHDOG_b)) {
        dcc_notaus();
    }
}

static uint8_t read_production_signature_row(uint8_t offset) {
    uint8_t value;
    NVM.CMD = NVM_CMD_READ_CALIB_ROW_gc;
    value = pgm_read_byte(offset);
    NVM.CMD = NVM_CMD_NO_OPERATION_gc;
    return value;
}

void do_init_system(void) {
	
    port_out(PORTD) = 0xff;
    port_dirout(PORTD, 0x07);
    PORTCFG_MPCMASK = 0x07; // PD2..0
    PORTD.PIN0CTRL = PORT_OPC_TOTEM_gc;
    
    port_out(PORTC) = 0;
    port_dirout(PORTC, 0x57);
    PORTCFG_MPCMASK = 0x57;
    PORTC.PIN0CTRL = PORT_OPC_TOTEM_gc;
    port_dirin(PORTC, Bit(7)|Bit(5)|Bit(3));
    PORTCFG_MPCMASK = Bit(7)|Bit(5)|Bit(3);
    PORTC.PIN3CTRL = PORT_OPC_PULLUP_gc;
    
    port_dirin(PORTA, 0xff);
    PORTCFG_MPCMASK = 0xff; // all pins
    PORTA.PIN0CTRL = PORT_OPC_PULLDOWN_gc;
    PORTCFG_MPCMASK = 0x03;
    PORTA.PIN0CTRL = PORT_OPC_TOTEM_gc;
    
    // configure sleep mode: idle sleep mode, sleep mode allowed
    SLEEP.CTRL = SLEEP_SMODE_IDLE_gc|Bit(SLEEP_SEN_bp);
        
    // timer
    TCC0.CTRLB = TC_WGMODE_NORMAL_gc;
    TCC0.CTRLD = 0;
    TCC0.CTRLE = 0;
    TCC0.INTCTRLA = 0;
    TCC0.INTCTRLB = TC_CCAINTLVL_LO_gc;
    TCC0.INTFLAGS = 0xff;
    TCC0.PER = 0xffff;
    TCC0.CCA = TCC0.CNT + TIMER_PERIOD;
    TCC0.CTRLA = TC_CLKSEL_DIV64_gc;

    // ADC: use signed mode (unsigned mode may be broken) and Vcc/1.6 reference (1V internal reference may be broken)
    //      see Atmel Xmega Errata
    ADCA.CTRLB = Bsv(ADC_CONMODE_bp,1)|Bsv(ADC_FREERUN_bp,0)|ADC_RESOLUTION_12BIT_gc;
    ADCA.REFCTRL = ADC_REFSEL_VCC_gc; //|Bit(ADC_BANDGAP_bp);
    ADCA.EVCTRL = 0;
    ADCA.PRESCALER = ADC_PRESCALER_DIV128_gc;
    ADCA.INTFLAGS = 0x0f;
    ADCA.CALL = read_production_signature_row(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0));
    ADCA.CALH = read_production_signature_row(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1));
    ADCA.CTRLA = Bit(ADC_ENABLE_bp);
    // ADC Channel 0: current
    ADCA.CH0.CTRL = ADC_CH_GAIN_1X_gc|ADC_CH_INPUTMODE_SINGLEENDED_gc;
    ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
    ADCA.CH0.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc|ADC_CH_INTLVL_OFF_gc;
    // ADC Channel 1: voltage
    ADCA.CH1.CTRL = ADC_CH_GAIN_1X_gc|ADC_CH_INPUTMODE_SINGLEENDED_gc;
    ADCA.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc;
    ADCA.CH1.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc|ADC_CH_INTLVL_OFF_gc;
    // start conversions
    ADCA.CTRLA |= Bit(3)|Bit(2);
    
    g_com.productid = PRODUCT_ID;
    g_com.vendorid = VENDOR_ID;
    g_com.firmware_version = FIRMWARE_VERSION;
    g_com.capabilities = 0
        |CAP_DCC_BOOSTER
        ;
    g_com.cap_class = 0;
    g_com.dev_desc_P = PSTR(DEVICE_DESC);

    g_booster_flags = 0;

    booster_init();
}

uint8_t do_msg(struct sboxnet_msg_max *pmsg) {
    switch (pmsg->msg.cmd) {
        case SBOXNET_CMD_LOCO_POWER: {
            if (pmsg->msg.opt.len != 1) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            uint8_t flags = pmsg->data[0];
            if (flags & 0x01) { // on
                if (bit_is_set(g_switches, SWITCH_NOTAUS_b) || bit_is_set(g_v.dev_state, DEV_STATE_FLG_WATCHDOG_b)) {
                    return SBOXNET_ACKRC_LOCO_NOTAUS;
                } else {
                    clrbit(g_booster_flags, BOOSTER_FLG_NOTAUS_b);
                    port_setbit(LED_PORT, LED_NOTAUS_b);
                }

                if (bit_is_clear(g_booster_flags, BOOSTER_FLG_ON_b)) {
                    dcc_power_on_track();
                }
                setbit(g_booster_flags, BOOSTER_FLG_ON_b);
            } else { // off
                dcc_power_off_all();
                clrbit(g_booster_flags, BOOSTER_FLG_ON_b);
            }
            pmsg->msg.opt.len = 0;
            return SBOXNET_ACKRC_OK;
        }

    }
    return SBOXNET_ACKRC_CMD_UNKNOWN;
}

uint8_t do_reg_read(uint16_t reg, uint16_t* pdata) {
    switch(reg) {

        case R_BOOSTER_FLAGS: *pdata = g_booster_flags; return 0;
        case R_ADCVAL_NUM: *pdata = 2; return 0;
        case R_ADCVAL_0: *pdata = g_advals[0]; return 0;
        case R_ADCVAL_1: *pdata = g_advals[1]; return 0;
        
        case 202: *pdata = g_shortcutnummax; g_shortcutnummax = 0; return 0;

    }    
    return SBOXNET_ACKRC_REG_INVALID;
};


void do_setup(void) {
}

void do_main(void) {
    measure_task();
    notaus_task();
}

void do_before_bldr_activate(void) {

    dcc_power_off_all();

    
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
