/***************************************************************************
 *   Copyright (C) 2014
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

/*
 * TCD1: use common TCD1.CCB get sensors in cutout
 * TCC1: CCA DCC Decoder
 * ~ at 600 ns after cutout begin read feedback
 */
#include "dcc_dec.h"
#include "common.h"

#define gm_PRODUCT_ID   0x0200
#define gm_VENDOR_ID    0x1234
#define gm_FIRMWARE_VERSION 0x0103
#define gm_DEVICE_DESC  "GBmelder:1"

//APP_FIRMWARE_HEADER(gm_PRODUCT_ID, gm_VENDOR_ID, gm_FIRMWARE_VERSION)

struct gm_sensor {
    uint8_t  timer;
    uint16_t locoaddr;
    uint8_t  last_seq;
    uint8_t  holdtime;
    struct bits {
        unsigned notack:1;
        unsigned on:1;
        unsigned holdtime_changed:1;
    }        flags;
    uint8_t  retry_timer;
};


#define gm_DEFAULT_HOLDTIME 200

#define gm_NUM_SENSORS   10
struct gm_gbmelder_pipe {
	pipe_t pipe;
	uint8_t buf[64];
};
/*
struct gm_dccdec {
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
*/
struct gm_v_t {
	uint8_t g_holdtime;
	uint8_t g_old_holdtime;
	uint16_t g_sensor_bits;
	uint16_t g_sensor_bits_1;
	struct gm_sensor g_sensors[gm_NUM_SENSORS];
	uint8_t  g_led_counter;
	uint8_t  g_transmit_seq;
	struct gm_gbmelder_pipe g_locoaddr_pipe;
	//struct timer g_led_timer;
	struct timer g_power_on_timer;
	uint8_t g_power_on;
	uint16_t g_dec_lastaddr;
	struct dcc_dec_t dccdec;
};

struct gm_v_t gm_v = { 0 };  // Speicher gm Variablen

