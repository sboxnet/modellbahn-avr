/***************************************************************************
 *   Copyright (C) 2012-2020
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

#include "sboxnet.h"

#ifndef __AVR_XMEGA__
#undef __SELECT_USART
#define __SELECT_USART  SBOXNET_USART_ID
#include "avrutilslib/select-usart.h"
#endif

//#define ICEDBG y

#ifndef __AVR_XMEGA__
# ifndef SBOXNET_USE_HW_SENSOR_PINCH
#  define SBOXNET_USE_HW_SENSOR_PINCH 1
# endif // SBOXNET_USE_HW_SENSOR_PINCH
# ifndef SBOXNET_USE_HW_SENSOR_EXTINT
#  define SBOXNET_USE_HW_SENSOR_EXTINT 0
# endif // SBOXNET_USE_HW_SENSOR_EXTINT
#endif
#ifdef SBOXNET_USE_HW_SENSOR_EXTRA_PIN
# error "please remove SBOXNET_USE_HW_SENSOR_EXTRA_PIN, SBOXNET_HW_SENSOR_PORT and SBOXNET_HW_SENSOR_BIT"
#endif

#ifdef SBOXNET_RxD_DDR
# error "please use new port definitions!"
#endif
    
// check mandatory defines
#ifndef SBOXNET_VAR // Sboxnet Zeiger auf sboxnet Bereich struct
# error "SBOXNET_VAR not defined!"
#endif
// Receiver Port
#ifndef SBOXNET_RxD_PORT
# error "SBOXNET_RxD_PORT not defined!"
#endif
// Bitnummer des Receiver Bits
#ifndef SBOXNET_RxD_BIT
# error "SBOXNET_RxD_BIT not defined!"
#endif
// Transmitter Port
#ifndef SBOXNET_TxD_PORT
# error "SBOXNET_TxD_PORT not defined!"
#endif
// Bitnummer des TxD Bits
#ifndef SBOXNET_TxD_BIT
# error "SBOXNET_TxD_BIT not defined!"
#endif
// wo die Geräteadresse abgelegt werden soll
#ifndef SBOXNET_DEVICE_ADDR
# error "SBOXNET_DEVICE_ADDR not defined!"
#endif
#ifdef __AVR_XMEGA__
// Timer Periode
# ifndef SBOXNET_TC_PERIOD
#  error "SBOXNET_TC_PERIOD not defined!"
# endif
// Timer Vorteiler
# ifndef SBOXNET_TC_PRESCALER
#  error "SBOXNET_TC_PERIOD not defined!"
# endif
// Receiver Interrupt Level
# ifndef SBOXNET_USART_RXCINTLVL
#  error "SBOXNET_USART_RXCINTLVL not defined!"
# endif
// Transmitter Interrupt Level
# ifndef SBOXNET_USART_TXCINTLVL
#  error "SBOXNET_USART_TXCINTLVL not defined!"
# endif
// welcher Timer wird verwendet
# ifndef SBOXNET_TIMER
#  error "SBOXNET_TIMER not defined!"
# endif
// welche CC Einheit im Timer
# ifndef SBOXNET_TIMER_CC
#  error "SBOXNET_TIMER_CC not defined!"
# endif
// welcher CC Buffer im Timer
# ifndef SBOXNET_TIMER_CCBUF
#  error "SBOXNET_TIMER_CCBUF not defined!"
# endif
//  Timer CC Interrupt Flag
# ifndef SBOXNET_TIMER_CCIF
#  error "SBOXNET_TIMER_CCIF not defined!"
# endif
// die Timer CC Interrupt Maske
# ifndef SBOXNET_TIMER_CCINTLVL_MASK
#  error "SBOXNET_TIMER_CCINTLVL_MASK not defined!"
# endif
// der Timer CC Interrupt Level
# ifndef SBOXNET_TIMER_CCINTLVL
#  error "SBOXNET_TIMER_CCINTLVL not defined!"
# endif
#else // !__AVR_XMEGA__
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
// wieviel Zeit soll vergehen
#ifndef SBOXNET_TIMER_BIT_TICKS
# error "SBOXNET_TIMER_BIT_TICKS not defined!"
#endif
// Maske für Periode
#ifndef SBOXNET_TIMER_PERIOD_MASK
# error "SBOXNET_TIMER_PERIOD_MASK not defined!"
#endif
// maske für Zufallswert
#ifndef SBOXNET_TIMER_RANDOM_MASK
# error "SBOXNET_TIMER_RANDOM_MASK not defined!"
#endif
// USART Kennung
#ifndef SBOXNET_USART_ID
# error "SBOXNET_USART_ID not defined!"
#endif
// Funktion die den Random Seed bereitstellt
#ifndef SBOXNET_PRNG_SEED
# error "SBOXNET_PRNG_SEED not defined!"
#endif
#ifdef __AVR_XMEGA__
// für ATXMEGA
// Hardware Sensor Interrupt Level Maske
# ifndef SBOXNET_HW_SENSOR_INTLVL_MASK
#  error "SBOXNET_HW_SENSOR_INTLVL_MASK not defined!"
# endif
// Hardware Sensor Interrupt Maske
# ifndef SBOXNET_HW_SENSOR_INTMASK
#  error "SBOXNET_HW_SENSOR_INTMASK not defined!"
# endif
// Hardware Sensor Interrupt Flag Bit
# ifndef SBOXNET_HW_SENSOR_INTF_BIT
#  error "SBOXNET_HW_SENSOR_INTF_BIT not defined!"
# endif
#else // !__AVR_XMEGA__
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


/*
 * transmit over CAN Bus shares one line.
 * so transmit byte == receive byte.
 */


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


// Sboxnet CRC Funktionen ---

// CRC Initialiseren
SBOXNET_SECTION static INLINE uint8_t sboxnet_crc8_init(void) {
    return 0xff;
}

// ein Byte zur CRC hinzufügen:
// siehe https://www.nongnu.org/avr-libc/user-manual/group__util__crc.html
SBOXNET_SECTION static uint8_t sboxnet_crc8_add(uint8_t crc, uint8_t v) {
    return _crc_ibutton_update(crc, v);
}

