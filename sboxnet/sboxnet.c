/***************************************************************************
 *   Copyright (C) 2012-2015
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


#include <string.h>

#include <util/crc16.h>
#include <util/atomic.h>
#include <stddef.h>

#include "avrutilslib/defines.h"
#include "avrutilslib/random.h"

#include "sboxnet/sboxnet.h"

#ifndef __AVR_XMEGA__
#undef __SELECT_USART
#define __SELECT_USART  SBOXNET_USART_ID
#include "avrutilslib/select-usart.h"
#endif

#ifndef __AVR_XMEGA__
# ifndef SBOXNET_USE_HW_SENSOR_PINCH
#  define SBOXNET_USE_HW_SENSOR_PINCH 1
# endif
# ifndef SBOXNET_USE_HW_SENSOR_EXTINT
#  define SBOXNET_USE_HW_SENSOR_EXTINT 0
# endif
#endif
#ifdef SBOXNET_USE_HW_SENSOR_EXTRA_PIN
# error "please remove SBOXNET_USE_HW_SENSOR_EXTRA_PIN, SBOXNET_HW_SENSOR_PORT and SBOXNET_HW_SENSOR_BIT"
#endif

#ifdef SBOXNET_RxD_DDR
# error "please use new port definitions!"
#endif
    
// check mandatory defines
#ifndef SBOXNET_VAR
# error "SBOXNET_VAR not defined!"
#endif
#ifndef SBOXNET_RxD_PORT
# error "SBOXNET_RxD_PORT not defined!"
#endif
#ifndef SBOXNET_RxD_BIT
# error "SBOXNET_RxD_BIT not defined!"
#endif
#ifndef SBOXNET_TxD_PORT
# error "SBOXNET_TxD_PORT not defined!"
#endif
#ifndef SBOXNET_TxD_BIT
# error "SBOXNET_TxD_BIT not defined!"
#endif
#ifndef SBOXNET_DEVICE_ADDR
# error "SBOXNET_DEVICE_ADDR not defined!"
#endif
#ifdef __AVR_XMEGA__
# ifndef SBOXNET_USART_RXCINTLVL
#  error "SBOXNET_USART_RXCINTLVL not defined!"
# endif
# ifndef SBOXNET_USART_TXCINTLVL
#  error "SBOXNET_USART_TXCINTLVL not defined!"
# endif
# ifndef SBOXNET_TIMER
#  error "SBOXNET_TIMER not defined!"
# endif
# ifndef SBOXNET_TIMER_CC
#  error "SBOXNET_TIMER_CC not defined!"
# endif
# ifndef SBOXNET_TIMER_CCIF
#  error "SBOXNET_TIMER_CCIF not defined!"
# endif
# ifndef SBOXNET_TIMER_CCINTLVL_MASK
#  error "SBOXNET_TIMER_CCINTLVL_MASK not defined!"
# endif
# ifndef SBOXNET_TIMER_CCINTLVL
#  error "SBOXNET_TIMER_CCINTLVL not defined!"
# endif
#else
# ifndef SBOXNET_TIMER_TIMSK
#  error "SBOXNET_TIMER_TIMSK not defined!"
# endif
# ifndef SBOXNET_TIMER_TIFR
#  error "SBOXNET_TIMER_TIFR not defined!"
# endif
# ifndef SBOXNET_TIMER_OCIE
#  error "SBOXNET_TIMER_OCIE not defined!"
# endif
# ifndef SBOXNET_TIMER_OCF
#  error "SBOXNET_TIMER_OCF not defined!"
# endif
# ifndef SBOXNET_TIMER_TCNT
#  error "SBOXNET_TIMER_TCNT not defined!"
# endif
# ifndef SBOXNET_TIMER_OCR
#  error "SBOXNET_TIMER_OCR not defined!"
# endif
#endif
#ifndef SBOXNET_TIMER_BIT_TICKS
# error "SBOXNET_TIMER_BIT_TICKS not defined!"
#endif
#ifndef SBOXNET_TIMER_PERIOD_MASK
# error "SBOXNET_TIMER_PERIOD_MASK not defined!"
#endif
#ifndef SBOXNET_TIMER_RANDOM_MASK
# error "SBOXNET_TIMER_RANDOM_MASK not defined!"
#endif
#ifndef SBOXNET_USART_ID
# error "SBOXNET_USART_ID not defined!"
#endif
#ifndef SBOXNET_PRNG_SEED
# error "SBOXNET_PRNG_SEED not defined!"
#endif
#ifdef __AVR_XMEGA__
# ifndef SBOXNET_HW_SENSOR_INTLVL_MASK
#  error "SBOXNET_HW_SENSOR_INTLVL_MASK not defined!"
# endif
# ifndef SBOXNET_HW_SENSOR_INTMASK
#  error "SBOXNET_HW_SENSOR_INTMASK not defined!"
# endif
# ifndef SBOXNET_HW_SENSOR_INTF_BIT
#  error "SBOXNET_HW_SENSOR_INTF_BIT not defined!"
# endif
#else
# if SBOXNET_USE_HW_SENSOR_PINCH == 1
#  ifndef SBOXNET_HW_SENSOR_PCICR
#   error "SBOXNET_HW_SENSOR_PCICR not defined!"
#  endif
#  ifndef SBOXNET_HW_SENSOR_PCIE_BIT
#   error "SBOXNET_HW_SENSOR_PCIE_BIT not defined!"
#  endif
#  ifndef SBOXNET_HW_SENSOR_PCMSK
#   error "SBOXNET_HW_SENSOR_PCMSK not defined!"
#  endif
#  ifndef SBOXNET_HW_SENSOR_PCINT_BIT
#   error "SBOXNET_HW_SENSOR_PCINT_BIT not defined!"
#  endif
#  ifndef SBOXNET_HW_SENSOR_PCIFR
#   error "SBOXNET_HW_SENSOR_PCIFR not defined!"
#  endif
#  ifndef SBOXNET_HW_SENSOR_PCIF_BIT
#   error "SBOXNET_HW_SENSOR_PCIF_BIT not defined!"
#  endif
# elif SBOXNET_USE_HW_SENSOR_EXTINT == 1
#  ifndef SBOXNET_HW_SENSOR_EICR
#   error "SBOXNET_HW_SENSOR_EICR not defined!"
#  endif
#  ifndef SBOXNET_HW_SENSOR_ISC1
#   error "SBOXNET_HW_SENSOR_ISC1 not defined!"
#  endif
#  ifndef SBOXNET_HW_SENSOR_ISC0
#   error "SBOXNET_HW_SENSOR_ISC0 not defined!"
#  endif
#  ifndef SBOXNET_HW_SENSOR_EIMSK
#   error "SBOXNET_HW_SENSOR_EIMSK not defined!"
#  endif
#  ifndef SBOXNET_HW_SENSOR_IE_BIT
#   error "SBOXNET_HW_SENSOR_IE_BIT not defined!"
#  endif
#  ifndef SBOXNET_HW_SENSOR_EIFR
#   error "SBOXNET_HW_SENSOR_EIFR not defined!"
#  endif
#  ifndef SBOXNET_HW_SENSOR_IF_BIT
#   error "SBOXNET_HW_SENSOR_IF_BIT not defined!"
#  endif
# else
#  error "do not known which rxd line sensor to use"
# endif
#endif


#ifndef SBOXNET_SECTION
# define SBOXNET_SECTION
#endif
#ifndef SBOXNET_SECTION2
# define SBOXNET_SECTION2 SBOXNET_SECTION
#endif
    
#ifndef SBOXNET_USART_DEF_INTR
# define SBOXNET_USART_DEF_INTR 1
#endif

#ifndef SBOXNET_RANDOM_VALUE
# define SBOXNET_RANDOM_VALUE 0
#endif
    
#ifndef SBOXNET_USE_SNIFFER
# define SBOXNET_USE_SNIFFER 0
#endif



/* To save some space in Eeprom if debugging is enabled.
 */
