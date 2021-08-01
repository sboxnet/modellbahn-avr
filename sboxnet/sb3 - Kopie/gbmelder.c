/***************************************************************************
 *   Copyright (C) 2014-2020
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
 * TCD0
 * - general timer 500 kHz / 2us, normal mode, 16bit
 * - CCC: dcc decoder cutout channel 2 timer
 */

#include "common.h"


#define PRODUCT_ID   0x000b
#define DEVICE_DESC "gbmelder2"
#define VENDOR_ID    0x9999
#define FIRMWARE_VERSION 0x0200

APP_FIRMWARE_HEADER(PRODUCT_ID, VENDOR_ID, FIRMWARE_VERSION)

#define CONCAT(A,B)        A ## B
#define CONCAT3(A,B,C)     A ## B ## C
#define CONCAT4(A,B,C,D)   A ## B ## C ## D
#define CONCAT5(A,B,C,D,E) A ## B ## C ## D ## E
// DCC Sensor Port
#define DCCSENSE_PORT  PORTB
// DCC Sensor Pin im DCCSENSE_PORT
#define DCCSENSE_PIN   0

#define DCCSENSE_CTRL(DCCSENSE_PIN)    CONCAT3(PIN, DCCSENSEPIN, CTRL)


struct sensor {
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

uint8_t hardwaretyp = 0; // alte Hardware


#define DEFAULT_HOLDTIME 200
#define NUM_SENSORS (hardwaretyp == 1 ? 10 : 16)

uint8_t g_holdtime;
uint8_t g_old_holdtime;
uint16_t g_sensor_bits = 0;
uint16_t g_sensor_bits_1 = 0;
struct sensor g_sensors[16];
uint8_t  g_led_counter = 0;

uint8_t  g_transmit_seq = 0;

struct gbmelder_pipe {
    pipe_t pipe;
    uint8_t buf[64];
};

struct gbmelder_pipe g_locoaddr_pipe;

struct timer g_led_timer;
struct timer g_power_on_timer;
uint8_t g_power_on;

#include "dccdec.c"

uint16_t g_dec_lastaddr = 0;

struct Eeprom {
    uint8_t  holdtime;
    struct {
        uint8_t  holdtime;
        uint8_t  reserved[15];
    }    sensors[16];
};
struct Eeprom g_eeprom EEMEM;


static void do_dec_parse_packet(void) {
    if (!g_power_on || !timer_timedout(&g_power_on_timer)) {
        return;
    }
    
    if (g_dccdec.buf[0] != 0 // broadcast or reset
           && g_dccdec.buf[0] != 0xff) { // idle
        
        g_dec_lastaddr = 0;
        if (g_dccdec.buf[0] <= 0x7f) {
            g_dec_lastaddr = g_dccdec.buf[0];
        } else if (g_dccdec.buf[0] >= 192 && g_dccdec.buf[0] <= 231) {
            g_dec_lastaddr = ((uint16_t)g_dccdec.buf[0] << 8) | g_dccdec.buf[1];
        }
        if (g_dec_lastaddr) {
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                TCD0.CCC = TCD0.CNT + (290/2);
            }
            TCD0.INTFLAGS = Bit(TC0_CCCIF_bp);
            TCD0.INTCTRLB = (TCD0.INTCTRLB & ~TC0_CCCINTLVL_gm)|TC_CCCINTLVL_LO_gc;
    
            port_clrbit(PORTB, 2);
        }
    }
}

ISR(TCD0_CCC_vect) {
    TCD0.INTCTRLB = (TCD0.INTCTRLB & ~TC0_CCCINTLVL_gm)|TC_CCCINTLVL_OFF_gc;
    
    //port_setbit(PORTB, 2);
    uint8_t sens1 = ~port_in(PORTA);
    uint8_t sens2 = ~port_in(PORTC);
    // sensoren lesen
    uint16_t sens = ((uint16_t)sens2 << 8 | sens1) & g_sensor_bits;
    if (pipe_getfree(&g_locoaddr_pipe.pipe) >= 4) {
        pipe_write(&g_locoaddr_pipe.pipe, lowbyte(g_dec_lastaddr));
        pipe_write(&g_locoaddr_pipe.pipe, highbyte(g_dec_lastaddr));
        pipe_write(&g_locoaddr_pipe.pipe, lowbyte(sens));
        pipe_write(&g_locoaddr_pipe.pipe, highbyte(sens));
    }
    g_dec_lastaddr = 0;
}


