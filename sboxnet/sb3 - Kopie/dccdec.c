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

struct dccdec g_dccdec;

// declaration
static void do_dec_parse_packet(void);

/* void dec_init(uint8_t evmux)
 *   evmux  Wert für Kanal 0 Multiplexer C0MUX z.b. EVSYS_CHMUX_PORTC_PIN4_g
 * DCC Decoder Init. evmux Wert für Kanal 0 Multiplexer C0MUX z.b. EVSYS_CHMUX_PORTC_PIN4_gc
 * Event Channel wird benutzt im DCC Generator um die DMA Kanäle zu wechseln
 */
static void dec_init(uint8_t evmux) { // e.g.: EVSYS_CHMUX_PORTC_PIN4_gc
	// DCC Decoder Init
	
    g_dccdec.state = DEC_STATE_OFF; // Status OFF
    g_dccdec.preamble = 0;  // keine Preamble 
    g_dccdec.bufsize = 0;   // aktuelle Buffergröße
    g_dccdec.bits = 0;      // wieviele Bits sind im Buffer
    g_dccdec.lasthbit = 0;  // letzte Halfbit
    g_dccdec.xor = 0;       // XOR Wert für die CRC
    g_dccdec.cutout = 0;    // Sind wir ein Cutout?
    
	// setze Event System Multiplexer EVSYS_CHMUX_PORTC_PIN4_gc
    EVSYS.CH0MUX = evmux;
	// Event Kanal 0: Quadratur ausgeschaltet
    EVSYS.CH0CTRL = 0;
    
	// Timer D1 off
    TCD1.CTRLA = TC_CLKSEL_OFF_gc;
	// Timer D1 CCA Enable, Normal WG Mode
    TCD1.CTRLB = Bit(TC1_CCAEN_bp)|TC_WGMODE_NORMAL_gc;
	// Timer D1 Event Action Input Capture, Event Channel 0
    TCD1.CTRLD = TC_EVACT_CAPT_gc|TC_EVSEL_CH0_gc;
	// Upper Counter to Byte Mode
    TCD1.CTRLE = TC1_BYTEM_bm;
	// TCD1 Kein OVF oder ERR Interrupt
    TCD1.INTCTRLA = 0;
	// TCD1 Kein CC Interrupt
    TCD1.INTCTRLB = 0;
	// Interrupt Flags zurücksetzen
    TCD1.INTFLAGS = 0xff;
	// TCD1 Periode auf 0xffff
    TCD1.PER = 0xffff;
}

/* void dec_start(void)
 * Start den DCC Decoder.
 */
static void dec_start(void) {
	// Erstes Bit
    g_dccdec.state = DEC_STATE_FIRST;
	// noch keine Preamble
    g_dccdec.preamble = 0;
	// der Buffer ist noch leer
    g_dccdec.bufsize = 0;
	// kein Cutout
    g_dccdec.cutout = 0;
    
	// TCD1 Restart
    TCD1.CTRLFSET = TC_CMD_RESTART_gc;
	// alle Interrupt Flags clearen
    TCD1.INTFLAGS = 0xff;
	// TCD2.CCA auf LO Interrupt
    TCD1.INTCTRLB = TC_CCAINTLVL_LO_gc;
	// TCD1 Timer starten mit /64
    TCD1.CTRLA = TC_CLKSEL_DIV64_gc;
}

/* void dec_stop(void)
 * DCC Decoder stoppen.
 */
static void dec_stop(void) {
	// Status auf OFF
    g_dccdec.state = DEC_STATE_OFF;
	// keine Interrupts
    TCD1.INTCTRLB = 0;
	// Interrupt Flags clearen
    TCD1.INTFLAGS = 0xff;
	// und Timer TCD1 aus
    TCD1.CTRLA = TC_CLKSEL_OFF_gc;
}

/* void dec_parse_packet(void)
 * Ein DCC Packet parsen im User Code.
 */ 