#if SBOXNET_DEBUG_ENABLED != 0
SBOXNET_SECTION static NOINLINE void sboxnet_debug_inc_recverrors(void) { 
    SBOXNET_VAR->debug.recverrors++;
}

SBOXNET_SECTION static NOINLINE void sboxnet_debug_inc_recverr_dor(void) { 
    SBOXNET_VAR->debug.recverr_dor++;
}

SBOXNET_SECTION static NOINLINE void sboxnet_debug_inc_recv_byte(void) { 
    SBOXNET_VAR->debug.recv_byte++;
}
#endif


// Sboxnet CRC functions ---

SBOXNET_SECTION static INLINE uint8_t sboxnet_crc8_init(void) {
    return 0xff;
}

SBOXNET_SECTION static uint8_t sboxnet_crc8_add(uint8_t crc, uint8_t v) {
    return _crc_ibutton_update(crc, v);
}

SBOXNET_SECTION static INLINE uint8_t sboxnet_crc8_ok(uint8_t crc) {
    return crc == 0x00;
}

// Sboxnet Random ---

SBOXNET_SECTION static uint8_t sboxnet_prandom(void) {
    SBOXNET_VAR->prng_seed = prng_xorshift8(SBOXNET_VAR->prng_seed);
    return SBOXNET_VAR->prng_seed;
}


// Sboxnet Ringbuffer ---

SBOXNET_SECTION static void sboxnet_rb_init(struct sboxnet_ring_buffer* rb, uint8_t size) {
    rb->rd = 0;
    rb->rd_count = 0;
    rb->rd_rollback = 0;
    rb->wr = 0;
    rb->wr_count = 0;
    rb->wr_rollback = 0;
    rb->count = 0;
    rb->size = size;
}


SBOXNET_SECTION static INLINE uint8_t sboxnet_rb_getcount(struct sboxnet_ring_buffer* rb) {
    return rb->count;
}
SBOXNET_SECTION static INLINE uint8_t sboxnet_rb_getfree(struct sboxnet_ring_buffer* rb) {
    return rb->size - rb->count - rb->wr_count;
}

SBOXNET_SECTION static INLINE void sboxnet_rb_write_commit(struct sboxnet_ring_buffer* rb) {
#if SBOXNET_DEBUG_ENABLED != 0
    if ((rb->count + rb->wr_count) > rb->size) {
        setbit(SBOXNET_VAR->flags, SBOXNET_FLAG_WBUFOV_b);          
    }
#endif
    rb->count += rb->wr_count;
    rb->wr_count = 0;
    rb->wr_rollback = rb->wr;
}

SBOXNET_SECTION static void sboxnet_rb_write_rollback(struct sboxnet_ring_buffer* rb) {
    rb->wr_count = 0;
    rb->wr = rb->wr_rollback;
}

SBOXNET_SECTION static uint8_t sboxnet_rb_write(struct sboxnet_ring_buffer* rb, uint8_t a_v) {    
    uint8_t rc = 0;
    if ((uint8_t)(rb->count + rb->wr_count) < rb->size) {
        rb->wr_count++;
        rb->buf[rb->wr] = a_v;
        rb->wr++;
        if (rb->wr >= rb->size) {
            rb->wr = 0;
        }
        rc = 1;
    } else {
#if SBOXNET_DEBUG_ENABLED != 0
        setbit(SBOXNET_VAR->flags, SBOXNET_FLAG_WBUFOV_b);
#endif
        rc = 0;
    }
    return rc;
}

