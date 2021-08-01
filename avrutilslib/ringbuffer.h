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


#ifndef _AVRUTILSLIB_RINGBUFFER_H
#define _AVRUTILSLIB_RINGBUFFER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ring_buffer {
    uint8_t rd;
    uint8_t wr;
    uint8_t size;
    uint8_t count;
    uint8_t buf[0];
};


void ringbuf_init(struct ring_buffer* a_rb, uint8_t a_size);

uint8_t ringbuf_read(struct ring_buffer* a_rb);

void ringbuf_write(struct ring_buffer* a_rb, uint8_t a_v);

static inline uint8_t ringbuf_isempty(struct ring_buffer* a_rb) {
    return a_rb->count == 0;
}

static inline uint8_t ringbuf_isfull(struct ring_buffer* a_rb) {
    return a_rb->count == a_rb->size;
}

static inline uint8_t ringbuf_getfree(struct ring_buffer* a_rb) {
    return a_rb->size - a_rb->count;
}

static inline uint8_t ringbuf_count(struct ring_buffer* a_rb) {
    return a_rb->count;
}

#ifdef __cplusplus
}
#endif

#endif /* _AVRUTILSLIB_RINGBUFFER_H */