static void read_sensors(void) {
    uint8_t sens1 = ~port_in(PORTA);
    uint8_t sens2 = ~port_in(PORTC);
    
    uint16_t sensors = (uint16_t)sens2 << 8 | sens1;
    
    debounce_16(&g_sensor_bits, &g_sensor_bits_1, sensors);
    
    volatile uint16_t mask = 0x01;
    for (volatile uint8_t i = 0; i < NUM_SENSORS; i++, mask <<= 1) {
        if (sensors & mask) {
            if (!g_sensors[i].flags.on) {
                g_sensors[i].flags.notack = 1;
                g_sensors[i].flags.on = 1;
                g_sensors[i].retry_timer = 0;
                g_sensors[i].locoaddr = 0;
            }
            g_sensors[i].timer = g_sensors[i].holdtime;
            g_sensor_bits |= mask;
        } else if (g_sensors[i].timer == 0) {
            if (g_sensors[i].flags.on) {
                g_sensors[i].flags.notack = 1;
                g_sensors[i].flags.on = 0;
                g_sensors[i].retry_timer = 0;                
            }
            g_sensors[i].locoaddr = 0;
            g_sensor_bits &= ~mask;
        }
        if (g_sensors[i].timer)
            g_sensors[i].timer--;
        if (g_sensors[i].retry_timer)
            g_sensors[i].retry_timer--;
    }
    
    if (sensors) {
        if (!g_power_on) {
            timer_set(&g_power_on_timer, 30);
            g_power_on = 1;
        }
    } else if (g_power_on) {
        g_power_on = 0;
    }
}

// für neue Hardware!
void show_besetzt_leds(void) {
	// PC6 (Bit 0) und PC7 (Bit 1)
	// PD0..PD7(Bit 2 .. 9)
	// zuerst mal alles aus
	port_clr(PORTC, Bit(6)|Bit(7));
	port_out(PORTD) = 0;
	uint16_t inp = g_sensor_bits;
	for (uint8_t i = 0; i < NUM_SENSORS; i++) {
		// if sensor is on -> led ON
		uint16_t mask = 1 << i;
		if (inp & mask) {
			if (i == 0) {
				port_set(PORTC, Bit(6));
			} else if (i == 1 ) {
				port_set(PORTC, Bit(7));
			} else if (i == 2) {
				port_set(PORTD, Bit(0));
			} else if (i == 3) {
				port_set(PORTD, Bit(1));
			} else if (i == 4) {
				port_set(PORTD, Bit(2));
			} else if (i == 5) {
				port_set(PORTD, Bit(3));
			} else if (i == 6) {
				port_set(PORTD, Bit(4));
			} else if (i == 7) {
				port_set(PORTD, Bit(5));
			} else if (i == 8) {
				port_set(PORTD, Bit(6));
			} else if (i == 9) {
				port_set(PORTD, Bit(7));
			}
		}
	}
}

// only for old hardware
void multiplex_leds(void) {
    uint8_t ledrow = (g_led_counter++) & 0x03;
    uint16_t inp = g_sensor_bits;
    uint8_t b;
    switch (ledrow) {
        case 0: ledrow = ~Bit(4) & 0xf0; b = inp; break;
        case 1: ledrow = ~Bit(5) & 0xf0; b = (inp >> 4); break;
        case 2: ledrow = ~Bit(6) & 0xf0; b = (inp >> 8); break;
        default: ledrow = ~Bit(7) & 0xf0; b = (inp >> 12); break;
    }
    // all leds off
    port_out(PORTD) = ledrow | (b & 0x0f);
}

uint8_t get_next_transmit_seq(void) {
    uint8_t seq = g_transmit_seq + 1;
    if (seq < 10) {
        seq = 10;
    }
    
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        if (g_sensors[i].flags.notack && seq == g_sensors[i].last_seq) {
            seq++;
            i = 0; // retry
        }   
    }

    g_transmit_seq = seq;
    return seq;
}