SBOXNET_SECTION static void sboxnet_rb_put_first(struct sboxnet_ring_buffer* rb, uint8_t v) {
    rb->buf[rb->wr_rollback] = v;
}

SBOXNET_SECTION static INLINE uint8_t sboxnet_rb_write_count(struct sboxnet_ring_buffer* rb) {
    return rb->wr_count;
}
 
SBOXNET_SECTION static void sboxnet_rb_read_commit(struct sboxnet_ring_buffer* rb) {
#if SBOXNET_DEBUG_ENABLED != 0
    if (rb->count < rb->rd_count) {
        setbit(SBOXNET_VAR->flags, SBOXNET_FLAG_RBUFOV_b);
    }
#endif
    rb->count -= rb->rd_count;
    rb->rd_count = 0;
    rb->rd_rollback = rb->rd;
}

SBOXNET_SECTION static void sboxnet_rb_read_rollback(struct sboxnet_ring_buffer* rb) {
    rb->rd_count = 0;
    rb->rd = rb->rd_rollback;
}

SBOXNET_SECTION static uint8_t sboxnet_rb_read(struct sboxnet_ring_buffer* rb) {
    uint8_t v = 0;
    if (rb->count > rb->rd_count) {
        rb->rd_count++;
        v = rb->buf[rb->rd];
        rb->rd++;
        if (rb->rd >= rb->size) {
            rb->rd = 0;
        }
    } else {
#if SBOXNET_DEBUG_ENABLED != 0
        setbit(SBOXNET_VAR->flags, SBOXNET_FLAG_RBUFOV_b);
#endif
    }
    return v;
}

SBOXNET_SECTION static uint8_t sboxnet_rb_read_getone(struct sboxnet_ring_buffer* rb) {
    uint8_t v = 0;
    if (rb->count > rb->rd_count) {
        v = rb->buf[rb->rd];
    } else {
#if SBOXNET_DEBUG_ENABLED != 0
        setbit(SBOXNET_VAR->flags, SBOXNET_FLAG_RBUFOV_b);
#endif
    }
    return v;
}

SBOXNET_SECTION static INLINE uint8_t sboxnet_rb_can_read_bytes(struct sboxnet_ring_buffer* rb) {
    return rb->count - rb->rd_count;
}

SBOXNET_SECTION NOINLINE __ATTR_USED static void sboxnet_receiver_enable(void) {
#ifdef __AVR_XMEGA__
    SBOXNET_USART_ID.CTRLA = (SBOXNET_USART_ID.CTRLA & ~USART_RXCINTLVL_gm) | SBOXNET_USART_RXCINTLVL;
    setbv(SBOXNET_USART_ID.CTRLB, USART_RXEN_bm|USART_MPCM_bm);
#else
    setbit(__UCSRA, __MPCM);
    __UCSRB |= (Bit(__RXEN)|Bit(__RXCIE));
#endif
}

// Sboxnet init

SBOXNET_SECTION NOINLINE __ATTR_USED static void sboxnet_init(void) {
    struct sboxnet_device *dev = SBOXNET_VAR;
    memset(dev, 0, sizeof(struct sboxnet_device));
    dev->state = SBOXNET_STATE_NONE;
    dev->prng_seed = SBOXNET_PRNG_SEED;
    dev->retry_counter = SBOXNET_TMIT_RETRY;
    
    sboxnet_rb_init(&dev->recv_buf.rb, sizeof(dev->recv_buf.buf));
    sboxnet_rb_init(&dev->tmit_buf.rb, sizeof(dev->tmit_buf.buf));
    
    port_dirin(SBOXNET_RxD_PORT, Bit(SBOXNET_RxD_BIT));
    port_pullup_on(SBOXNET_RxD_PORT, Bit(SBOXNET_RxD_BIT)); // set RxD pin to input, pullup on
    
    port_setbit(SBOXNET_TxD_PORT, SBOXNET_TxD_BIT); // set TxD pin to output and value "1" (MARK)
    port_dirout(SBOXNET_TxD_PORT, Bit(SBOXNET_TxD_BIT));
    
    // USART
#ifdef __AVR_XMEGA__
    SBOXNET_RxD_PORT.INTCTRL &= ~SBOXNET_HW_SENSOR_INTLVL_MASK;
    SBOXNET_RxD_PORT.SBOXNET_HW_SENSOR_INTMASK = Bit(SBOXNET_RxD_BIT);
    SBOXNET_RxD_PORT.INTFLAGS = Bit(SBOXNET_HW_SENSOR_INTF_BIT);
    
    SBOXNET_USART_ID.BAUDCTRLB = highbyte(SBOXNET_UBRR_VAL) & 0x0f;
    SBOXNET_USART_ID.BAUDCTRLA = lowbyte(SBOXNET_UBRR_VAL);
    
    SBOXNET_USART_ID.STATUS = Bit(USART_TXCIF_bp); // clear TXCIF
    SBOXNET_USART_ID.CTRLA = USART_RXCINTLVL_OFF_gc|USART_TXCINTLVL_OFF_gc|USART_DREINTLVL_OFF_gc;
    SBOXNET_USART_ID.CTRLB = Bsv(USART_RXEN_bp,0)|Bsv(USART_TXEN_bp,0)|Bsv(USART_CLK2X_bp,0)|Bsv(USART_MPCM_bp,1);
    SBOXNET_USART_ID.CTRLC = USART_CMODE_ASYNCHRONOUS_gc|USART_PMODE_EVEN_gc|Bsv(USART_SBMODE_bp,0)|USART_CHSIZE_9BIT_gc;
    
#else // __AVR_XMEGA__
    // line hw sensor
#if SBOXNET_USE_HW_SENSOR_PINCH == 1
    clrbit(SBOXNET_HW_SENSOR_PCICR, SBOXNET_HW_SENSOR_PCIE_BIT);
    SBOXNET_HW_SENSOR_PCMSK = Bit(SBOXNET_HW_SENSOR_PCINT_BIT);
    SBOXNET_HW_SENSOR_PCIFR = Bit(SBOXNET_HW_SENSOR_PCIF_BIT);
#else
    clrbit(SBOXNET_HW_SENSOR_EIMSK, SBOXNET_HW_SENSOR_IE_BIT); // disable interrupt
    clrbit(SBOXNET_HW_SENSOR_EICR, SBOXNET_HW_SENSOR_ISC1); // any edge of INT
    setbit(SBOXNET_HW_SENSOR_EICR, SBOXNET_HW_SENSOR_ISC0);
    SBOXNET_HW_SENSOR_EIFR = Bit(SBOXNET_HW_SENSOR_IF_BIT); // reset flag
#endif
    __UBRRH = highbyte(SBOXNET_UBRR_VAL) & 0x7f; // Bit URSEL=0
    __UBRRL = lowbyte(SBOXNET_UBRR_VAL);
    __UCSRA = Bsv(__TXC,1)|Bsv(__U2X,0)|Bsv(__MPCM,0); // clear TXC, normal speed
    __UCSRC = 
#ifdef __URSEL
            Bsv(__URSEL,1)|
#endif
            Bsv(__UMSEL0,0)|Bsv(__UPM1,1)|Bsv(__UPM0,0)|Bsv(__USBS,0)|Bsv(__UCSZ1,1)|Bsv(__UCSZ0,1); // async, even parity, 1 stop bit, 9 chars

    __UCSRB = Bsv(__RXCIE,0)|Bsv(__TXCIE,0)|Bsv(__UDRIE,0)|Bsv(__RXEN,0)|Bsv(__TXEN,0)|Bsv(__UCSZ2,1)|Bsv(__TXB8,0); // 9 chars
#endif // __AVR_XMEGA__
    
    sboxnet_receiver_enable();
}