// prüft ob CRC OK berechnet wurde 
SBOXNET_SECTION static INLINE uint8_t sboxnet_crc8_ok(uint8_t crc) {
    return crc == 0x00;
}

/* 
 * Sboxnet Random Funktion.
 * eine Xor Shift Funktion für das Bilden einer Pseudo Zufallszahl,
 */
SBOXNET_SECTION static uint8_t sboxnet_prandom(void) {
    SBOXNET_VAR->prng_seed = prng_xorshift8(SBOXNET_VAR->prng_seed);
    return SBOXNET_VAR->prng_seed;
}


/* Sboxnet Ringbuffer
 * Ringbuffer Init.
 * sboxnet_rb_init(struct sboxnet_ring_buffer* rb, uint8_t size)
 *  rb Zeiger auf Ringbuffer
 *  size Grösse des Ringbuffers
 */
SBOXNET_SECTION static void sboxnet_rb_init(struct sboxnet_ring_buffer* rb, uint8_t size) {
    rb->rd = 0; // Index auf Lesedaten
    rb->rd_count = 0; // Leseanzahl
    rb->rd_rollback = 0; // Rollback Leseindex
    rb->wr = 0; // Index auf Schreibdaten
    rb->wr_count = 0; // Schreibanzahl
    rb->wr_rollback = 0; // Rollback Schreibindex
    rb->count = 0; // Gesammte Bytes im Buffer
    rb->size = size; // Grösse des Buffers
}

/* Ermittle Leseanzahl.
 */
SBOXNET_SECTION static INLINE uint8_t sboxnet_rb_getcount(struct sboxnet_ring_buffer* rb) {
    return rb->count;
}
/* Wieviel ist noch frei im Lesebuffer?
 */
SBOXNET_SECTION static INLINE uint8_t sboxnet_rb_getfree(struct sboxnet_ring_buffer* rb) {
    return rb->size - rb->count - rb->wr_count;
}

/* Schreibdaten Commiten.
 */
SBOXNET_SECTION static INLINE void sboxnet_rb_write_commit(struct sboxnet_ring_buffer* rb) {
#if SBOXNET_DEBUG_ENABLED != 0
    // sind zu viele Schreibdaten im Buffer?
    if ((rb->count + rb->wr_count) > rb->size) {
        setbit(SBOXNET_VAR->flags, SBOXNET_FLAG_WBUFOV_b);          
    }
#endif
	// Schreibdatenanzahl um wr_count erhöhen
    rb->count += rb->wr_count;
	// keine Schreibdaten mehr 
    rb->wr_count = 0;
	// Roolback Index setzen
    rb->wr_rollback = rb->wr;
}

/* Führe einen Rollback der Schreibdaten aus.
 */
SBOXNET_SECTION static void sboxnet_rb_write_rollback(struct sboxnet_ring_buffer* rb) {
    rb->wr_count = 0;
    rb->wr = rb->wr_rollback;
}

/* ein Byte in den Rinbuffer schreiben.
 */
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

/* Ein Byte als erstes in den Rinbuffer einfügen.
 */
SBOXNET_SECTION static void sboxnet_rb_put_first(struct sboxnet_ring_buffer* rb, uint8_t v) {
    rb->buf[rb->wr_rollback] = v;
}

/* Schreibanzahl im Ringbuffer.
 */
SBOXNET_SECTION static INLINE uint8_t sboxnet_rb_write_count(struct sboxnet_ring_buffer* rb) {
    return rb->wr_count;
}

/* Lese Commit.
 */ 
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

/* Lese Rollback.
 */
SBOXNET_SECTION static void sboxnet_rb_read_rollback(struct sboxnet_ring_buffer* rb) {
    rb->rd_count = 0;
    rb->rd = rb->rd_rollback;
}

/* Ein Byte aus dem Ringbuffer lesen.
 */
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

/* Ein Byte beim aktuellen Leseindex lesen.
 */
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

/* Ist was da zum lesen?
 */
SBOXNET_SECTION static INLINE uint8_t sboxnet_rb_can_read_bytes(struct sboxnet_ring_buffer* rb) {
    return rb->count - rb->rd_count;
}

/* static void sboxnet_receiver_enable(void)
 *
 * Sboxnet Empfänger einschalten.
 * MED Interrupt Level für Empfänger und Empfänger ein und Multiprocessor Modus ein.
 */
SBOXNET_SECTION NOINLINE __ATTR_USED static void sboxnet_receiver_enable(void) {
    // set RXC Intlevel to SBOXNET_USART_RXCINTLVL
    SBOXNET_USART_ID.CTRLA = (USARTC0.CTRLA & ~USART_RXCINTLVL_gm) | USART_RXCINTLVL_MED_gc;
    // enable receiver and set mcpm 
    setbv(SBOXNET_USART_ID.CTRLB, USART_RXEN_bm|USART_MPCM_bm);
}

// Sboxnet init
/* static void sboxnet_init(void)
 *
 * Sboxnet initialieren für SBOXNET_TIMER und USART SBOXNET_USART_ID über
 * SBOXNET_RxD_PORT.SBOXNET_RxD_BIT und SBOXNET_TxD_PORT.SBOXNET_TxD_BIT.
 * Benutzt den USART um Nachrichten zu empfangen und versenden im
 * Multiprozessor Modus.
 * Benutzt SBOXNET_RxD_PORT.SBOXNET_RxD_BIT Interrupt um Änderungen zu
 * erfassen.
 */
