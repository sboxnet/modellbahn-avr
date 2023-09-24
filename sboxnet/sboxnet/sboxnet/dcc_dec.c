/*
 * CFile1.c
 *
 * Created: 05.09.2023 16:27:41
 *  Author: balagi
 */ 

#include "common.h"

/* event system
   src: EVSYS_CHMUX_PORTC_PIN4_gc --> dst: TCC1.CTRLD = TC_EVACT_CAPT_gc|TC_EVSEL_CH0_gc;
   when tcc1 is started from 0,
   CCA contains number of ticks till capture is made at rise or fall of pc4=dcc-input
   TCC1: 32.000.000/64 = 500.000 = 2us
   
*/


struct dcc_dec_t *g_dccdec = 0;

void dcc_dec_init(struct dcc_dec_t* dccdec) { // e.g.: EVSYS_CHMUX_PORTC_PIN4_gc
    dccdec->state = DCC_DEC_STATE_OFF;
    dccdec->preamble = 0;
    dccdec->bufsize = 0;
    dccdec->bits = 0;
    dccdec->lasthbit = 0;
    dccdec->xor = 0;
    dccdec->cutout = 0;
    
    EVSYS.CH0MUX = EVSYS_CHMUX_PORTC_PIN4_gc; // event source multiplexer: src PORTC PIN4 DCC Input --> dest TCC1.CCA
    EVSYS.CH0CTRL = 0;
	
	g_dccdec = dccdec;
}

void dcc_dec_start(void) {
    g_dccdec->state = DCC_DEC_STATE_FIRST;
    g_dccdec->preamble = 0;
    g_dccdec->bufsize = 0;
    g_dccdec->cutout = 0;
    
	TCC1.CCA = 87/2; // dcc high 52..64us: dcc low 90us..10ms: is CCA lower then 87/2 --> DCC H
	 
    TCC1.CTRLFSET = TC_CMD_RESTART_gc;		// start timer from begin
    TCC1.INTFLAGS = 0xff;
    TCC1.INTCTRLB = TC_CCAINTLVL_LO_gc;		// Interrupt Level low
    TCC1.CTRLA = TC_CLKSEL_DIV64_gc;		// start timer with /64 = 32Mhz / 64 = 500kHz = 2us Step
}

void dcc_dec_stop(void) {
    g_dccdec->state = DCC_DEC_STATE_OFF;
    TCC1.INTCTRLB = 0;
    TCC1.INTFLAGS = 0xff;
    TCC1.CTRLA = TC_CLKSEL_OFF_gc;
	g_dccdec = 0;
}