void do_init_system(void) {
	// Erst Hardware prüfen
	// PORTB auf Input: PB0..PB3
	port_dirin(PORTB, 0x0f);
	// und Eingabe überprüfen
	uint8_t pbin = port_in(PORTB);
	// ist PB0 und PB2 auf Low, PB1 und PB3 auf High ==> neue Hardware
	if (bit_is_set(pbin, 2) && bit_is_clear(pbin, 1) && bit_is_clear(pbin, 3)) {
		// neue Hardware
		hardwaretyp = 1;
	} else {
		// alte Hardware
		hardwaretyp = 0;
	}
	switch(hardwaretyp) {
		case 1: {
            // Port für LEDs: im Gegensatz zur alten Hardware wird hier nicht multiplexed, sondern direkt LEDs angesteuert
            // PD0..PD7(Bit 2 .. 9)
			port_out(PORTD) = 0; // PORTD = 0
			port_dirout(PORTD, 0xff); // PORTD as output
			PORTCFG_MPCMASK = 0xff; // Alle Pins
			PORTD.PIN0CTRL = PORT_OPC_TOTEM_gc; // Alle PORTD Pins auf TOTEM Pole
			// PC6 (Bit 0) und PC7 (Bit 1)
			port_dirout(PORTC, Bit(6) | Bit(7)); // PC6 and PC7 as output
            PORTCFG_MPCMASK = 0xc0;
            PORTC.PIN0CTRL = PORT_OPC_TOTEM_gc;

            // PORTA als Eingabe (PA0..PA7) (Bit 0 .. 7) mit Pull Up's
            port_dirin(PORTA, 0xff);
			// pull up on
			PORTCFG_MPCMASK = 0xff; 
			port_pullup_on(PORTA, 0xff);
            // PC0 (Bit 8), PC1 (Bit 9) als Eingabe mit Pull Up's
            port_dirin(PORTC, 0x3);
			// pull up on
			PORTCFG_MPCMASK = 0x03;
			port_pullup_on(PORTC, 0x03);

			break;
		}

        default:
		// alter Hardware Typ
		{
            // Multiplex BesetztLEDs

            // PORTD 0
			port_out(PORTD) = 0;
            // PORTD als Ausgabe
			port_dirout(PORTD, 0xff);
            // alle Pins Totem Pole
			PORTCFG_MPCMASK = 0xff; // all pins
			PORTD.PIN0CTRL = PORT_OPC_TOTEM_gc;

            // PORTC als Sensor 8-15, Eingabe
			port_dirin(PORTC, 0xff);
            // alle Pins mit Pull Up
			PORTCFG_MPCMASK = 0xff; // all pins
			PORTC.PIN0CTRL = PORT_OPC_PULLUP_gc;

            // PORTB als Eingabe
            port_dirin(PORTB, 0xff);
            // alle Pins
            PORTCFG_MPCMASK = 0xff; // all pins
            // PORTB als PullDown
            PORTB.PIN0CTRL = PORT_OPC_PULLDOWN_gc;

            //port_dirout(PORTB, Bit(2));

            //PORTA als Sensor 0-7, Eingabe
            port_dirin(PORTA, 0xff);
            // alle Pins
            PORTCFG_MPCMASK = 0xff; // all pins
            // Pull Up
            PORTA.PIN0CTRL = PORT_OPC_PULLUP_gc;

			break;
		}
	}
    
    // Konfigure Sleep Modus: idle sleep mode, sleep mode allowed
    SLEEP.CTRL = SLEEP_SMODE_IDLE_gc|Bit(SLEEP_SEN_bp);
    // power reduction
    /*
    PR.PRPA = Bit(PR_DAC_bp)|Bit(PR_ADC_bp)|Bit(PR_AC_bp);
    PR.PRPB = Bit(PR_DAC_bp)|Bit(PR_ADC_bp)|Bit(PR_AC_bp);
    PR.PRPC = Bit(PR_TWI_bp)|Bit(PR_USART1_bp)|Bit(PR_USART0_bp)|Bit(PR_SPI_bp)|Bit(PR_HIRES_bp); // PR_TC1_bp PR_TC0_bp;
    PR.PRPD = Bit(PR_TWI_bp)|Bit(PR_USART1_bp)|Bit(PR_USART0_bp)|Bit(PR_SPI_bp)|Bit(PR_HIRES_bp); // PR_TC1_bp PR_TC0_bp;
    */
    // Produkt ID, Vendor ID, Firmware Version setzen
    g_com.productid = PRODUCT_ID;
    g_com.vendorid = VENDOR_ID;
    g_com.firmware_version = FIRMWARE_VERSION;
    // Fähigkeiten des Geräts
    g_com.capabilities = CAP_FB_OCCUPANCY; // Besetztmelder
    g_com.cap_class = 0;
    g_com.dev_desc_P = PSTR(DEVICE_DESC); // Beschreibung
    
    // 1ms Timer setzen
    timer_register(&g_led_timer, TIMER_RESOLUTION_1MS);
    
    // Lokadresse Pipeline einrichten
    pipe_init(&g_locoaddr_pipe.pipe, sizeof(g_locoaddr_pipe.buf));
    
    // Sensoren Init mit 0 == Nichts belegt
    memset(g_sensors, 0, sizeof(g_sensors));
    
    // PB0  DCC Signal Pin

    // PB0 PullDown, beide Flanken
    DCCSENSE_PORT.PIN0CTRL = PORT_OPC_PULLDOWN_gc|PORT_ISC_BOTHEDGES_gc;
    //PORTB.PIN0CTRL = PORT_OPC_PULLDOWN_gc|PORT_ISC_BOTHEDGES_gc;
    // Decoder Init PortB0
    //dec_init(CONCAT5(EVSYS_CHMUX_,DCCSENSE_PORT,_PIN,DCCSENSE_PIN, _gc));
	dec_init(EVSYS_CHMUX_PORTB_PIN0_gc);
    
    timer_register(&g_power_on_timer, TIMER_RESOLUTION_16MS);
    g_power_on = 0;
    
    g_holdtime = e2prom_get_byte(&g_eeprom.holdtime);
    if (g_holdtime == 0xff) {
        g_holdtime = DEFAULT_HOLDTIME;
    }
    g_old_holdtime = g_holdtime;
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        uint8_t ht = e2prom_get_byte(&g_eeprom.sensors[i].holdtime);
        if (ht == 0xff) {
            ht = g_holdtime;
        }
        g_sensors[i].holdtime = ht;
    }
}

