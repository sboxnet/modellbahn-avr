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

#define DEC_STATE_OFF      0
#define DEC_STATE_FIRST    1
#define DEC_STATE_PREAMBLE 2
#define DEC_STATE_STARTBIT 3
#define DEC_STATE_BIT_H1   4
#define DEC_STATE_BIT_H2   5
#define DEC_STATE_STARTSTOPBIT 6

struct dccdec {
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

#define JOIN(x, y) JOIN_AGAIN(x, y)
#define JOIN_AGAIN(x, y) x ## y

#define DCCFUNC(func) JOIN(_DCC_PREFIX, func)

// declaration
static void DCCFUNC(do_dec_parse_packet)(void);

struct dccdec DCCFUNC(dccdec);


static void DCCFUNC(dec_init)(uint8_t evmux) { // e.g.: EVSYS_CHMUX_PORTC_PIN4_gc
    DCCFUNC(dccdec).state = DEC_STATE_OFF;
    DCCFUNC(dccdec).preamble = 0;
    DCCFUNC(dccdec).bufsize = 0;
    DCCFUNC(dccdec).bits = 0;
    DCCFUNC(dccdec).lasthbit = 0;
    DCCFUNC(dccdec).xor = 0;
    DCCFUNC(dccdec).cutout = 0;
    
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

static void DCCFUNC(dec_start)(void) {
    DCCFUNC(dccdec).state = DEC_STATE_FIRST;
    DCCFUNC(dccdec).preamble = 0;
    DCCFUNC(dccdec).bufsize = 0;
    DCCFUNC(dccdec).cutout = 0;
    
    TCD1.CTRLFSET = TC_CMD_RESTART_gc;
    TCD1.INTFLAGS = 0xff;
    TCD1.INTCTRLB = TC_CCAINTLVL_LO_gc;
    TCD1.CTRLA = TC_CLKSEL_DIV64_gc;
}

static void DCCFUNC(dec_stop)(void) {
    DCCFUNC(dccdec).state = DEC_STATE_OFF;
    TCD1.INTCTRLB = 0;
    TCD1.INTFLAGS = 0xff;
    TCD1.CTRLA = TC_CLKSEL_OFF_gc;
}

static void DCCFUNC(dec_parse_packet)(void) {
    if (DCCFUNC(dccdec).bufsize >= 3 && DCCFUNC(dccdec).xor == 0) {
//port_tglbit(PORTC, 6);
        DCCFUNC(do_dec_parse_packet)();
    }
}

static void DCCFUNC(dec_halfbit)(uint8_t hb) {
    switch(DCCFUNC(dccdec).state) {
        case DEC_STATE_PREAMBLE: {
            if (hb) {
                if (DCCFUNC(dccdec).preamble < 100) {
                    DCCFUNC(dccdec).preamble++;
                }
            } else {
                if (DCCFUNC(dccdec).preamble >= 20) {
                    DCCFUNC(dccdec).state = DEC_STATE_STARTBIT;
                } else {
                    DCCFUNC(dccdec).preamble = 0;
                }
            }
            break;
        }
        case DEC_STATE_STARTBIT: {
            if (hb) {
                goto DCCFUNC(dec_reset);
            }
            DCCFUNC(dccdec).state = DEC_STATE_BIT_H1;
            DCCFUNC(dccdec).bufsize = 0;
            DCCFUNC(dccdec).bits = 0;
            DCCFUNC(dccdec).xor = 0;
            break;
        }
        case DEC_STATE_BIT_H1: {
            DCCFUNC(dccdec).lasthbit = hb;
            if (DCCFUNC(dccdec).bits < 8) {
                DCCFUNC(dccdec).state = DEC_STATE_BIT_H2;
            } else {
                if (DCCFUNC(dccdec).bufsize < sizeof(DCCFUNC(dccdec).buf)) {
                    DCCFUNC(dccdec).buf[DCCFUNC(dccdec).bufsize++] = DCCFUNC(dccdec).bitbuf;
                }
                DCCFUNC(dccdec).xor ^= DCCFUNC(dccdec).bitbuf;
                DCCFUNC(dccdec).state = DEC_STATE_STARTSTOPBIT;
            }
            break;
        }
        case DEC_STATE_BIT_H2: {
            if (hb != DCCFUNC(dccdec).lasthbit) {
                goto DCCFUNC(dec_reset);
            }
            DCCFUNC(dccdec).bitbuf <<= 1;
            if (hb) {
                DCCFUNC(dccdec).bitbuf |= 0x01;
            }
            DCCFUNC(dccdec).bits++;
            DCCFUNC(dccdec).state = DEC_STATE_BIT_H1;
            break;
        }
        case DEC_STATE_STARTSTOPBIT: {
            if (hb != DCCFUNC(dccdec).lasthbit) {
                goto DCCFUNC(dec_reset);
            }
            if (hb) { // end of packet bit ?
                DCCFUNC(dec_parse_packet)();
                goto DCCFUNC(dec_reset);
                
            } else { // stop bit
                DCCFUNC(dccdec).state = DEC_STATE_BIT_H1;
                DCCFUNC(dccdec).bits = 0;
            }
            break;
        }
    }
    return;
    
DCCFUNC(dec_reset):
    DCCFUNC(dccdec).state = DEC_STATE_PREAMBLE;
    DCCFUNC(dccdec).preamble = 0;
    DCCFUNC(dccdec).bufsize = 0;
    DCCFUNC(dccdec).xor = 0;
}

ISR(TCD1_CCA_vect) {
    if (DCCFUNC(dccdec).state == DEC_STATE_OFF) {
        return;
    }
    TCD1.CTRLFSET = TC_CMD_RESTART_gc;
    if (DCCFUNC(dccdec).state == DEC_STATE_FIRST) {
        DCCFUNC(dccdec).state = DEC_STATE_PREAMBLE;
        DCCFUNC(dccdec).preamble = 0;
        TCD1.INTFLAGS = Bit(TC1_OVFIF_bp);
    } else {
        uint8_t hb = 0;
        if (bit_is_clear(TCD1.INTFLAGS, TC1_OVFIF_bp) && TCD1.CCA < (87/2) ) {
            hb = 1;
        }
        TCD1.INTFLAGS = Bit(TC1_OVFIF_bp);
        DCCFUNC(dec_halfbit)(hb);
    }
}