// DCC Decoder states
/*#define gm_DEC_STATE_OFF      0
#define gm_DEC_STATE_FIRST    1
#define gm_DEC_STATE_PREAMBLE 2
#define gm_DEC_STATE_STARTBIT 3
#define gm_DEC_STATE_BIT_H1   4
#define gm_DEC_STATE_BIT_H2   5
#define gm_DEC_STATE_STARTSTOPBIT 6

// declaration
static void gm_do_dec_parse_packet(void);

void gm_dec_init() {
    gm_v.dccdec.state = gm_DEC_STATE_OFF;
    gm_v.dccdec.preamble = 0;
    gm_v.dccdec.bufsize = 0;
    gm_v.dccdec.bits = 0;
    gm_v.dccdec.lasthbit = 0;
    gm_v.dccdec.xor = 0;
    gm_v.dccdec.cutout = 0;
    
    EVSYS.CH0MUX = EVSYS_CHMUX_PORTC_PIN4_gc; // event source multiplexer: src PORTC PIN4 DCC Input --> dest TCC1.CCA
    EVSYS.CH0CTRL = 0;
}

void gm_dec_start(void) {
    gm_v.dccdec.state = gm_DEC_STATE_FIRST;
    gm_v.dccdec.preamble = 0;
    gm_v.dccdec.bufsize = 0;
    gm_v.dccdec.cutout = 0;
    
	TCC1.CCA = 87/2; // dcc high 52..64us: dcc low 90us..10ms: is CCA lower then 87/2 --> DCC H
	 
    TCC1.CTRLFSET = TC_CMD_RESTART_gc;		// start timer from begin
    TCC1.INTFLAGS = 0xff;
    TCC1.INTCTRLB = TC_CCAINTLVL_LO_gc;		// Interrupt Level low
    TCC1.CTRLA = TC_CLKSEL_DIV64_gc;		// start timer with /64 = 32Mhz / 64 = 500kHz = 2us Step
}

static void gm_dec_stop(void) {
    gm_v.dccdec.state = gm_DEC_STATE_OFF;
    TCD0.INTCTRLB = 0;
    TCD0.INTFLAGS = 0xff;
    TCD0.CTRLA = TC_CLKSEL_OFF_gc;
}

static void gm_dec_parse_packet(void) {
    if (gm_v.dccdec.bufsize >= 3 && gm_v.dccdec.xor == 0) {
//port_tglbit(PORTC, 6);
        gm_do_dec_parse_packet();
    }
}

static void gm_dec_halfbit(uint8_t hb) {
	// hb is the half bit
	
    switch(gm_v.dccdec.state) {
		// if in preamble?
        case gm_DEC_STATE_PREAMBLE: {
            if (hb) {
				// hb is 1, then wait till there where up to 100 hb==1
                if (gm_v.dccdec.preamble < 100) {
                    gm_v.dccdec.preamble++;
                }
            } else {
				// hb == 0
                if (gm_v.dccdec.preamble >= 20) {
					// greater then 20 preamble half bits? then it is a DCC startbit
                    gm_v.dccdec.state = gm_DEC_STATE_STARTBIT;
                } else {
					// else begin again
                    gm_v.dccdec.preamble = 0;
                }
            }
            break;
        }
		// start bit
        case gm_DEC_STATE_STARTBIT: {
            if (hb) {
				// is half bit is 1, then dec reset
				// first bit must be a 0
                goto gm_dec_reset;
            }
			// first bit is a 0, start
            gm_v.dccdec.state = gm_DEC_STATE_BIT_H1;  // first half bit after start bit
            gm_v.dccdec.bufsize = 0;	// bits in buffer
            gm_v.dccdec.bits = 0;		// number of bits in buffer
            gm_v.dccdec.xor = 0;		// xor state
            break;
        }
        case gm_DEC_STATE_BIT_H1: {
            gm_v.dccdec.lasthbit = hb;
            if (gm_v.dccdec.bits < 8) {
                gm_v.dccdec.state = gm_DEC_STATE_BIT_H2;
            } else {
                if (gm_v.dccdec.bufsize < sizeof(gm_v.dccdec.buf)) {
                    gm_v.dccdec.buf[gm_v.dccdec.bufsize++] = gm_v.dccdec.bitbuf;
                }
                gm_v.dccdec.xor ^= gm_v.dccdec.bitbuf;
                gm_v.dccdec.state = gm_DEC_STATE_STARTSTOPBIT;
            }
            break;
        }
        case gm_DEC_STATE_BIT_H2: {
            if (hb != gm_v.dccdec.lasthbit) {
                goto gm_dec_reset;
            }
            gm_v.dccdec.bitbuf <<= 1;
            if (hb) {
                gm_v.dccdec.bitbuf |= 0x01;
            }
            gm_v.dccdec.bits++;
            gm_v.dccdec.state = gm_DEC_STATE_BIT_H1;
            break;
        }
        case gm_DEC_STATE_STARTSTOPBIT: {
            if (hb != gm_v.dccdec.lasthbit) {
                goto gm_dec_reset;
            }
            if (hb) { // end of packet bit ?
                gm_dec_parse_packet();
                goto gm_dec_reset;
                
            } else { // stop bit
                gm_v.dccdec.state = gm_DEC_STATE_BIT_H1;
                gm_v.dccdec.bits = 0;
            }
            break;
        }
    }
    return;
   
   gm_dec_reset:
    gm_v.dccdec.state = gm_DEC_STATE_PREAMBLE;
    gm_v.dccdec.preamble = 0;
    gm_v.dccdec.bufsize = 0;
    gm_v.dccdec.xor = 0;
}
*/
/*
static void gm_do_dec_parse_packet(void) {
    if (!gm_v.g_power_on || !timer_timedout(&gm_v.g_power_on_timer)) {
        return;
    }
    
    if (gm_v.dccdec.buf[0] != 0 // broadcast or reset
           && gm_v.dccdec.buf[0] != 0xff) { // idle
        
        gm_v.g_dec_lastaddr = 0;
        if (gm_v.dccdec.buf[0] <= 0x7f) {
            gm_v.g_dec_lastaddr = gm_v.dccdec.buf[0];
        } else if (gm_v.dccdec.buf[0] >= 192 && gm_v.dccdec.buf[0] <= 231) {
            gm_v.g_dec_lastaddr = ((uint16_t)gm_v.dccdec.buf[0] << 8) | gm_v.dccdec.buf[1];
        }
        if (gm_v.g_dec_lastaddr) {
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                TCD1.CCB = TCD1.CNT + (290/2); // ca 290 us after stop bit
            }
            TCD1.INTFLAGS = Bit(TC1_CCBIF_bp);
            TCD1.INTCTRLB = (TCD1.INTCTRLB & ~TC1_CCBINTLVL_gm)|TC_CCBINTLVL_LO_gc;
    
//port_clrbit(PORTB, 2);
        }
    }
}

ISR(TCD1_CCB_vect) {
    TCD1.INTCTRLB = (TCD1.INTCTRLB & ~TC1_CCBINTLVL_gm)|TC_CCBINTLVL_OFF_gc;
    
//port_setbit(PORTB, 2);
    uint8_t sensa = ~port_in(PORTA);
    uint8_t sensc = ~port_in(PORTC);
	sensc &= 0x03;
    
    uint16_t sens = ((uint16_t)sensc << 8 | sensa) & gm_v.g_sensor_bits;
    if (pipe_getfree(&gm_v.g_locoaddr_pipe.pipe) >= 4) {
        pipe_write(&gm_v.g_locoaddr_pipe.pipe, lowbyte(gm_v.g_dec_lastaddr));
        pipe_write(&gm_v.g_locoaddr_pipe.pipe, highbyte(gm_v.g_dec_lastaddr));
        pipe_write(&gm_v.g_locoaddr_pipe.pipe, lowbyte(sens));
        pipe_write(&gm_v.g_locoaddr_pipe.pipe, highbyte(sens));
    }
    gm_v.g_dec_lastaddr = 0;
}
*/d