uint8_t do_msg(struct sboxnet_msg_header *pmsg) {
    if (pmsg->dstaddr == SBOXNET_ADDR_BROADCAST) {
        return SBOXNET_ACKRC_CMD_UNKNOWN;
    }
    switch (pmsg->cmd) {
        case SBOXNET_CMD_FB_LOCOADDR|0x80: {
            if (pmsg->opt.len != 0) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            for (uint8_t i = 0; i < NUM_SENSORS; i++) {
                if (g_sensors[i].flags.notack && pmsg->seq == g_sensors[i].last_seq) {
                    g_sensors[i].flags.notack = 0;
                    g_sensors[i].last_seq = 0;
                }
            }
            return SBOXNET_ACKRC_SEND_NO_ANSWER;
        }
    }
    return SBOXNET_ACKRC_CMD_UNKNOWN;
}

uint8_t do_reg_read(uint16_t reg, uint16_t* pdata) {
    switch(reg) {
        case R_FB_NUM:      *pdata = NUM_SENSORS; return 0;
        case R_FB_VALUE0:   *pdata = g_sensor_bits; return 0;
        
        case R_GBM_HOLDTIME: *pdata = g_holdtime; return 0;
    }    
    if (reg >= R_GBM_PAR_0 && reg < R_GBM_PAR_15+NUM_GBM_PARS) {
        uint8_t roffs = reg & NUM_GBM_PARS_MASK;
        uint8_t gbmnum = (reg-R_GBM_PAR_0) / NUM_GBM_PARS;
        switch (roffs) {
            case ROFFS_GBM_HOLDTIME: {
                *pdata = g_sensors[gbmnum].holdtime;
                return 0;
            }
            case ROFFS_GBM_LOCOADDR: {
                *pdata = g_sensors[gbmnum].locoaddr;
                return 0;
            }
            default: {
                break;
            }
        }
    }
    return SBOXNET_ACKRC_REG_INVALID;
};

