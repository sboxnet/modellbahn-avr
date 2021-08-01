/***************************************************************************
 *   Copyright (C) 2013
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

#define F_CPU_MHZ 16
#define F_CPU  (F_CPU_MHZ * 1000000)


#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/cpufunc.h>
#include <avr/eeprom.h>
#include <util/delay.h>


#define TIMER_PRESCALER     64
#define TIMER_FREQ_HZ       100
#define TIMER_TOP  (F_CPU / (TIMER_PRESCALER * TIMER_FREQ_HZ) - 1)  // 2500-1


#define DEVICE_ID      0x00010003
#define DEVICE_VERSION 0x0001
#define DEVICE_DESC    "RelayCard"

#include "common.c"

#define LED_PORT  PORTD
#define LED_BIT   PD2

static uint8_t g_timer = 0;
static uint16_t g_wait_timer = 0;


struct Eeprom {
    uint8_t devaddr;
};
struct Eeprom eeprom EEMEM;



static NOINLINE void init_system(void) {

    PORTB = 0b11000000;
    DDRB  = 0b00111111; // PB0..5 as output
    PORTC = 0b11000000;
    DDRC  = 0b00111111; // PC0..5 as output
    PORTD = 0b00001111;
    DDRD  = 0b11110100; // PD2 (LED), PD4..7 as output
    
    // Analog comparator control and status: disable analog comparator
    ACSR = Bit(ACD);
    
    // configure sleep mode: idle sleep mode, sleep mode allowed
    SMCR = Bsv(SM2,0)|Bsv(SM1,0)|Bsv(SM0,0)|Bsv(SE,1);
    
    OCR1A = TIMER_TOP;
    TCCR1A = Bsv(WGM11,0)|Bsv(WGM10,0);      // CTC with OCR1A
    TCCR1B = Bsv(WGM13,0)|Bsv(WGM12,1)|Bsv(CS12,0)|Bsv(CS11,1)|Bsv(CS10,1); // start timer with prescaler 1/64
    
    // timer for ux
    TCCR2A = Bsv(WGM21,0)|Bsv(WGM20,0);  // normal mode
    TCCR2B = Bsv(WGM22,0)|TC2_PRESCALER; // start timer with prescaler
    TIMSK2 = 0;
    
    TIFR1 = 0xff;
    TIMSK1 = Bsv(OCIE1A,1);
    
    g_dev_addr = e2prom_get_byte(0); // get UXNET address from EEPROM address 0
}

BLDR_APP_SECTION
void bldr_main_init_system(void) {
    PORTB = 0b11000000;
    DDRB  = 0b00000000;
    PORTC = 0b11000000;
    DDRC  = 0b00000000;

    PORTD = 0b00001011; // LED on
    DDRD  = 0b00000100; // PD2 (LED) as output

    // Analog comparator control and status: disable analog comparator
    ACSR = Bit(ACD);

    // timer for ux
    TCCR2A = Bsv(WGM21,0)|Bsv(WGM20,0);  // normal mode
    TCCR2B = Bsv(WGM22,0)|TC2_PRESCALER; // start timer with prescaler
    TIMSK2 = 0;

    TIMSK1 = 0;
}

static NOINLINE void wait_10ms(uint16_t mult) {
    cli();
    g_wait_timer = mult;
    sei();
    while (*(volatile uint16_t*)&g_wait_timer) {
        sleep_cpu();
    }
}

// every 10ms
ISR(TIMER1_COMPA_vect) {
    g_timer++;
    
    if (g_wait_timer) {
        g_wait_timer--;
    }
}



static void process_msg(struct ux_msg_max *pmsg) {
    uint8_t rc = UX_ACKRC_OK;
    uint8_t len = 0;

    if (pmsg->msg.cmd & 0x80) {
        return;  // discard Ack Answers
    }
    
    switch (pmsg->msg.cmd) {
        
        case UX_CMD_FUNC_GET_BITS: {
            if (pmsg->msg.opt.len != 1) {
                rc = UX_ACKRC_INVALID_ARG;
                goto return_error;
            }
            pmsg->data[0] = (PORTB & 0x3f) | (PORTD & 0xc0);
            pmsg->data[1] = (PORTC & 0x3f) | ((PORTD & 0x30) << 2);
            len = 2;
            break;
        }
        
        case UX_CMD_FUNC_SET_BITS: {
            if (pmsg->msg.opt.len != 4) {
                rc = UX_ACKRC_INVALID_ARG;
                goto return_error;
            }
            uint8_t mask0 = pmsg->data[0];
            uint8_t mask1 = pmsg->data[1];
            uint8_t data0 = pmsg->data[2] & mask0;
            uint8_t data1 = pmsg->data[3] & mask1;
            
            PORTB = (PORTB & ~(0x3f & mask0)) | (0x3f & data0);
            PORTC = (PORTC & ~(0x3f & mask1)) | (0x3f & data1);
            PORTD = (PORTD & ~((0xc0 & mask0)|(0x30 & (mask1 >> 2))))
                    | (0xc0 & data0) | (0x30 & (data1 >> 2));
            break;
        }
        
        case UX_CMD_FUNC_SET_BITNR: {
            if (pmsg->msg.opt.len != 1) {
                rc = UX_ACKRC_INVALID_ARG;
                goto return_error;
            }
            uint8_t d = pmsg->data[0];
            uint8_t n = d & 0x3f;
            uint8_t dotoggle = d & 0x80;
            uint8_t doset    = d & 0x40;
            uint8_t m;
            if (n < 6) {
                m = bitmask(n); // PB0..5
                if (dotoggle) { // toggle
                    PORTB ^= m;
                } else if (doset) {
                    PORTB |= m;
                } else {
                    PORTB &= ~m;
                }
            } else if (n < 8) {
                m = bitmask(n); // PD6..7
                goto do_portd;
            } else if (n < 14) {
                m = bitmask(n - 8); // PC0..5
                if (dotoggle) { // toggle
                    PORTC ^= m;
                } else if (doset) {
                    PORTC |= m;
                } else {
                    PORTC &= ~m;
                }
            } else if (n < 16) {
                m = bitmask(n - 10); // PD4..5
           do_portd:
                if (dotoggle) {
                    PORTD ^= m;
                } else if (doset) {
                    PORTD |= m;
                } else {
                    PORTD &= ~m;
                }
            }    
            break;
        }
        
        default: {
            uint8_t r = process_std_msg(pmsg);
            if (!(r & PROCMSG_FLG_ERROR)) {
                len = r;
                break;
            }
            rc = r & 0x7f;
    return_error:
            len = 2;
            pmsg->data[0] = pmsg->msg.cmd;
            pmsg->data[1] = rc;
            pmsg->msg.cmd = 0x80;
            break;
        }        
    }
    // send anwser
    pmsg->msg.dstaddr = pmsg->msg.srcaddr;
    pmsg->msg.opt.len = len;
    pmsg->msg.cmd |= 0x80;
    ux_send_msg(pmsg);
}


static void receive_msg(void) {
    struct ux_msg_max msg;
    
    switch (ux_receive_msg(&msg)) {
        case 0: {
            process_msg(&msg);
            break;
        }
        
        case 1: break; // no message
        
        case 2: {
            g_dev_errflags |= DEV_ERR_FLG_UXCRC; // crc error
            break;
        }
    }
}

static void show_status(void) {
	uint8_t errflags = g_dev_errflags;
	if (errflags) {
		clrbit(LED_PORT, LED_BIT); // Status LED on
	} else {
		if (g_timer & 0x20) {
            setbit(LED_PORT, LED_BIT); // off
        } else {
            clrbit(LED_PORT, LED_BIT); // on
        }
	}
}


__ATTR_OS_MAIN int main(void) {
    cli();
    init_system();

    ux_init();
    ux_receiver_enable();
    
    sei();

    wait_10ms(10);
    
    while(1) {
        sleep_cpu(); // at least every 10ms the cpu is waked up
        
        receive_msg();
        show_status();
        
        if (g_dev_state & DEV_STATE_FLG_FWUP_FLASH_ON) {
            // start firmware update
            cli();
            clrbit(LED_PORT, LED_BIT); // Status LED on
            TIMSK1 = 0;
            TCCR1B = 0; // stop timer 1
            PORTB &= ~0x3f; // relais off
            PORTC &= ~0x3f;
            PORTD &= ~0xf0;

            jump_to_bootloader();
            // never reached
        }
    }

    return 0;
}