/*
PA0..7
PC0..1
*/

static void gm_read_sensors(void) {
    uint8_t sensa = ~port_in(PORTA);
    uint8_t sensc = ~port_in(PORTC);
	sensc &= 0x03;
    
    uint16_t sensors = (uint16_t)sensc << 8 | sensa;
    
    debounce_16(&gm_v.g_sensor_bits, &gm_v.g_sensor_bits_1, sensors);
    
    uint16_t mask = 0x01;
    for (uint8_t i = 0; i < gm_NUM_SENSORS; i++, mask <<= 1) {
        if (sensors & mask) {
            if (!gm_v.g_sensors[i].flags.on) {
                gm_v.g_sensors[i].flags.notack = 1;
                gm_v.g_sensors[i].flags.on = 1;
                gm_v.g_sensors[i].retry_timer = 0;
                gm_v.g_sensors[i].locoaddr = 0;
            }
            gm_v.g_sensors[i].timer = gm_v.g_sensors[i].holdtime;
            gm_v.g_sensor_bits |= mask;
        } else if (gm_v.g_sensors[i].timer == 0) {
            if (gm_v.g_sensors[i].flags.on) {
                gm_v.g_sensors[i].flags.notack = 1;
                gm_v.g_sensors[i].flags.on = 0;
                gm_v.g_sensors[i].retry_timer = 0;                
            }
            gm_v.g_sensors[i].locoaddr = 0;
            gm_v.g_sensor_bits &= ~mask;
        }
        if (gm_v.g_sensors[i].timer)
            gm_v.g_sensors[i].timer--;
        if (gm_v.g_sensors[i].retry_timer)
            gm_v.g_sensors[i].retry_timer--;
    }
    
    if (sensors) {
        if (!gm_v.g_power_on) {
            timer_set(&gm_v.g_power_on_timer, 30);
            gm_v.g_power_on = 1;
        }
    } else if (gm_v.g_power_on) {
        gm_v.g_power_on = 0;
    }
}

uint8_t gm_get_next_transmit_seq(void) {
    uint8_t seq = gm_v.g_transmit_seq + 1;
    if (seq < 10) {
        seq = 10;
    }
    
    for (uint8_t i = 0; i < gm_NUM_SENSORS; i++) {
        if (gm_v.g_sensors[i].flags.notack && seq == gm_v.g_sensors[i].last_seq) {
            seq++;
            i = 0; // retry
        }   
    }

    gm_v.g_transmit_seq = seq;
    return seq;
}