uint8_t do_reg_write(uint16_t reg, uint16_t data, uint16_t mask) {
    switch (reg) {
        case R_GBM_HOLDTIME: {
            g_old_holdtime = g_holdtime;
            g_holdtime = (data >= 0xff ? 0xff : (uint8_t)data);
            return 0;
        }
    }
    if (reg >= R_GBM_PAR_0 && reg < R_GBM_PAR_15+NUM_GBM_PARS) {
        uint8_t roffs = reg & NUM_GBM_PARS_MASK;
        uint8_t gbmnum = (reg-R_GBM_PAR_0) / NUM_GBM_PARS;
        switch (roffs) {
            case ROFFS_GBM_HOLDTIME: {
                g_sensors[gbmnum].holdtime = (data >= 0xff ? 0xff : (uint8_t)data);
                g_sensors[gbmnum].flags.holdtime_changed = 1;
                return 0;
            }
            default: {
                break;
            }
        }
    }
    return SBOXNET_ACKRC_REG_INVALID;
}

void do_setup(void) {
    dec_start();
}

void do_main(void) {    
    if (timer_timedout(&g_led_timer)) {
        timer_set(&g_led_timer, 5); // 5ms

        read_sensors();
        
		if (hardwaretyp == 1) {
			// neue Hardware
			show_besetzt_leds();
		} else {
			//alte hardware
			multiplex_leds();
		}
    }
        
    uint8_t canread = 0;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        canread = pipe_count(&g_locoaddr_pipe.pipe);
    }
    if (canread >= 4) {
        uint16_t sens = 0;
        uint16_t lastaddr = 0;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { lastaddr = pipe_read(&g_locoaddr_pipe.pipe); }
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { lastaddr |= ((uint16_t)pipe_read(&g_locoaddr_pipe.pipe) << 8); }
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { sens = pipe_read(&g_locoaddr_pipe.pipe); }
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { sens |= ((uint16_t)pipe_read(&g_locoaddr_pipe.pipe) << 8); }
        
        uint16_t mask = 0x01;
        for (uint8_t i = 0; i < 16; i++, mask <<= 1) {
            if (sens & mask) {
                if (g_sensors[i].flags.on && g_sensors[i].locoaddr != lastaddr) {
                    g_sensors[i].locoaddr = lastaddr;
                    g_sensors[i].flags.notack = 1;
                    g_sensors[i].retry_timer = 0;
                }
            }
        }
    }
    
    if (bit_is_clear(g_dev_state, DEV_STATE_FLG_WATCHDOG_b)) {
        for (uint8_t i = 0; i < NUM_SENSORS; i++) {
            if (g_sensors[i].retry_timer == 0 && g_sensors[i].flags.notack) {
                g_sensors[i].retry_timer = 50;
                
                struct sboxnet_msg_max msg;
                msg.msgh.dstaddr = 0;
                msg.msgh.seq = g_sensors[i].last_seq = get_next_transmit_seq();
                msg.msgh.opt.len = 3;
                msg.msgh.cmd = SBOXNET_CMD_FB_LOCOADDR;
                if (g_sensors[i].flags.on) {
                    msg.data[0] = i|0x80;
                } else {
                    msg.data[0] = i;
                }
                msg.data[1] = lowbyte(g_sensors[i].locoaddr);
                msg.data[2] = highbyte(g_sensors[i].locoaddr);
                
                sboxnet_send_msg(&msg.msgh);
                break; // only send one per do_main() loop
            }
        }
    }
    
    // write eeprom
    if (g_holdtime != g_old_holdtime && eeprom_is_ready()) {
        eeprom_update_byte(&g_eeprom.holdtime, g_holdtime);
        g_old_holdtime = g_holdtime;
    }
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        if (g_sensors[i].flags.holdtime_changed && eeprom_is_ready()) {
            eeprom_update_byte(&g_eeprom.sensors[i].holdtime, g_sensors[i].holdtime);
            g_sensors[i].flags.holdtime_changed = 0;
        }
    }
    
    sleep_cpu();
}

void do_before_bldr_activate(void) {
    dec_stop();
}