SBOXNET_SECTION NOINLINE __ATTR_USED static void sboxnet_init(void) {
    // sboxnet global variable of type struct sboxnet_device
    struct sboxnet_device *dev = SBOXNET_VAR;
    // clear sboxnet variable
    memset(dev, 0, sizeof(struct sboxnet_device));
    // init state
    dev->state = SBOXNET_STATE_NONE;
    // init pseudo random generator
    dev->prng_seed = SBOXNET_PRNG_SEED;
    // tmit retry counter
    dev->retry_counter = SBOXNET_TMIT_RETRY;
    
    // Init receive buffer
    sboxnet_rb_init(&dev->recv_buf.rb, sizeof(dev->recv_buf.buf));
    // init transmit buffer
    sboxnet_rb_init(&dev->tmit_buf.rb, sizeof(dev->tmit_buf.buf));
    
    // dir of port RxD IN
    port_dirin(PORTE, Bit(SBOXNET_RxD_BIT));
    // set RxD pin to input, pullup on
    port_pullup_on(PORTE, Bit(SBOXNET_RxD_BIT)); 
    
    // set TxD pin to output and value "1" (MARK)
    port_setbit(PORTE, SBOXNET_TxD_BIT);
    port_dirout(PORTE, Bit(SBOXNET_TxD_BIT));

//#define SBOXNET_TC_PRESCALER TC_CLKSEL_DIV8_gc // 32MHz / 8 = 4MHz
//#define SBOXNET_TC_PERIOD    (16384-1) // 0x3fff = 32MHz / 8 / 16384 = 244.14 Hz
    // Sboxnet Timer Normal Modus
    SBOXNET_TIMER.CTRLB = TC_WGMODE_NORMAL_gc;
	// Keine Eventaktion
    SBOXNET_TIMER.CTRLD = 0;
	// kein Byte Modus
    SBOXNET_TIMER.CTRLE = 0;
	// OV Interrupt Level MED
    SBOXNET_TIMER.INTCTRLA = TC_OVFINTLVL_MED_gc;
	// keine CC Interrupts
    SBOXNET_TIMER.INTCTRLB = 0;
	// Interrupt Flags löschen
    SBOXNET_TIMER.INTFLAGS = 0xff;
	// Periode setzen
	//(16384-1) 0x3fff = 32MHz / 8 / 16384 = 244.14 Hz
    SBOXNET_TIMER.PER = SBOXNET_TC_PERIOD;
	// Vorteiler einstellen
	// SBOXNET Timer Prescaler: 32Mhz / 8
    SBOXNET_TIMER.CTRLA = SBOXNET_TC_PRESCALER;
    
    // Init Interrupts

    // PORTE.INT0 is hardware sensor interrupt
    // clear io bin hardware sensor interrupt level
    SBOXNET_HW_SENSOR_PORT.INTCTRL &= ~PORT_INT0LVL_gm;
    // set io bin hardware sensor interrupt mask, pin 2 receive pin, INT 0
    SBOXNET_HW_SENSOR_PORT.INT0MASK = Bit(SBOXNET_RxD_BIT);
    // clear 0 interrupt flag
    SBOXNET_HW_SENSOR_PORT.INTFLAGS = Bit(PORT_INT0IF_bp);
    
    // USART
    // Setup baudrate generator with scale 0
    // upper 4bits of high byte
    SBOXNET_USART_ID.BAUDCTRLB = highbyte(SBOXNET_UBRR_VAL) & 0x0f;
    // lower byte of 16 bit
    SBOXNET_USART_ID.BAUDCTRLA = lowbyte(SBOXNET_UBRR_VAL);
    
    // clear transmit flag
    SBOXNET_USART_ID.STATUS = Bit(USART_TXCIF_bp); // clear TXCIF
    // clear receiver and transmitter interrupts, clear data register empty interrupt
    SBOXNET_USART_ID.CTRLA = USART_RXCINTLVL_OFF_gc|USART_TXCINTLVL_OFF_gc|USART_DREINTLVL_OFF_gc;
    // disable receiver and transmitter, normal speed, MPCM to 1
    SBOXNET_USART_ID.CTRLB = Bsv(USART_RXEN_bp,0)|Bsv(USART_TXEN_bp,0)|Bsv(USART_CLK2X_bp,0)|Bsv(USART_MPCM_bp,1);
    // init Usart: asyncron, even parity, 1 stop bit, 9 bits
    SBOXNET_USART_ID.CTRLC = USART_CMODE_ASYNCHRONOUS_gc|USART_PMODE_EVEN_gc|Bsv(USART_SBMODE_bp,0)|USART_CHSIZE_9BIT_gc;
	// Sender einschalten
    setbit(SBOXNET_USART_ID.CTRLB, USART_TXEN_bp);
    // Empfänger einschalten
    sboxnet_receiver_enable();
}

/* static void sboxnet_set_timer(uint16_t offset)
 *  offset Offset von SBOXNET_TIMER.CNT ab.
 *
 * Sboxnet setzt Timer für Timeouts.
 */
#ifndef ICEDBG
SBOXNET_SECTION static void sboxnet_set_timer(uint16_t offset) {
#else
SBOXNET_SECTION void sboxnet_set_timer(uint16_t offset) {
#endif
    register uint16_t ocr;
    // sichere aktuellen wert von SBOXNET_TIMER.CNT
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ocr = SBOXNET_TIMER.CNT; // 16bit register access !
		// timeout: ocr = (ocr + offset) & 0x3fff
		ocr = (ocr + offset) & SBOXNET_TIMER_PERIOD_MASK;
		// setze neuen CCA Wert über den CCA Buffer: SBOXNET_TIMER.CCABUF = ocr
        SBOXNET_TIMER.SBOXNET_TIMER_CCBUF = ocr;
		// Timer CC Interrupt Flag löschen: SBOXNET_TIMER.INFLAGS = TC0_CCAIF_bp
		SBOXNET_TIMER.INTFLAGS = Bit(SBOXNET_TIMER_CCIF);
		// Timer Interrupt Level setzen:  SBOXNET_TIMER.INTCTRLB = (SBOXNET_TIMER.INTCTRLB & ~TC0_CCAINTLVL_gm) | TC_CCAINTLVL_MED_gc
		SBOXNET_TIMER.INTCTRLB = (SBOXNET_TIMER.INTCTRLB & ~SBOXNET_TIMER_CCINTLVL_MASK) | SBOXNET_TIMER_CCINTLVL;
	}
}

/* static inline void sboxnet_clear_line_sensor(void)
 * Hardware Sensor zurücksetzen.
 */
SBOXNET_SECTION static inline void sboxnet_clear_line_sensor(void) {
	// Hardware Sensor Interrupt Flag löschen
    SBOXNET_HW_SENSOR_PORT.INTFLAGS = Bit(SBOXNET_HW_SENSOR_INTF_BIT);
}

