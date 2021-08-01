/***************************************************************************
 *   Copyright (C) 2012-2013
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


#include <util/atomic.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "avrutilslib/defines.h"
#include "avrutilslib/logger-usart.h"
#include "avrutilslib/ringbuffer.h"

#undef __SELECT_USART
#define __SELECT_USART  LOGGER_USART_ID
#include "avrutilslib/select-usart.h"


static struct logger_buf {
    struct ring_buffer rb;
    uint8_t buf[LOGGER_USART_BUF_SIZE];
} g_logger_usart_buf;


void logger_usart_init(void) {
    ringbuf_init(&g_logger_usart_buf.rb, sizeof(g_logger_usart_buf.buf));

    __UBRRH = highbyte(LOGGER_USART_BAUD_UBRR) & 0x7f;
    __UBRRL = lowbyte(LOGGER_USART_BAUD_UBRR);
    __UCSRA = Bsv(__TXC,1)|Bsv(__U2X,0)|Bsv(__MPCM,0); // clear TXC, normal speed and disable Multiprocessor Mode
    __UCSRC = 
#ifdef __URSEL
              Bsv(__URSEL,1)|  // if __URSEL is defined, __UCSRC and __UBRRH share the same io location ;(
#endif
              Bsv(__UMSEL0,0)|Bsv(__UPM1,0)|Bsv(__UPM0,0)|Bsv(__USBS,0)|Bsv(__UCSZ1,1)|Bsv(__UCSZ0,1); // async, no parity, 1 stop bit, 8 chars
#ifdef __UCSRD
    __UCSRD = 0;
#endif
    __UCSRB = Bsv(__RXCIE,0)|Bsv(__TXCIE,0)|Bsv(__UDRIE,0)|Bsv(__RXEN,0)|Bsv(__TXEN,1)|Bsv(__UCSZ2,0)|Bsv(__TXB8,0); // 8 chars, transmitter enabled
}

static inline void logger_usart_start(void) {
    setbit(__UCSRB, __UDRIE); // enable UDR Empty interrupt
}

static inline void logger_usart_interrupt(void) {
    if (ringbuf_isempty(&g_logger_usart_buf.rb)) {
        clrbit(__UCSRB, __UDRIE); // disable UDR Empty interrupt
    } else {
        __UDR = ringbuf_read(&g_logger_usart_buf.rb);
    }
}

ISR(__USART_UDRE_vect) {
    logger_usart_interrupt();
}

static void logger_usart_write(uint8_t a_v) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (bit_is_set(__UCSRA, __UDRE) && !ringbuf_isempty(&g_logger_usart_buf.rb)) {
            __UDR = ringbuf_read(&g_logger_usart_buf.rb);
        }
        
        if (ringbuf_isfull(&g_logger_usart_buf.rb)) {
            // sorry, no space left...
        } else {
            if (ringbuf_getfree(&g_logger_usart_buf.rb) == 1) {
                a_v = '$';
            }
            ringbuf_write(&g_logger_usart_buf.rb, a_v);
            if (bit_is_set(__UCSRA, __UDRE)) {
                __UDR = ringbuf_read(&g_logger_usart_buf.rb);
            }
        }
    }
    logger_usart_start();
}

void logger_usart_check(void) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (bit_is_set(__UCSRA, __UDRE) && !ringbuf_isempty(&g_logger_usart_buf.rb)) {
            __UDR = ringbuf_read(&g_logger_usart_buf.rb);
        }
    }
}

void logger_usart_str_P(const char* a_str) {
    char c;
    while ((c = pgm_read_byte(a_str)) != 0) {
        logger_usart_write(c);
        a_str++;
    }
}

void logger_usart_hex_byte(uint8_t a_v) {
    static const char hexstring[] PROGMEM = "0123456789abcdef";
    logger_usart_write(pgm_read_byte(&hexstring[(a_v >> 4) & 0x0f]));
    logger_usart_write(pgm_read_byte(&hexstring[a_v & 0x0f]));
}

void logger_usart_hex_word(uint16_t a_v) {
    logger_usart_hex_byte(a_v >> 8);
    logger_usart_hex_byte(a_v);
}


#undef  __SELECT_USART
#include "avrutilslib/select-usart.h"