// Sboxnet deinit

SBOXNET_SECTION __ATTR_NO_UNUSED_WARN 
static void sboxnet_deinit(void) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        SBOXNET_VAR->state = SBOXNET_STATE_NONE;
#ifdef __AVR_XMEGA__
        SBOXNET_USART_ID.CTRLB = Bsv(USART_RXEN_bp,0)|Bsv(USART_TXEN_bp,0)|Bsv(USART_CLK2X_bp,0)|Bsv(USART_MPCM_bp,0);
        SBOXNET_USART_ID.CTRLA = USART_RXCINTLVL_OFF_gc|USART_TXCINTLVL_OFF_gc|USART_DREINTLVL_OFF_gc;
#else // __AVR_XMEGA__
        __UCSRB = Bsv(__RXCIE,0)|Bsv(__TXCIE,0)|Bsv(__UDRIE,0)|Bsv(__RXEN,0)|Bsv(__TXEN,0)|Bsv(__UCSZ2,1)|Bsv(__TXB8,0); // 9 chars
#endif // __AVR_XMEGA__
        
        port_dirin(SBOXNET_RxD_PORT, Bit(SBOXNET_RxD_BIT));
        port_dirin(SBOXNET_TxD_PORT, Bit(SBOXNET_TxD_BIT));
        port_pullup_on(SBOXNET_TxD_PORT, Bit(SBOXNET_TxD_BIT));
#ifdef __AVR_XMEGA__
        SBOXNET_RxD_PORT.INTFLAGS = Bit(SBOXNET_HW_SENSOR_INTF_BIT);
        SBOXNET_RxD_PORT.SBOXNET_HW_SENSOR_INTMASK = 0;
#else
#if SBOXNET_USE_HW_SENSOR_PINCH == 1
        clrbit(SBOXNET_HW_SENSOR_PCMSK, SBOXNET_HW_SENSOR_PCINT_BIT);        
#else
        clrbit(SBOXNET_HW_SENSOR_EIMSK, SBOXNET_HW_SENSOR_IE_BIT); // disable interrupt
#endif
#endif
    }
}

SBOXNET_SECTION static void sboxnet_set_timer(uint16_t offset) {
#ifdef __AVR_XMEGA__
    register uint16_t ocr;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ocr = SBOXNET_TIMER.CNT; // 16bit register access !
    }
    ocr = (ocr + offset) & SBOXNET_TIMER_PERIOD_MASK;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        SBOXNET_TIMER.SBOXNET_TIMER_CC = ocr;
    }
    SBOXNET_TIMER.INTFLAGS = Bit(SBOXNET_TIMER_CCIF);
    SBOXNET_TIMER.INTCTRLB = (SBOXNET_TIMER.INTCTRLB & ~SBOXNET_TIMER_CCINTLVL_MASK) | SBOXNET_TIMER_CCINTLVL;
#else
    register uint16_t ocr = SBOXNET_TIMER_TCNT; // 16bit register access, we are here already in an interrupt
    SBOXNET_TIMER_OCR = (ocr + offset) & SBOXNET_TIMER_PERIOD_MASK;
    SBOXNET_TIMER_TIFR = Bit(SBOXNET_TIMER_OCF);
    setbit(SBOXNET_TIMER_TIMSK, SBOXNET_TIMER_OCIE);
#endif
}

SBOXNET_SECTION static inline void sboxnet_clear_line_sensor(void) {
#ifdef __AVR_XMEGA__
    SBOXNET_RxD_PORT.INTFLAGS = Bit(SBOXNET_HW_SENSOR_INTF_BIT);
#else
#if SBOXNET_USE_HW_SENSOR_PINCH == 1
    SBOXNET_HW_SENSOR_PCIFR = Bit(SBOXNET_HW_SENSOR_PCIF_BIT); // clear HW SENSOR interrupt flag
#else
    SBOXNET_HW_SENSOR_EIFR = Bit(SBOXNET_HW_SENSOR_IF_BIT); // clear HW SENSOR interrupt flag
#endif
#endif
}