/* static inline uint8_t sboxnet_line_sensor_changed(void)
 * 
 * Teste ob der Hardware Sensor eine Veränderung festgestellt hat.
 * Gibt war zurück wenn ja.
 */
SBOXNET_SECTION static inline uint8_t sboxnet_line_sensor_changed(void) {
	// Teste ob Interrupt Flag gesetzt ist
    return bit_is_set(SBOXNET_HW_SENSOR_PORT.INTFLAGS, SBOXNET_HW_SENSOR_INTF_BIT);
}

/* static void sboxnet_set_backoff_with_offset(uint16_t offset)
 *  Eine Backoff Zeit setzen für Senden/Kollisonsvermeidung/-erkennung.
 */
#ifndef ICEDBG
SBOXNET_SECTION static void sboxnet_set_backoff_with_offset(uint16_t offset) {
#else
SBOXNET_SECTION void sboxnet_set_backoff_with_offset(uint16_t offset) {
#endif
    // Hardware Sensor zurücksetzen
    sboxnet_clear_line_sensor();
    // Status auf SBOXNET_STATE_BACKOFF
    SBOXNET_VAR->state = SBOXNET_STATE_BACKOFF;

    // Timer Offset setzen: (32 * 15)+ offset = 480 + offset
    sboxnet_set_timer(SBOXNET_TIMER_BIT_TICKS * SBOXNET_BACKOFF_BITS + offset);
}

/* static void sboxnet_intr_usart_rx(void)
 * Sboxnet Empfänger Interrupt.
 */
#ifndef ICEDBG
SBOXNET_SECTION static void sboxnet_intr_usart_rx(void) {
#else
SBOXNET_SECTION void sboxnet_intr_usart_rx(void) {
#endif
	// MPCPM Bit, Empfangens Byte, Flags
    uint8_t rxb8, rxb, rflags;
	// Zeiger auf Sboxnet Device
    struct sboxnet_device *dev = SBOXNET_VAR;
	// Muss ein Zeiger sein
    FORCEPTR(dev);
    
	// USART Status
    rflags = SBOXNET_USART_ID.STATUS;
	// Ist RXB8 im Status gesetzt
    rxb8 = bit_is_set(rflags, USART_RXB8_bp);
	// die empfangenen Daten
    rxb = SBOXNET_USART_ID.DATA;
	// USART Fehlerflags (Frame Error, Buffer Overflow, Paritätsfehler
    rflags &= (USART_FERR_bm|USART_BUFOVF_bm|USART_PERR_bm);
	// Ist eins der Fehler Bits gesetzt?
    if (rflags) {
		// dann für Debugzwecke merken
        SBOXNET_DEBUG(sboxnet_debug_inc_recverrors();)
		// pro Fehler merken
        SBOXNET_DEBUG(if (rflags & USART_FERR_bm)   { dev->debug.recverr_fe++; } )
        SBOXNET_DEBUG(if (rflags & USART_BUFOVF_bm) { sboxnet_debug_inc_recverr_dor(); } )
        SBOXNET_DEBUG(if (rflags & USART_PERR_bm)   { dev->debug.recverr_pe++; } )
		// Empfänger Zurücksetzen
        goto receive_reset;
        
    } else if (rxb8) { // wenn 8. Bit gesetzt ist es ein Adressbyte
        if (sboxnet_rb_write_count(&dev->recv_buf.rb) != 0) {	// OH, da ist schon was im Empfangsbuffer
            SBOXNET_DEBUG(dev->debug.recverr_proto++;)			// Muss ein Protokollfehler sein, 
            SBOXNET_DEBUG(sboxnet_debug_inc_recverrors();)
            sboxnet_rb_write_rollback(&dev->recv_buf.rb);		// entfernen
        }
		// ist die Adressbyte die Geräteadresse oder die Broadcast Adresse?
        if (rxb == SBOXNET_DEVICE_ADDR || rxb == SBOXNET_ADDR_BROADCAST
#if SBOXNET_USE_SNIFFER == 1
            || bit_is_set(dev->flags, SBOXNET_FLAG_SNIFFER_b)
#endif
        ) { // dann ist die Nachricht für dieses Gerät
			// MPCMP löschen
            clrbit(SBOXNET_USART_ID.CTRLB, USART_MPCM_bp);
			// Platzhalter für Nachrichtenlänge
            if (!sboxnet_rb_write(&dev->recv_buf.rb, 0)) {
                goto rec_dor;
            }
			// erstes Datenbyte
            if (sboxnet_rb_write(&dev->recv_buf.rb, rxb)) {
                SBOXNET_DEBUG(sboxnet_debug_inc_recv_byte();)
				// Empfängerlänge erstmal auf die mindest Nachrichtenlänge
                dev->recv_len = SBOXNET_MSG_MIN_LEN;
            } else {
				// wenn nicht OK, DATA OVERRUN
                rec_dor:
                SBOXNET_DEBUG(sboxnet_debug_inc_recverr_dor();)
                goto receive_error;
            }
        } else {
			// warte auf nächstes Adress Byte
            goto receive_reset;
        }
        
    } else {
		// Daten Byte
		
		// weiviele Bytes sind im Schreibringbuffer?
        uint8_t writecount = sboxnet_rb_write_count(&dev->recv_buf.rb);
		// ist die Schreibanzahl > Empfange Bytes Anzahl?
        if (writecount > dev->recv_len) { // Hinweis: writecount+1 !  erstes Byte im Buffer ist die Nachrichtenlänge!
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
                setbit(SBOXNET_USART_ID.CTRLB, USART_MPCM_bp);
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
    setbit(SBOXNET_USART_ID.CTRLB, USART_MPCM_bp);

    sboxnet_rb_write_rollback(&dev->recv_buf.rb);
}

// must be called with interrupts off !
#ifndef ICEDBG
SBOXNET_SECTION static void sboxnet_check_try_transmit(void) {
#else
SBOXNET_SECTION void sboxnet_check_try_transmit(void) {
#endif
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

#ifndef ICEDBG
SBOXNET_SECTION static void sboxnet_intr_usart_tx(void) {
#else
SBOXNET_SECTION void sboxnet_intr_usart_tx(void) {
#endif
    struct sboxnet_device *dev = SBOXNET_VAR;
    FORCEPTR(dev);
    
    // collision check
    uint8_t ucsra = SBOXNET_USART_ID.STATUS;
    uint8_t crxc = !(ucsra & USART_RXCIF_bm); // echo receive not complete
    uint8_t cfe  = (ucsra & (USART_FERR_bm|USART_PERR_bm)); // receive frame or parity error
    uint8_t cudr = (SBOXNET_USART_ID.DATA != dev->tmit_lastbyte); // echo byte != transmit byte
    if (crxc || cfe || cudr) {
        SBOXNET_DEBUG(if (crxc) { dev->debug.coll_rxc++; } )
        SBOXNET_DEBUG(if (cfe)  { dev->debug.coll_fe++; } )
        SBOXNET_DEBUG(if (cudr) { dev->debug.coll_udr++; } )
        
        port_clrbit(SBOXNET_TxD_PORT, SBOXNET_TxD_BIT); // send a 0 to the line

        //SBOXNET_USART_ID.CTRLB &= ~(USART_TXEN_bm|USART_RXEN_bm); // and disable transmitter and receiver
        //SBOXNET_USART_ID.CTRLA &= ~(USART_TXCINTLVL_gm);

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

    clrbit(SBOXNET_USART_ID.CTRLB, USART_RXEN_bp);
    SBOXNET_USART_ID.CTRLA &= ~(USART_RXCINTLVL_gm);
    setbit(SBOXNET_USART_ID.CTRLB, USART_RXEN_bp);    

    
    if (dev->tmit_cnt < dev->tmit_bytes) {
        uint8_t v = sboxnet_rb_read(&dev->tmit_buf.rb);
        dev->tmit_cnt++;
        dev->tmit_lastbyte = v;

        clrbit(SBOXNET_USART_ID.CTRLB, USART_TXB8_bp);
        SBOXNET_USART_ID.DATA = v;
    } else {
        SBOXNET_USART_ID.CTRLA &= ~(USART_TXCINTLVL_gm);

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

SBOXNET_SECTION ISR(SBOXNET_USART_RxC_vect) {
    sboxnet_intr_usart_rx();
}

SBOXNET_SECTION ISR(SBOXNET_USART_TxC_vect) {
    sboxnet_intr_usart_tx();
}
#ifndef ICEDBG
SBOXNET_SECTION static void sboxnet_timer_bit(void) { // must be run with interrupts off !
#else
SBOXNET_SECTION void sboxnet_timer_bit(void) { // must be run with interrupts off !
#endif
    struct sboxnet_device *dev = SBOXNET_VAR;
    //FORCEPTR(dev);
    SBOXNET_TIMER.INTCTRLB &= ~SBOXNET_TIMER_CCINTLVL_MASK;


	volatile uint8_t st = dev->state;
	
    if (dev->state == SBOXNET_STATE_BACKOFF) {
        // if backoff
        if (dev->tmit_bytes > 0 && dev->tmit_cnt == 0) {
            // es dinf noch bytes im buffer und tmit count == 0
            
            // hole nächstes byte aus tmitbuffer
            uint8_t d = sboxnet_rb_read_getone(&dev->tmit_buf.rb);
            
            // setze tmit line High
            port_setbit(SBOXNET_TxD_PORT, SBOXNET_TxD_BIT);
            // set bit 8 => marks address byte in Multiprocessor Mode, marks address byte
            setbv(SBOXNET_USART_ID.CTRLB, USART_TXEN_bm|USART_TXB8_bm); // enable transmitter, and set txb8
           
            if (sboxnet_line_sensor_changed() || bit_is_clear(port_in(SBOXNET_RxD_PORT), SBOXNET_RxD_BIT)) {
                uint16_t offset = sboxnet_prandom() & SBOXNET_TIMER_RANDOM_MASK;
                sboxnet_set_backoff_with_offset((uint16_t)dev->retry_counter * (SBOXNET_TIMER_BIT_TICKS / 4) + offset);
                
            } else {

                clrbit(SBOXNET_USART_ID.CTRLB, USART_RXEN_bp);
                SBOXNET_USART_ID.CTRLA &= ~(USART_RXCINTLVL_gm|USART_TXCINTLVL_gm);
                setbit(SBOXNET_USART_ID.CTRLB, USART_RXEN_bp);
                SBOXNET_USART_ID.CTRLA |= SBOXNET_USART_TXCINTLVL;
                SBOXNET_USART_ID.DATA = d;
                clrbit(SBOXNET_USART_ID.CTRLB, USART_MPCM_bp);

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

SBOXNET_SECTION __ATTR_USED NOINLINE uint8_t sboxnet_send_msg(struct sboxnet_msg_header* pmsg) {
    // sboxnet device
    struct sboxnet_device *dev = SBOXNET_VAR;
    // msglen mind SBOXNET_MSG_MIN_LEN + pmsg->opt.len
    uint8_t msglen = SBOXNET_MSG_MIN_LEN + pmsg->opt.len;
    uint8_t free;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        // freie bytes im TMIT Buffer
        free = sboxnet_rb_getfree(&dev->tmit_buf.rb);
    }
    // ist genug frei?
    uint8_t RC = 0;
    if (!(free > msglen)) {
        // nicht genug frei im TMIT Buffer
        g_dev_errflags |= DEV_ERR_FLG_TMITNOTFREE;
        RC = DEV_ERR_FLG_TMITNOTFREE;
        return SBOXNET_ERR_TMIT_NOTFREE; // nein
    }
    uint8_t rc;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        // test if enoogh free for msg in tmit buf
        rc = sboxnet_rb_write(&dev->tmit_buf.rb, msglen);
    }
    if (!rc) {
        RC = DEV_ERR_FLG_TMITNOTENOUGBUF;
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
            // write 1 byte to tmit ringbuffer
            rc = sboxnet_rb_write(&dev->tmit_buf.rb, d);
        }
        if (!rc) {
            RC = DEV_ERR_FLG_TMITCANNOTPUTINTORINGBUF;
            goto send_error;
        }
    }
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        // last byte: write crc to ringbuffer
        rc = sboxnet_rb_write(&dev->tmit_buf.rb, crc);
    }
    if (!rc) {
        send_error:
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            sboxnet_rb_write_rollback(&dev->tmit_buf.rb);
        }
        return RC;
    }
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        sboxnet_rb_write_commit(&dev->tmit_buf.rb);
        sboxnet_check_try_transmit();
    }
    SBOXNET_DEBUG(SBOXNET_VAR->debug.tmit_msg_tosend++;)
    return 0;
}

SBOXNET_SECTION2 NOINLINE uint8_t sboxnet_can_send_msg(struct sboxnet_msg_header* pmsg) {
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



#if 0
// -------------------------------------------------------------
/* static void sboxnet_receiver_enable(void)
 * 
 * Enable the sboxnet receiver
 */
#ifndef ICEDBG
SBOXNET_SECTION NOINLINE __ATTR_USED void sboxnet_receiver_enable(void) {
#else
SBOXNET_SECTION NOINLINE __ATTR_USED void sboxnet_receiver_enable(void) {
#endif
#ifdef __AVR_XMEGA__
    // set RXC Intlevel to SBOXNET_USART_RXCINTLVL
    SBOXNET_USART_ID.CTRLA = (SBOXNET_USART_ID.CTRLA & ~USART_RXCINTLVL_gm) | SBOXNET_USART_RXCINTLVL;
    // enable receiver and set mcpm 
    setbv(SBOXNET_USART_ID.CTRLB, USART_RXEN_bm|USART_MPCM_bm);
#else
    // set MPXM bit
    setbit(__UCSRA, __MPCM);
    // enable receiver and set receiver IE
    __UCSRB |= (Bit(__RXEN)|Bit(__RXCIE));
#endif
}

// Sboxnet init
/* static void sboxnet_init(void)
 * 
 * Init the sboxnet commuction bus over USART SBOXNET_USART_ID.
 * Uses the USART to receive and transmit sboxnet msgs in multiprocessor mode.
 * Used SBOXNET_RxD_PORT port interrupts to detect changes.
 */
SBOXNET_SECTION NOINLINE __ATTR_USED static void sboxnet_init(void) {
    // sboxnet global variable of type struct sboxnet_device
    struct sboxnet_device *dev = SBOXNET_VAR;
    // clear sboxnet variable
    memset(dev, 0, sizeof(struct sboxnet_device));
    // init state
    dev->state = SBOXNET_STATE_NONE;
    // init pseudo random generator
    dev->prng_seed = SBOXNET_PRNG_SEED;
    // tmit retry counter
    dev->retry_counter = SBOXNET_TMIT_RETRY;
    
    // Init receive buffer
    sboxnet_rb_init(&dev->recv_buf.rb, sizeof(dev->recv_buf.buf));
    // init transmit buffer
    sboxnet_rb_init(&dev->tmit_buf.rb, sizeof(dev->tmit_buf.buf));
    
    // dir of port RxD IN
    port_dirin(SBOXNET_RxD_PORT, Bit(SBOXNET_RxD_BIT));
    // set RxD pin to input, pullup on
    port_pullup_on(SBOXNET_RxD_PORT, Bit(SBOXNET_RxD_BIT)); 
    
    // set TxD pin to output and value "1" (MARK)
    port_setbit(SBOXNET_TxD_PORT, SBOXNET_TxD_BIT);
    port_dirout(SBOXNET_TxD_PORT, Bit(SBOXNET_TxD_BIT));
    
    // USART
#ifdef __AVR_XMEGA__
    // Init Interrupts
    
    // clear io bin hardware sensor interrupt level
    SBOXNET_RxD_PORT.INTCTRL &= ~SBOXNET_HW_SENSOR_INTLVL_MASK;
    // set io bin hardware sensor interrupt mask
    SBOXNET_RxD_PORT.SBOXNET_HW_SENSOR_INTMASK = Bit(SBOXNET_RxD_BIT);
    // clear interrupt flag
    SBOXNET_RxD_PORT.INTFLAGS = Bit(SBOXNET_HW_SENSOR_INTF_BIT);
    
    //setup baudrate generator with scale 0
    // upper 4bits of hidh byte
    SBOXNET_USART_ID.BAUDCTRLB = highbyte(SBOXNET_UBRR_VAL) & 0x0f;
    // lower byte of 16 bit
    SBOXNET_USART_ID.BAUDCTRLA = lowbyte(SBOXNET_UBRR_VAL);
    
    // clear transmit flag
    SBOXNET_USART_ID.STATUS = Bit(USART_TXCIF_bp); // clear TXCIF
    // clear receiver and transmitter interrupts, clear data register empty interrupt
    SBOXNET_USART_ID.CTRLA = USART_RXCINTLVL_OFF_gc|USART_TXCINTLVL_OFF_gc|USART_DREINTLVL_OFF_gc;
    // disable receiver and transmitter, normal speed, MPCM to 1
    SBOXNET_USART_ID.CTRLB = Bsv(USART_RXEN_bp,0)|Bsv(USART_TXEN_bp,0)|Bsv(USART_CLK2X_bp,0)|Bsv(USART_MPCM_bp,1);
    // init Usart: asyncron, even parity, 1 stop bit, 9 bits
    SBOXNET_USART_ID.CTRLC = USART_CMODE_ASYNCHRONOUS_gc|USART_PMODE_EVEN_gc|Bsv(USART_SBMODE_bp,0)|USART_CHSIZE_9BIT_gc;
    
#else // __AVR_XMEGA__
    // line hw sensor
#if SBOXNET_USE_HW_SENSOR_PINCH == 1
    // if not XMega
    //
    clrbit(SBOXNET_HW_SENSOR_PCICR, SBOXNET_HW_SENSOR_PCIE_BIT);
    SBOXNET_HW_SENSOR_PCMSK = Bit(SBOXNET_HW_SENSOR_PCINT_BIT);
    SBOXNET_HW_SENSOR_PCIFR = Bit(SBOXNET_HW_SENSOR_PCIF_BIT);
#else
    clrbit(SBOXNET_HW_SENSOR_EIMSK, SBOXNET_HW_SENSOR_IE_BIT); // disable interrupt
    clrbit(SBOXNET_HW_SENSOR_EICR, SBOXNET_HW_SENSOR_ISC1); // any edge of INT
    setbit(SBOXNET_HW_SENSOR_EICR, SBOXNET_HW_SENSOR_ISC0);
    SBOXNET_HW_SENSOR_EIFR = Bit(SBOXNET_HW_SENSOR_IF_BIT); // reset flag
#endif
    // set baud rate
    __UBRRH = highbyte(SBOXNET_UBRR_VAL) & 0x7f; // Bit URSEL=0
    __UBRRL = lowbyte(SBOXNET_UBRR_VAL);
    // clear TXC, normal speed
    __UCSRA = Bsv(__TXC,1)|Bsv(__U2X,0)|Bsv(__MPCM,0); 
    
    __UCSRC = 
#ifdef __URSEL
            Bsv(__URSEL,1)|
#endif
            Bsv(__UMSEL0,0)|Bsv(__UPM1,1)|Bsv(__UPM0,0)|Bsv(__USBS,0)|Bsv(__UCSZ1,1)|Bsv(__UCSZ0,1); // async, even parity, 1 stop bit, 9 chars

    __UCSRB = Bsv(__RXCIE,0)|Bsv(__TXCIE,0)|Bsv(__UDRSBOXNET_USART_IDIE,0)|Bsv(__RXEN,0)|Bsv(__TXEN,0)|Bsv(__UCSZ2,1)|Bsv(__TXB8,0); // 9 chars
#endif // __AVR_XMEGA__
    
    // enable receiver
    sboxnet_receiver_enable();
}

// Sboxnet deinit
/* static void sboxnet_deinit(void)
 * 
 * Deinit sboxnet.
 */
/*
SBOXNET_SECTION __ATTR_NO_UNUSED_WARN 
static void sboxnet_deinit(void) {
    // atomic block
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        //set state to SBOXNET_STATE_NONE
        SBOXNET_VAR->state = SBOXNET_STATE_NONE;
#ifdef __AVR_XMEGA__
        // if XMEGA:
        // disable receiver and transmitter, Double Transmission Speed to normal, disable MPCM 
        SBOXNET_USART_ID.CTRLB = Bsv(USART_RXEN_bp,0)|Bsv(USART_TXEN_bp,0)|Bsv(USART_CLK2X_bp,0)|Bsv(USART_MPCM_bp,0);
        // interupts off
        SBOXNET_USART_ID.CTRLA = USART_RXCINTLVL_OFF_gc|USART_TXCINTLVL_OFF_gc|USART_DREINTLVL_OFF_gc;
#else // __AVR_XMEGA__
        __UCSRB = Bsv(__RXCIE,0)|Bsv(__TXCIE,0)|Bsv(__UDRIE,0)|Bsv(__RXEN,0)|Bsv(__TXEN,0)|Bsv(__UCSZ2,1)|Bsv(__TXB8,0); // 9 chars
#endif // __AVR_XMEGA__
        // ports to input
        port_dirin(SBOXNET_RxD_PORT, Bit(SBOXNET_RxD_BIT));
        port_dirin(SBOXNET_TxD_PORT, Bit(SBOXNET_TxD_BIT));
        // and TxD Pullup on
        port_pullup_on(SBOXNET_TxD_PORT, Bit(SBOXNET_TxD_BIT));
#ifdef __AVR_XMEGA__
        // clear inerrupt flags: SBOXNET_HW_SENSOR_INTF_BIT
        SBOXNET_RxD_PORT.INTFLAGS = Bit(SBOXNET_HW_SENSOR_INTF_BIT);
        // hw sensor INTMASK = 0
        SBOXNET_RxD_PORT.SBOXNET_HW_SENSOR_INTMASK = 0;
#else
#if SBOXNET_USE_HW_SENSOR_PINCH == 1
        clrbit(SBOXNET_HW_SENSOR_PCMSK, SBOXNET_HW_SENSOR_PCINT_BIT);        
#else
        clrbit(SBOXNET_HW_SENSOR_EIMSK, SBOXNET_HW_SENSOR_IE_BIT); // disable interrupt
#endif
#endif
    }
}*/

/* static void sboxnet_set_timer(uint16_t offset)
 * 
 * sboxnet set timer for timeouts.
 */
#ifndef ICEDBG
SBOXNET_SECTION static void sboxnet_set_timer(uint16_t offset) {
#else
SBOXNET_SECTION void sboxnet_set_timer(uint16_t offset) {
#endif
#ifdef __AVR_XMEGA__
    register uint16_t ocr;
    // sichere aktuellen wert von SBOXNET_TIMER.CNT
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ocr = SBOXNET_TIMER.CNT; // 16bit register access !
    }
    // timeout: ocr = (ocr + offset) & 0x3fff
    ocr = (ocr + offset) & SBOXNET_TIMER_PERIOD_MASK;
    // set new timer value: SBOXNET_TIMER.CCA = ocr
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        // set timer
        SBOXNET_TIMER.SBOXNET_TIMER_CC = ocr;
    }
    // clear timer cc interrupt flag: SBOXNET_TIMER.INFLAGS = TC0_CCAIF_bp
    SBOXNET_TIMER.INTFLAGS = Bit(SBOXNET_TIMER_CCIF);
    // set timer interrupt level:  SBOXNET_TIMER.INTCTRLB = (SBOXNET_TIMER.INTCTRLB & ~TC0_CCAINTLVL_gm) | TC_CCAINTLVL_MED_gc
    SBOXNET_TIMER.INTCTRLB = (SBOXNET_TIMER.INTCTRLB & ~SBOXNET_TIMER_CCINTLVL_MASK) | SBOXNET_TIMER_CCINTLVL;
#else
    register uint16_t ocr = SBOXNET_TIMER_TCNT; // 16bit register access, we are here already in an interrupt
    SBOXNET_TIMER_OCR = (ocr + offset) & SBOXNET_TIMER_PERIOD_MASK;
    SBOXNET_TIMER_TIFR = Bit(SBOXNET_TIMER_OCF);
    setbit(SBOXNET_TIMER_TIMSK, SBOXNET_TIMER_OCIE);
#endif
}

/* static inline void sboxnet_clear_line_sensor(void)
 * 
 * clear HW sensor.
 */
#ifndef ICEDBG
SBOXNET_SECTION static inline void sboxnet_clear_line_sensor(void) {
#else
SBOXNET_SECTION void sboxnet_clear_line_sensor(void) {
#endif
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

/* static inline uint8_t sboxnet_line_sensor_changed(void)
 * 
 * check it hardware sensor has detected a change.
 * returns true if yes.
 */
#ifndef ICEDBG
SBOXNET_SECTION static inline uint8_t sboxnet_line_sensor_changed(void) {
#else
SBOXNET_SECTION uint8_t sboxnet_line_sensor_changed(void) {
#endif
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

/* static void sboxnet_set_backoff_with_offset(uint16_t offset)
 * 
 * set a backoff for transmissions.
 */
#ifndef ICEDBG
SBOXNET_SECTION static void sboxnet_set_backoff_with_offset(uint16_t offset) {
#else
SBOXNET_SECTION void sboxnet_set_backoff_with_offset(uint16_t offset) {
#endif
    // clear HW sensor 
    sboxnet_clear_line_sensor();
    // state to SBOXNET_STATE_BACKOFF
    SBOXNET_VAR->state = SBOXNET_STATE_BACKOFF;
    // sboxnet timer to SBOXNET_TIMER_BIT_TICKS * SBOXNET_TIMER_BIT_TICKS + offset
    // (32 * 15)+ offset = 480 + offset
    sboxnet_set_timer(SBOXNET_TIMER_BIT_TICKS * SBOXNET_BACKOFF_BITS + offset);
}

/* static void sboxnet_intr_usart_rx(void)
 * 
 * sboxnet USART RX interrupt
 */
#ifndef ICEDBG
SBOXNET_SECTION static void sboxnet_intr_usart_rx(void) {
#else
SBOXNET_SECTION void sboxnet_intr_usart_rx(void) {
#endif
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
#ifndef ICEDBG
SBOXNET_SECTION static void sboxnet_check_try_transmit(void) {
#else
SBOXNET_SECTION void sboxnet_check_try_transmit(void) {
#endif
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

#ifndef ICEDBG
SBOXNET_SECTION static void sboxnet_intr_usart_tx(void) {
#else
SBOXNET_SECTION static void sboxnet_intr_usart_tx(void) {
#endif
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

#ifndef ICEDBG 
SBOXNET_SECTION static void sboxnet_timer_bit(void) { // must be run with interrupts off !
#else
SBOXNET_SECTION void sboxnet_timer_bit(void) {
#endif
    struct sboxnet_device *dev = SBOXNET_VAR;
    //FORCEPTR(dev);
#ifdef __AVR_XMEGA__
    SBOXNET_TIMER.INTCTRLB &= ~SBOXNET_TIMER_CCINTLVL_MASK;
#else
    clrbit(SBOXNET_TIMER_TIMSK, SBOXNET_TIMER_OCIE);
#endif
    if (dev->state == SBOXNET_STATE_BACKOFF) {
        // if backoff
        if (dev->tmit_bytes > 0 && dev->tmit_cnt == 0) {
            // es dinf noch bytes im buffer und tmit count == 0
            
            // hole nächstes byte aus tmitbuffer
            uint8_t d = sboxnet_rb_read_getone(&dev->tmit_buf.rb);
            
            // setze tmit line High
            port_setbit(SBOXNET_TxD_PORT, SBOXNET_TxD_BIT);
            // set bit 8 => marks address byte in Multiprocessor Mode, marks address byte
#ifdef __AVR_XMEGA__
            setbv(SBOXNET_USART_ID.CTRLB, USART_TXEN_bm|USART_TXB8_bm); // enable transmitter, and set txb8
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

SBOXNET_SECTION __ATTR_USED NOINLINE uint8_t sboxnet_send_msg(struct sboxnet_msg_header* pmsg) {
    // sboxnet device
    struct sboxnet_device *dev = SBOXNET_VAR;
    // msglen mind SBOXNET_MSG_MIN_LEN + pmsg->opt.len
    uint8_t msglen = SBOXNET_MSG_MIN_LEN + pmsg->opt.len;
    uint8_t free;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        // freie bytes im TMIT Buffer
        free = sboxnet_rb_getfree(&dev->tmit_buf.rb);
    }
    // ist genug frei?
    uint8_t RC = 0;
    if (!(free > msglen)) {
        // nicht genug frei im TMIT Buffer
        g_dev_errflags |= DEV_ERR_FLG_TMITNOTFREE;
        RC = DEV_ERR_FLG_TMITNOTFREE;
        return SBOXNET_ERR_TMIT_NOTFREE; // nein
    }
    uint8_t rc;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        // test if enoogh free for msg in tmit buf
        rc = sboxnet_rb_write(&dev->tmit_buf.rb, msglen);
    }
    if (!rc) {
        RC = DEV_ERR_FLG_TMITNOTENOUGBUF;
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
            // write 1 byte to tmit ringbuffer
            rc = sboxnet_rb_write(&dev->tmit_buf.rb, d);
        }
        if (!rc) {
            RC = DEV_ERR_FLG_TMITCANNOTPUTINTORINGBUF;
            goto send_error;
        }
    }
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        // last byte: write crc to ringbuffer
        rc = sboxnet_rb_write(&dev->tmit_buf.rb, crc);
    }
    if (!rc) {
 send_error:
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            sboxnet_rb_write_rollback(&dev->tmit_buf.rb);
        }
        return RC;
    }
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        sboxnet_rb_write_commit(&dev->tmit_buf.rb);
        sboxnet_check_try_transmit();
    }
    SBOXNET_DEBUG(SBOXNET_VAR->debug.tmit_msg_tosend++;)
    return 0;
}

SBOXNET_SECTION2 NOINLINE uint8_t sboxnet_can_send_msg(struct sboxnet_msg_header* pmsg) {
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

#endif