void dcc_dec_parse_packet(struct dcc_dec_t* dccdec) {
    if (dccdec->bufsize >= 3 && dccdec->xor == 0) {
//port_tglbit(PORTC, 6);
        switch(g_v.module) {
			case MODULE_BOOSTER:
			{
				if (g_dccdec->buf[0] != 0 // broadcast or reset
					&& g_dccdec->buf[0] != 0xff) { // idle
						// produce cutout with TCD0.CCC
						ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
							TCD0.CCC = TCD0.CNT + (28/2);
						}
						TCD0.INTFLAGS = Bit(TC0_CCCIF_bp);
						TCD0.INTCTRLB = (TCD0.INTCTRLB & ~TC0_CCCINTLVL_gm)|TC_CCCINTLVL_LO_gc;
						g_dccdec->cutout = 1;
						port_setbit(bo_DCCM_PORT, bo_DCCM_CUTOUT_b); // cutout test point
				}
				break;
			}
			
			case MODULE_GBM:
			{
				if (!gm_v.g_power_on || !timer_timedout(&gm_v.g_power_on_timer)) {
					return;
				}
				    
				if (g_dccdec->buf[0] != 0 // broadcast or reset
				    && (g_dccdec->buf[0] != 0xff)) { // idle
					    
					    gm_v.g_dec_lastaddr = 0;
					    if (g_dccdec->buf[0] <= 0x7f) {
						    gm_v.g_dec_lastaddr = g_dccdec->buf[0];
						} else if (g_dccdec->buf[0] >= 192 && g_dccdec->buf[0] <= 231) {
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
				break;
			}
			default: return;

		}
    }
}

void dcc_dec_halfbit(struct dcc_dec_t* dccdec, uint8_t hb) {
	// hb is the half bit
	
    switch(g_dccdec->state) {
		// if in preamble?
        case DCC_DEC_STATE_PREAMBLE: {
            if (hb) {
				// hb is 1, then wait till there where up to 100 hb==1
                if (g_dccdec->preamble < 100) {
                    g_dccdec->preamble++;
                }
            } else {
				// hb == 0
                if (g_dccdec->preamble >= 20) {
					// greater then 20 preamble half bits? then it is a DCC startbit
                    g_dccdec->state = DCC_DEC_STATE_STARTBIT;
                } else {
					// else begin again
                    g_dccdec->preamble = 0;
                }
            }
            break;
        }
		// start bit
        case DCC_DEC_STATE_STARTBIT: {
            if (hb) {
				// is half bit is 1, then dec reset
				// first bit must be a 0
                goto dcc_dec_reset;
            }
			// first bit is a 0, start
            g_dccdec->state = DCC_DEC_STATE_BIT_H1;  // first half bit after start bit
            g_dccdec->bufsize = 0;	// bits in buffer
            g_dccdec->bits = 0;		// number of bits in buffer
            g_dccdec->xor = 0;		// xor state
            break;
        }
        case DCC_DEC_STATE_BIT_H1: {
            g_dccdec->lasthbit = hb;
            if (g_dccdec->bits < 8) {
                g_dccdec->state = DCC_DEC_STATE_BIT_H2;
            } else {
                if (g_dccdec->bufsize < sizeof(g_dccdec->buf)) {
                    g_dccdec->buf[g_dccdec->bufsize++] = g_dccdec->bitbuf;
                }
                g_dccdec->xor ^= g_dccdec->bitbuf;
                g_dccdec->state = DCC_DEC_STATE_STARTSTOPBIT;
            }
            break;
        }
        case DCC_DEC_STATE_BIT_H2: {
            if (hb != g_dccdec->lasthbit) {
                goto dcc_dec_reset;
            }
            g_dccdec->bitbuf <<= 1;
            if (hb) {
                g_dccdec->bitbuf |= 0x01;
            }
            g_dccdec->bits++;
            g_dccdec->state = DCC_DEC_STATE_BIT_H1;
            break;
        }
        case DCC_DEC_STATE_STARTSTOPBIT: {
            if (hb != g_dccdec->lasthbit) {
                goto dcc_dec_reset;
            }
            if (hb) { // end of packet bit ?
                dcc_dec_parse_packet(g_dccdec);
                goto dcc_dec_reset;
                
            } else { // stop bit
                g_dccdec->state = DCC_DEC_STATE_BIT_H1;
                g_dccdec->bits = 0;
            }
            break;
        }
    }
    return;
   
   dcc_dec_reset:
    g_dccdec->state = DCC_DEC_STATE_PREAMBLE;
    g_dccdec->preamble = 0;
    g_dccdec->bufsize = 0;
    g_dccdec->xor = 0;
}

// ISR ......................

ISR(TCC1_CCA_vect) { // DCC DecoderS
	switch (g_v.module)
	{
		case MODULE_BOOSTER: break;
		case MODULE_GBM:     break;
		default: return;
	}
	// if state is OFF, do nothing
	if (g_dccdec->state == DCC_DEC_STATE_OFF) {
		return;
	}
	// else restart TCC1
	TCC1.CTRLFSET = TC_CMD_RESTART_gc;
			
	if (g_dccdec->state == DCC_DEC_STATE_FIRST) {
		// if first bit? then preamble begin

		g_dccdec->state = DCC_DEC_STATE_PREAMBLE;
		// number of bits in preamble
		g_dccdec->preamble = 0;
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
		dcc_dec_halfbit(g_dccdec, hb);
	}
}