SBOXNET_SECTION static inline uint8_t sboxnet_line_sensor_changed(void) {
#ifdef __AVR_XMEGA__
    return bit_is_set(SBOXNET_RxD_PORT.INTFLAGS, SBOXNET_HW_SENSOR_INTF_BIT);
#else
#if SBOXNET_USE_HW_SENSOR_PINCH == 1
    return bit_is_set(SBOXNET_HW_SENSOR_PCIFR, SBOXNET_HW_SENSOR_PCIF_BIT);
#else
    return bit_is_set(SBOXNET_HW_SENSOR_EIFR, SBOXNET_HW_SENSOR_IF_BIT);
#endif
#endif
}

SBOXNET_SECTION static void sboxnet_set_backoff_with_offset(uint16_t offset) {
    sboxnet_clear_line_sensor();
    SBOXNET_VAR->state = SBOXNET_STATE_BACKOFF;
    sboxnet_set_timer(SBOXNET_TIMER_BIT_TICKS * SBOXNET_BACKOFF_BITS + offset);
}


SBOXNET_SECTION static void sboxnet_intr_usart_rx(void) {
    uint8_t rxb8, rxb, rflags;
    struct sboxnet_device *dev = SBOXNET_VAR;
    FORCEPTR(dev);

#ifdef __AVR_XMEGA__
    rflags = SBOXNET_USART_ID.STATUS;
    rxb8 = bit_is_set(rflags, USART_RXB8_bp);
    rxb = SBOXNET_USART_ID.DATA;
    rflags &= (USART_FERR_bm|USART_BUFOVF_bm|USART_PERR_bm);
#else
    rflags = __UCSRA;
    rxb8 = bit_is_set(__UCSRB, __RXB8); // MPCM address byte ?
    rxb = __UDR;
    rflags &= (Bit(__FE)|Bit(__DOR)|Bit(__UPE));
#endif
    if (rflags) {
        SBOXNET_DEBUG(sboxnet_debug_inc_recverrors();)
#ifdef __AVR_XMEGA__
        SBOXNET_DEBUG(if (rflags & USART_FERR_bm)   { dev->debug.recverr_fe++; } )
        SBOXNET_DEBUG(if (rflags & USART_BUFOVF_bm) { sboxnet_debug_inc_recverr_dor(); } )
        SBOXNET_DEBUG(if (rflags & USART_PERR_bm)   { dev->debug.recverr_pe++; } )
#else
        SBOXNET_DEBUG(if (rflags & Bit(__FE))  { dev->debug.recverr_fe++; } )
        SBOXNET_DEBUG(if (rflags & Bit(__DOR)) { sboxnet_debug_inc_recverr_dor(); } )
        SBOXNET_DEBUG(if (rflags & Bit(__UPE)) { dev->debug.recverr_pe++; } )
#endif
        goto receive_reset;
        
    } else if (rxb8) { // if nonzero, it is an address byte
        if (sboxnet_rb_write_count(&dev->recv_buf.rb) != 0) { // ups, there is already something in the buffer
            SBOXNET_DEBUG(dev->debug.recverr_proto++;) // must be a protocol error, discard it
            SBOXNET_DEBUG(sboxnet_debug_inc_recverrors();)
            sboxnet_rb_write_rollback(&dev->recv_buf.rb);
        }
        if (rxb == SBOXNET_DEVICE_ADDR || rxb == SBOXNET_ADDR_BROADCAST
#if SBOXNET_USE_SNIFFER == 1
            || bit_is_set(dev->flags, SBOXNET_FLAG_SNIFFER_b)
#endif
                                                ) { // it is a message for us
#ifdef __AVR_XMEGA__
            clrbit(SBOXNET_USART_ID.CTRLB, USART_MPCM_bp);
#else
            clrbit(__UCSRA, __MPCM); // disable Multiprocessor Mode to receive next bytes of message
#endif
            if (!sboxnet_rb_write(&dev->recv_buf.rb, 0)) { // placeholder for message length
                goto rec_dor;
            }
            if (sboxnet_rb_write(&dev->recv_buf.rb, rxb)) {
                SBOXNET_DEBUG(sboxnet_debug_inc_recv_byte();)
                dev->recv_len = SBOXNET_MSG_MIN_LEN;
            } else {
              rec_dor:
                SBOXNET_DEBUG(sboxnet_debug_inc_recverr_dor();)
                goto receive_error;
            }
        } else {
            // wait for next address byte
            goto receive_reset;
        }
        
    } else { // data byte
        uint8_t writecount = sboxnet_rb_write_count(&dev->recv_buf.rb);
        if (writecount > dev->recv_len) { // note: writecount+1 !  first byte in buffer is message length byte!
            SBOXNET_DEBUG(dev->debug.recverr_size++;)
            goto receive_error;
        } else {
            if (writecount == (offsetof(struct sboxnet_msg_header, opt)+1)) { // first byte is msglen !
                dev->recv_len = SBOXNET_MSG_MIN_LEN + (rxb & SBOXNET_MSG_DATA_LEN_MASK); // calculate packet length
            }
            if (!sboxnet_rb_write(&dev->recv_buf.rb, rxb)) {
                SBOXNET_DEBUG(sboxnet_debug_inc_recverr_dor();)
                goto receive_error;
            }
            SBOXNET_DEBUG(sboxnet_debug_inc_recv_byte();)
            if (sboxnet_rb_write_count(&dev->recv_buf.rb) == dev->recv_len+1) { // end condition, enable MPCM. note: writecount+1 !  first byte in buffer is message length byte!
                sboxnet_rb_put_first(&dev->recv_buf.rb, dev->recv_len);
                dev->recv_len = 0;
#ifdef __AVR_XMEGA__
                setbit(SBOXNET_USART_ID.CTRLB, USART_MPCM_bp);
#else
                setbit(__UCSRA, __MPCM);
#endif
                sboxnet_rb_write_commit(&dev->recv_buf.rb);
                SBOXNET_DEBUG(SBOXNET_VAR->debug.recv_msg_got++;)
            }
        }
    }
    return;
    
receive_error:
    SBOXNET_DEBUG(sboxnet_debug_inc_recverrors();)
receive_reset:
    dev->recv_len = 0;
#ifdef __AVR_XMEGA__
    setbit(SBOXNET_USART_ID.CTRLB, USART_MPCM_bp);
#else
    setbit(__UCSRA, __MPCM);
#endif
    sboxnet_rb_write_rollback(&dev->recv_buf.rb);
}