void gm_do_init_system(void) {
	/* PD0...7,PC6,PC7 LEDs */
	// LED is on when H
    port_out(PORTD) = 0; // LED off
    port_dirout(PORTD, 0xff); // as output
    PORTCFG_MPCMASK = 0xff; // all pins
    PORTD.PIN0CTRL = PORT_OPC_TOTEM_gc; // as TOTEM pole

	// PA0..7 input, pull up
    port_dirin(PORTA, 0xff);
    PORTCFG_MPCMASK = 0xff; // all pins
    PORTA.PIN0CTRL = PORT_OPC_PULLUP_gc;
    // PC0..1 input, pull up
	// PC4 as normal input
	port_dirin(PORTC, 0x01f);
    PORTCFG_MPCMASK = 0x03; // bit 0..1
    PORTC.PIN0CTRL = PORT_OPC_PULLUP_gc;
	
    // configure sleep mode: idle sleep mode, sleep mode allowed
    SLEEP.CTRL = SLEEP_SMODE_IDLE_gc|Bit(SLEEP_SEN_bp);
    // power reduction
    PR.PRPA = Bit(PR_DAC_bp)|Bit(PR_ADC_bp)|Bit(PR_AC_bp);
    PR.PRPB = Bit(PR_DAC_bp)|Bit(PR_ADC_bp)|Bit(PR_AC_bp);
    PR.PRPC = Bit(PR_TWI_bp)|Bit(PR_USART1_bp)|Bit(PR_USART0_bp)|Bit(PR_SPI_bp)|Bit(PR_HIRES_bp); // PR_TC1_bp PR_TC0_bp;
    PR.PRPD = Bit(PR_TWI_bp)|Bit(PR_USART1_bp)|Bit(PR_USART0_bp)|Bit(PR_SPI_bp)|Bit(PR_HIRES_bp); // PR_TC1_bp PR_TC0_bp;
    
    g_com.productid = gm_PRODUCT_ID;
    g_com.vendorid = gm_VENDOR_ID;
    g_com.firmware_version = gm_FIRMWARE_VERSION;
    g_com.capabilities = CAP_FB_OCCUPANCY;
    g_com.cap_class = 0;
    g_com.dev_desc_P = PSTR(gm_DEVICE_DESC);
    
    //timer_register(&gm_v.g_led_timer, TIMER_RESOLUTION_1MS);
    
    pipe_init(&gm_v.g_locoaddr_pipe.pipe, sizeof(gm_v.g_locoaddr_pipe.buf));
    
    memset(gm_v.g_sensors, 0, sizeof(gm_v.g_sensors));
    
    // DCC Input Pin
    PORTC.PIN4CTRL = PORT_OPC_PULLDOWN_gc|PORT_ISC_BOTHEDGES_gc;
    dcc_dec_init(&gm_v.dccdec);
    
    timer_register(&gm_v.g_power_on_timer, TIMER_RESOLUTION_16MS);
    gm_v.g_power_on = 0;
    
    
	//volatile uint16_t a1 = offsetof(struct Eeprom_t, gbm.holdtime);
	gm_v.g_holdtime = e2prom_get_byte((uint16_t *)offsetof(struct Eeprom_t, gbm.holdtime));
    if (gm_v.g_holdtime == 0xff) {
        gm_v.g_holdtime = gm_DEFAULT_HOLDTIME;
    }
    gm_v.g_old_holdtime = gm_v.g_holdtime;
	//
    for (uint8_t i = 0; i < gm_NUM_SENSORS; i++) {
        uint8_t ht = e2prom_get_byte((uint16_t*)offsetof(struct Eeprom_t,gbm.sensors[i].holdtime));
        if (ht == 0xff) {
            ht = gm_v.g_holdtime;
        }
        gm_v.g_sensors[i].holdtime = ht;
    }
}

uint8_t gm_do_msg(struct sboxnet_msg_header *pmsg) {
    if (pmsg->dstaddr == SBOXNET_ADDR_BROADCAST) {
        return SBOXNET_ACKRC_CMD_UNKNOWN;
    }
    switch (pmsg->cmd) {
        case SBOXNET_CMD_FB_LOCOADDR|0x80: {
            if (pmsg->opt.len != 0) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            for (uint8_t i = 0; i < gm_NUM_SENSORS; i++) {
                if (gm_v.g_sensors[i].flags.notack && pmsg->seq == gm_v.g_sensors[i].last_seq) {
                    gm_v.g_sensors[i].flags.notack = 0;
                    gm_v.g_sensors[i].last_seq = 0;
                }
            }
            return SBOXNET_ACKRC_SEND_NO_ANSWER;
        }
    }
    return SBOXNET_ACKRC_CMD_UNKNOWN;
}

uint8_t gm_do_reg_read(uint16_t reg, uint16_t* pdata) {
    switch(reg) {
        case R_FB_NUM:      *pdata = gm_NUM_SENSORS; return 0;
        case R_FB_VALUE0:   *pdata = gm_v.g_sensor_bits; return 0;
        
        case R_GBM_HOLDTIME: *pdata = gm_v.g_holdtime; return 0;
    }    
    if (reg >= R_GBM_PAR_0 && reg < R_GBM_PAR_15+NUM_GBM_PARS) {
        uint8_t roffs = reg & NUM_GBM_PARS_MASK;
        uint8_t gbmnum = (reg-R_GBM_PAR_0) / NUM_GBM_PARS;
        switch (roffs) {
            case ROFFS_GBM_HOLDTIME: {
                *pdata = gm_v.g_sensors[gbmnum].holdtime;
                return 0;
            }
            case ROFFS_GBM_LOCOADDR: {
                *pdata = gm_v.g_sensors[gbmnum].locoaddr;
                return 0;
            }
            default: {
                break;
            }
        }
    }
    return SBOXNET_ACKRC_REG_INVALID;
};