static void dec_parse_packet(void) {
	// wenn mehr als 3 Bits im Buffer sind und der XOR Wert 0 ist
    if (g_dccdec.bufsize >= 3 && g_dccdec.xor == 0) {
		// dann parsen
        do_dec_parse_packet();
    }
}

/*  void dec_halfbit(uint8_t hb)
 *   hb Wert des Half bits (0|1)
 * Ein halbes Bit dem Decoder zuführen.
 */
static void dec_halfbit(uint8_t hb) {
    switch(g_dccdec.state) {
		// sind wir in der preamble?
        case DEC_STATE_PREAMBLE: {
			// Halfbit 1?
            if (hb) {
				// ist Preamble < 100 halfbits
                if (g_dccdec.preamble < 100) {
					// dann Zähler erhöhen
                    g_dccdec.preamble++;
                }
            } else {
				// mind 10 Bits volle Bits für die Preamble
                if (g_dccdec.preamble >= 20) {
					// dann ist es ein Start Bit
                    g_dccdec.state = DEC_STATE_STARTBIT;
                } else {
					// ansonsten zurücksetzen
                    g_dccdec.preamble = 0;
                }
            }
            break;
        }
		// sind wir im Start Bit?
        case DEC_STATE_STARTBIT: {
            if (hb) {
                goto dec_reset;
            }
            g_dccdec.state = DEC_STATE_BIT_H1;
            g_dccdec.bufsize = 0;
            g_dccdec.bits = 0;
            g_dccdec.xor = 0;
            break;
        }
        case DEC_STATE_BIT_H1: {
            g_dccdec.lasthbit = hb;
            if (g_dccdec.bits < 8) {
                g_dccdec.state = DEC_STATE_BIT_H2;
            } else {
                if (g_dccdec.bufsize < sizeof(g_dccdec.buf)) {
                    g_dccdec.buf[g_dccdec.bufsize++] = g_dccdec.bitbuf;
                }
                g_dccdec.xor ^= g_dccdec.bitbuf;
                g_dccdec.state = DEC_STATE_STARTSTOPBIT;
            }
            break;
        }
        case DEC_STATE_BIT_H2: {
            if (hb != g_dccdec.lasthbit) {
                goto dec_reset;
            }
            g_dccdec.bitbuf <<= 1;
            if (hb) {
                g_dccdec.bitbuf |= 0x01;
            }
            g_dccdec.bits++;
            g_dccdec.state = DEC_STATE_BIT_H1;
            break;
        }
        case DEC_STATE_STARTSTOPBIT: {
            if (hb != g_dccdec.lasthbit) {
                goto dec_reset;
            }
            if (hb) { // end of packet bit ?
                dec_parse_packet();
                goto dec_reset;
                
            } else { // stop bit
                g_dccdec.state = DEC_STATE_BIT_H1;
                g_dccdec.bits = 0;
            }
            break;
        }
    }
    return;
    
dec_reset:
    g_dccdec.state = DEC_STATE_PREAMBLE;
    g_dccdec.preamble = 0;
    g_dccdec.bufsize = 0;
    g_dccdec.xor = 0;
}

ISR(TCD1_CCA_vect) {
    if (g_dccdec.state == DEC_STATE_OFF) {
        return;
    }
    TCD1.CTRLFSET = TC_CMD_RESTART_gc;
    if (g_dccdec.state == DEC_STATE_FIRST) {
        g_dccdec.state = DEC_STATE_PREAMBLE;
        g_dccdec.preamble = 0;
        TCD1.INTFLAGS = Bit(TC1_OVFIF_bp);
    } else {
        uint8_t hb = 0;
        if (bit_is_clear(TCD1.INTFLAGS, TC1_OVFIF_bp) && TCD1.CCA < (87/2) ) {
            hb = 1;
        }
        TCD1.INTFLAGS = Bit(TC1_OVFIF_bp);
        dec_halfbit(hb);
    }
}