// must be called with interrupts off !
SBOXNET_SECTION static void sboxnet_check_try_transmit(void) {
    struct sboxnet_device *dev = SBOXNET_VAR;
    FORCEPTR(dev);
    
    if (dev->tmit_bytes == 0 && sboxnet_rb_getcount(&dev->tmit_buf.rb) > SBOXNET_MSG_MIN_LEN) {
        dev->tmit_cnt = 0;
        dev->tmit_bytes = sboxnet_rb_read(&dev->tmit_buf.rb); // read message length
    }
    if (dev->tmit_bytes > 0 && dev->tmit_cnt == 0) {
        if (dev->state == SBOXNET_STATE_NONE) {
            uint16_t offset = sboxnet_prandom() & SBOXNET_TIMER_RANDOM_MASK;
            sboxnet_set_backoff_with_offset((uint16_t)dev->retry_counter * (SBOXNET_TIMER_BIT_TICKS / 4) + SBOXNET_TIMER_BIT_TICKS*2 + offset);
        }
    }
}

SBOXNET_SECTION static void sboxnet_intr_usart_tx(void) {
    struct sboxnet_device *dev = SBOXNET_VAR;
    FORCEPTR(dev);

    // collision check
#ifdef __AVR_XMEGA__
    uint8_t ucsra = SBOXNET_USART_ID.STATUS;
    uint8_t crxc = !(ucsra & USART_RXCIF_bm); // echo receive not complete
    uint8_t cfe  = (ucsra & (USART_FERR_bm|USART_PERR_bm)); // receive frame or parity error
    uint8_t cudr = (SBOXNET_USART_ID.DATA != dev->tmit_lastbyte); // echo byte != transmit byte
#else
    uint8_t ucsra = __UCSRA;
    uint8_t crxc = !(ucsra & Bit(__RXC)); // echo receive not complete
    uint8_t cfe  = (ucsra & (Bit(__FE)|Bit(__UPE))); // receive frame or parity error
    uint8_t cudr = (__UDR != dev->tmit_lastbyte); // echo byte != transmit byte
#endif
    if (crxc || cfe || cudr) {
        SBOXNET_DEBUG(if (crxc) { dev->debug.coll_rxc++; } )
        SBOXNET_DEBUG(if (cfe)  { dev->debug.coll_fe++; } )
        SBOXNET_DEBUG(if (cudr) { dev->debug.coll_udr++; } )

        port_clrbit(SBOXNET_TxD_PORT, SBOXNET_TxD_BIT); // send a 0 to the line
#ifdef __AVR_XMEGA__
        SBOXNET_USART_ID.CTRLB &= ~(USART_TXEN_bm|USART_RXEN_bm); // and disable transmitter and receiver
        SBOXNET_USART_ID.CTRLA &= ~(USART_TXCINTLVL_gm);
#else
        __UCSRB &= ~(Bit(__TXCIE)|Bit(__TXEN)|Bit(__RXEN)); // and disable transmitter and receiver
#endif
        dev->state = SBOXNET_STATE_COLLISION;
        SBOXNET_DEBUG(dev->debug.collisions++;)
        if (dev->retry_counter > 0) {
            dev->retry_counter--;
            sboxnet_rb_read_rollback(&dev->tmit_buf.rb);

            SBOXNET_DEBUG(dev->debug.retries++;)
        } else { // discard message !?
            while (dev->tmit_cnt < dev->tmit_bytes) {
                sboxnet_rb_read(&dev->tmit_buf.rb);
                dev->tmit_cnt++;
            }
            dev->tmit_cnt = dev->tmit_bytes = 0;
            dev->retry_counter = SBOXNET_TMIT_RETRY;
            sboxnet_rb_read_commit(&dev->tmit_buf.rb);
            SBOXNET_DEBUG(dev->debug.tmiterrors++;)
        }
        dev->tmit_cnt = dev->tmit_bytes = 0;
        // enable collision timer
        sboxnet_set_timer(SBOXNET_TIMER_BIT_TICKS * SBOXNET_COLLISION_BREAK_BITS);
        return;
    }
#ifdef __AVR_XMEGA__
    clrbit(SBOXNET_USART_ID.CTRLB, USART_RXEN_bp);
    SBOXNET_USART_ID.CTRLA &= ~(USART_RXCINTLVL_gm);
    setbit(SBOXNET_USART_ID.CTRLB, USART_RXEN_bp);    
#else
    __UCSRB &= ~(Bit(__RXCIE)|Bit(__RXEN)); // flush receiver
    __UCSRB |= Bit(__RXEN);
#endif

    if (dev->tmit_cnt < dev->tmit_bytes) {
        uint8_t v = sboxnet_rb_read(&dev->tmit_buf.rb);
        dev->tmit_cnt++;
        dev->tmit_lastbyte = v;
#ifdef __AVR_XMEGA__
        clrbit(SBOXNET_USART_ID.CTRLB, USART_TXB8_bp);
        SBOXNET_USART_ID.DATA = v;
#else
        clrbit(__UCSRB, __TXB8); // clear bit 8 => marks data byte in Multiprocessor Mode
        __UDR = v;
#endif
    } else {
#ifdef __AVR_XMEGA__
        SBOXNET_USART_ID.CTRLA &= ~(USART_TXCINTLVL_gm);
#else
        clrbit(__UCSRB, __TXCIE);
#endif
        dev->tmit_bytes = 0;
        dev->tmit_cnt = 0;
        sboxnet_rb_read_commit(&dev->tmit_buf.rb);
        SBOXNET_DEBUG(SBOXNET_VAR->debug.tmit_msg_sent++;)

        sboxnet_receiver_enable();
        if (dev->state == SBOXNET_STATE_TRANSMIT) {
            dev->state = SBOXNET_STATE_NONE;
        }
        dev->retry_counter = SBOXNET_TMIT_RETRY;
        sboxnet_check_try_transmit();
    }
    SBOXNET_DEBUG(dev->debug.tmit_byte++;)
}


