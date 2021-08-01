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



#ifndef _AVRUTILSLIB_SBOXNET_
#define _AVRUTILSLIB_SBOXNET_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Baudrate für Sboxnet Kommunikation
#define SBOXNET_BAUD_RATE  250000 // 4us
// Wert für Baudraten Generator; (32MHz / Prescaler / Baudrate) - 1
#define SBOXNET_UBRR_VAL   ((F_CPU / 16 / SBOXNET_BAUD_RATE) - 1)


#define SBOXNET_TMIT_RETRY 25 // wie oft sollen Sende Versuche unternommen werden? bei einer Kollision
//#define SBOXNET_TMIT_PRIO_MASTER  0
//#define SBOXNET_TMIT_PRIO_CLIENT  5
#define SBOXNET_COLLISION_BREAK_BITS 16 // wieviele Break Bits sollen bei einer Kollision gemacht werden?
#define SBOXNET_BACKOFF_BITS 15 // wieviele Bits sollen als Backoff genutzt werden
#define SBOXNET_COLLCHECK_BITS 4  // wieviele Bits als Collison check
    

#ifdef __cplusplus
}
#endif


#include "sboxnet-struct.h"


#ifdef __cplusplus
extern "C" {
#endif


#ifndef SBOXNET_RECV_BUF_SIZE
# define SBOXNET_RECV_BUF_SIZE (2*(SBOXNET_MSG_MAX_LEN+1))
#endif
#if SBOXNET_RECV_BUF_SIZE > 254
# error "SBOXNET_RECV_BUF_SIZE must be <= 254"
#endif
#ifndef SBOXNET_TMIT_BUF_SIZE
# define SBOXNET_TMIT_BUF_SIZE (2*(SBOXNET_MSG_MAX_LEN+1))
#endif
#if SBOXNET_TMIT_BUF_SIZE > 254
# error "SBOXNET_TMIT_BUF_SIZE must be <= 254"
#endif
//#define SBOXNET_DEBUG_ENABLED 1
#ifdef SBOXNET_DEBUG_ENABLED
# if SBOXNET_DEBUG_ENABLED != 0
#  define SBOXNET_DEBUG(_x) _x
# else
#  define SBOXNET_DEBUG(_x)
# endif
#else
# define SBOXNET_DEBUG(_x)
#endif

/* Sboxnet Ringbuffer
*/
struct sboxnet_ring_buffer {
    uint8_t rd;             // read index
    uint8_t rd_count;       // read count
    uint8_t rd_rollback;    // read rollback index
    uint8_t wr;             // write index
    uint8_t wr_count;       // write count
    uint8_t wr_rollback;    // write rollback index
    uint8_t count;          // count in ring buffer
    uint8_t size;           // size of ringbuffer
    uint8_t buf[0];         // buffer
};
// Ringbuffer Empfangen (Receive)
struct sboxnet_recv_ring_buffer {
    struct sboxnet_ring_buffer rb;
    uint8_t buf[SBOXNET_RECV_BUF_SIZE];
};
// Rinbuffer Senden (Transmit)
struct sboxnet_tmit_ring_buffer {
    struct sboxnet_ring_buffer rb;
    uint8_t buf[SBOXNET_TMIT_BUF_SIZE];
};

// Sboxnet Device
struct sboxnet_device {
    uint8_t state; // Status:
    uint8_t flags; // Flags:
    uint8_t retry_counter;
    uint8_t tmit_cnt;
    uint8_t tmit_bytes;
    uint8_t tmit_lastbyte;
    uint8_t backoff_bits;
    uint8_t recv_len;
    uint8_t prng_seed;
    struct sboxnet_recv_ring_buffer recv_buf;
    struct sboxnet_tmit_ring_buffer tmit_buf;
    
#if SBOXNET_DEBUG_ENABLED == 1
    struct sboxnet_debug_info debug;
#endif
};

#ifdef __cplusplus
}
#endif


#endif // _AVRUTILSLIB_SBOXNET_