uint8_t gm_do_reg_write(uint16_t reg, uint16_t data, uint16_t mask) {
    switch (reg) {
        case R_GBM_HOLDTIME: {
            gm_v.g_old_holdtime = gm_v.g_holdtime;
            gm_v.g_holdtime = (data >= 0xff ? 0xff : (uint8_t)data);
            return 0;
        }
    }
    if (reg >= R_GBM_PAR_0 && reg < R_GBM_PAR_15+NUM_GBM_PARS) {
        uint8_t roffs = reg & NUM_GBM_PARS_MASK;
        uint8_t gbmnum = (reg-R_GBM_PAR_0) / NUM_GBM_PARS;
        switch (roffs) {
            case ROFFS_GBM_HOLDTIME: {
                gm_v.g_sensors[gbmnum].holdtime = (data >= 0xff ? 0xff : (uint8_t)data);
                gm_v.g_sensors[gbmnum].flags.holdtime_changed = 1;
                return 0;
            }
            default: {
                break;
            }
        }
    }
    return SBOXNET_ACKRC_REG_INVALID;
}

void gm_do_setup(void) {
    dcc_dec_start();
}

void gm_do_main(void) {    
	gm_read_sensors();
    /*if (timer_timedout(&gbm_v.g_led_timer)) {
        timer_set(&gbm_v.g_led_timer, 5); // 5ms

        gbm_read_sensors();
        
        gbm_multiplex_leds();
    }*/
        
    uint8_t canread = 0;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        canread = pipe_count(&gm_v.g_locoaddr_pipe.pipe);
    }
    if (canread >= 4) {
        uint16_t sens = 0;
        uint16_t lastaddr = 0;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { lastaddr = pipe_read(&gm_v.g_locoaddr_pipe.pipe); }
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { lastaddr |= ((uint16_t)pipe_read(&gm_v.g_locoaddr_pipe.pipe) << 8); }
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { sens = pipe_read(&gm_v.g_locoaddr_pipe.pipe); }
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { sens |= ((uint16_t)pipe_read(&gm_v.g_locoaddr_pipe.pipe) << 8); }
        
        uint16_t mask = 0x01;
        for (uint8_t i = 0; i < 16; i++, mask <<= 1) {
            if (sens & mask) {
                if (gm_v.g_sensors[i].flags.on && gm_v.g_sensors[i].locoaddr != lastaddr) {
                    gm_v.g_sensors[i].locoaddr = lastaddr;
                    gm_v.g_sensors[i].flags.notack = 1;
                    gm_v.g_sensors[i].retry_timer = 0;
                }
            }
        }
    }
    
    //if (bit_is_clear(g_dev_state, DEV_STATE_FLG_WATCHDOG_b)) {
        for (uint8_t i = 0; i < gm_NUM_SENSORS; i++) {
            if (gm_v.g_sensors[i].retry_timer == 0 && gm_v.g_sensors[i].flags.notack) {
                gm_v.g_sensors[i].retry_timer = 50;
                
                struct sboxnet_msg_max msg;
                msg.msgh.dstaddr = 0;
                msg.msgh.seq = gm_v.g_sensors[i].last_seq = gm_get_next_transmit_seq();
                msg.msgh.opt.len = 3;
                msg.msgh.cmd = SBOXNET_CMD_FB_LOCOADDR;
                if (gm_v.g_sensors[i].flags.on) {
                    msg.data[0] = i|0x80;
                } else {
                    msg.data[0] = i;
                }
                msg.data[1] = lowbyte(gm_v.g_sensors[i].locoaddr);
                msg.data[2] = highbyte(gm_v.g_sensors[i].locoaddr);
                
                sboxnet_send_msg(&msg.msgh);
                break; // only send one per do_main() loop
            }
        }
    //}
    
    // write eeprom
    if (gm_v.g_holdtime != gm_v.g_old_holdtime && eeprom_is_ready()) {
		eeprom_update_byte((uint8_t*)offsetof(struct Eeprom_t,gbm.holdtime), gm_v.g_holdtime);
        gm_v.g_old_holdtime = gm_v.g_holdtime;
    }
    for (uint8_t i = 0; i < gm_NUM_SENSORS; i++) {
        if (gm_v.g_sensors[i].flags.holdtime_changed && eeprom_is_ready()) {
			eeprom_update_byte((uint8_t*)offsetof(struct Eeprom_t,gbm.sensors[i].holdtime),gm_v.g_sensors[i].holdtime);
            gm_v.g_sensors[i].flags.holdtime_changed = 0;
        }
    }
    
	sleep_cpu();
}

void gm_do_before_bldr_activate(void) {
    //gm_dec_stop();
}

// ISR(TCC1_CCA_vect) { // DCC Decoder is in dcc_dec.c depends on g_v.module