#if SBOXNET_USART_DEF_INTR == 1
#ifdef __AVR_XMEGA__
SBOXNET_SECTION ISR(SBOXNET_USART_RxC_vect) {
    sboxnet_intr_usart_rx();
}

SBOXNET_SECTION ISR(SBOXNET_USART_TxC_vect) {
    sboxnet_intr_usart_tx();
}
#else
SBOXNET_SECTION ISR(__USART_RX_vect) {
    sboxnet_intr_usart_rx();
}

SBOXNET_SECTION ISR(__USART_TX_vect) {
    sboxnet_intr_usart_tx();
}
#endif
#endif

SBOXNET_SECTION static void sboxnet_timer_bit(void) { // must be run with interrupts off !
    struct sboxnet_device *dev = SBOXNET_VAR;
    //FORCEPTR(dev);
#ifdef __AVR_XMEGA__
    SBOXNET_TIMER.INTCTRLB &= ~SBOXNET_TIMER_CCINTLVL_MASK;
#else
    clrbit(SBOXNET_TIMER_TIMSK, SBOXNET_TIMER_OCIE);
#endif
    if (dev->state == SBOXNET_STATE_BACKOFF) {
        if (dev->tmit_bytes > 0 && dev->tmit_cnt == 0) {
            uint8_t d = sboxnet_rb_read_getone(&dev->tmit_buf.rb);
            
            port_setbit(SBOXNET_TxD_PORT, SBOXNET_TxD_BIT);
            // set bit 8 => marks address byte in Multiprocessor Mode
#ifdef __AVR_XMEGA__
            setbv(SBOXNET_USART_ID.CTRLB, USART_TXEN_bm|USART_TXB8_bm);
#else
            __UCSRB |= Bit(__TXEN)|Bit(__TXB8); // enable transmitter
#endif            
            if (sboxnet_line_sensor_changed() || bit_is_clear(port_in(SBOXNET_RxD_PORT), SBOXNET_RxD_BIT)) {
                uint16_t offset = sboxnet_prandom() & SBOXNET_TIMER_RANDOM_MASK;
                sboxnet_set_backoff_with_offset((uint16_t)dev->retry_counter * (SBOXNET_TIMER_BIT_TICKS / 4) + offset);
            
            } else {
#ifdef __AVR_XMEGA__
                clrbit(SBOXNET_USART_ID.CTRLB, USART_RXEN_bp);
                SBOXNET_USART_ID.CTRLA &= ~(USART_RXCINTLVL_gm|USART_TXCINTLVL_gm);
                setbit(SBOXNET_USART_ID.CTRLB, USART_RXEN_bp);
                SBOXNET_USART_ID.CTRLA |= SBOXNET_USART_TXCINTLVL;
                SBOXNET_USART_ID.DATA = d;
                clrbit(SBOXNET_USART_ID.CTRLB, USART_MPCM_bp);
#else
                __UCSRB &= ~(Bit(__RXEN)|Bit(__RXCIE)); // flush receiver
                __UCSRB |= Bit(__RXEN)|Bit(__TXCIE); // and enable transmitter interrupts
                __UDR = d; // start transmission, after receiver is enabled ! (else receiver may not sync... at 16Mhz Clk...)
                clrbit(__UCSRA, __MPCM);
#endif
                sboxnet_rb_read(&dev->tmit_buf.rb);
                dev->tmit_lastbyte = d;
                dev->tmit_cnt++;
                dev->state = SBOXNET_STATE_TRANSMIT;
            }
        } else {
            dev->state = SBOXNET_STATE_NONE;
        }
        return;
    }
    
    if (dev->state == SBOXNET_STATE_COLLISION) {
        port_setbit(SBOXNET_TxD_PORT, SBOXNET_TxD_BIT); // first set txd to High
        dev->state = SBOXNET_STATE_COLLCHECK;
        sboxnet_set_timer(SBOXNET_TIMER_BIT_TICKS * SBOXNET_COLLCHECK_BITS);
        sboxnet_clear_line_sensor();                    // then clear the line sensor
        return;
    }
    
    if (dev->state == SBOXNET_STATE_COLLCHECK) {
        if (!sboxnet_line_sensor_changed() && bit_is_set(port_in(SBOXNET_RxD_PORT), SBOXNET_RxD_BIT)) {
            sboxnet_receiver_enable();
            dev->state = SBOXNET_STATE_NONE;
            sboxnet_check_try_transmit();
        } else {
            sboxnet_clear_line_sensor();
            sboxnet_set_timer(SBOXNET_TIMER_BIT_TICKS * SBOXNET_COLLCHECK_BITS);
        }
        return;
    }
}

// Sboxnet receive a message

SBOXNET_SECTION NOINLINE __ATTR_USED static int8_t sboxnet_receive_msg(struct sboxnet_msg_header* pmsg, uint8_t maxmsglen) {
    if (maxmsglen < SBOXNET_MSG_MIN_LEN)
        return SBOXNET_ERR_RECVMSG_TOLONG;
    
    struct sboxnet_device *dev = SBOXNET_VAR;
    uint8_t msglen = 0;
    uint8_t maxb = SBOXNET_MSG_MIN_LEN;
    uint8_t avail;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        avail = sboxnet_rb_can_read_bytes(&dev->recv_buf.rb);
        if (avail > SBOXNET_MSG_MIN_LEN)
            msglen = sboxnet_rb_read_getone(&dev->recv_buf.rb);
    }
    if (msglen < maxb ) {
        return SBOXNET_ERR_RECVMSG_NOMSG;
    }
    if ((msglen+1) > avail) { // msglen+1 because of first maxlen byte
        return SBOXNET_ERR_RECVMSG_NOMSG;
    }
    if (msglen > maxmsglen) {
        return SBOXNET_ERR_RECVMSG_TOLONG;
    }
    if (msglen > SBOXNET_MSG_MAX_LEN) {
        return SBOXNET_ERR_RECVMSG_INVLEN;
    }
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        sboxnet_rb_read(&dev->recv_buf.rb); // dummy read, msglen
    }
    uint8_t* p = (uint8_t*)pmsg;
    uint8_t crc = sboxnet_crc8_init();
    for (uint8_t i = 0; i < msglen; i++) {
        uint8_t v;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            v = sboxnet_rb_read(&dev->recv_buf.rb);
        }
        if (i == offsetof(struct sboxnet_msg_header, opt)) {
            maxb += (v & SBOXNET_MSG_DATA_LEN_MASK); // plus Data Length
        }
        crc = sboxnet_crc8_add(crc, v);
        *p++ = v;
    }
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        sboxnet_rb_read_commit(&dev->recv_buf.rb);
    }
 
    if (!sboxnet_crc8_ok(crc)) {
        SBOXNET_DEBUG(sboxnet_debug_inc_recverrors();)
        SBOXNET_DEBUG(SBOXNET_VAR->debug.crcerrors++;)
        return SBOXNET_ERR_RECVMSG_CRC;
    } else {
       SBOXNET_DEBUG(SBOXNET_VAR->debug.recv_msg_fetched++;) 
    }
    return msglen;
}

// Sboxnet transmit a message.

SBOXNET_SECTION NOINLINE __ATTR_USED static uint8_t sboxnet_send_msg(struct sboxnet_msg_header* pmsg) {
    struct sboxnet_device *dev = SBOXNET_VAR;
    uint8_t msglen = SBOXNET_MSG_MIN_LEN + pmsg->opt.len;
    
    uint8_t free;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        free = sboxnet_rb_getfree(&dev->tmit_buf.rb);
    }
    if (!(free > msglen)) {
        return 1;
    }

    uint8_t rc;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        rc = sboxnet_rb_write(&dev->tmit_buf.rb, msglen);
    }
    if (!rc) {
        goto send_error;
    }
    uint8_t crc = sboxnet_crc8_init();
    uint8_t i;
    for (i = 0; i < (msglen-1); i++) {
        uint8_t d;
        if (i == 1) { // srcaddr
            d = SBOXNET_DEVICE_ADDR;
        } else {
            d = ((uint8_t*)pmsg)[i];
        }
        crc = sboxnet_crc8_add(crc, d);
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            rc = sboxnet_rb_write(&dev->tmit_buf.rb, d);
        }
        if (!rc) {
            goto send_error;
        }
    }
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        rc = sboxnet_rb_write(&dev->tmit_buf.rb, crc);
    }
    if (!rc) {
 send_error:
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            sboxnet_rb_write_rollback(&dev->tmit_buf.rb);
        }
        return 1;
    }
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        sboxnet_rb_write_commit(&dev->tmit_buf.rb);
        sboxnet_check_try_transmit();
    }
    SBOXNET_DEBUG(SBOXNET_VAR->debug.tmit_msg_tosend++;)
    return 0;
}

SBOXNET_SECTION2 NOINLINE static uint8_t sboxnet_can_send_msg(struct sboxnet_msg_header* pmsg) {
    uint8_t msglen = SBOXNET_MSG_MIN_LEN + pmsg->opt.len;
    uint8_t free;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        free = sboxnet_rb_getfree(&SBOXNET_VAR->tmit_buf.rb);
    }
    return (free > msglen);
}
SBOXNET_SECTION2 NOINLINE static uint8_t sboxnet_all_sent(void) {
    return sboxnet_rb_getcount(&SBOXNET_VAR->tmit_buf.rb) == 0;
}
SBOXNET_SECTION2 NOINLINE static uint8_t sboxnet_can_read(void) {
    uint8_t b;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        b = sboxnet_rb_can_read_bytes(&SBOXNET_VAR->recv_buf.rb);
    }
    return (b >= SBOXNET_MSG_MIN_LEN);
}
SBOXNET_SECTION2 NOINLINE static uint8_t sboxnet_can_send(void) {
    uint8_t free;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        free = sboxnet_rb_getfree(&SBOXNET_VAR->tmit_buf.rb);
    }
    return (free > SBOXNET_MSG_MIN_LEN);
}

SBOXNET_SECTION2 static NOINLINE void sboxnet_set_sniffer_mode(uint8_t on) {
    if (on) {
        setbit(SBOXNET_VAR->flags, SBOXNET_FLAG_SNIFFER_b);
    } else {
        clrbit(SBOXNET_VAR->flags, SBOXNET_FLAG_SNIFFER_b);
    }
}


#ifndef __AVR_XMEGA__
#undef __SELECT_USART
#include "avrutilslib/select-usart.h"
#endif