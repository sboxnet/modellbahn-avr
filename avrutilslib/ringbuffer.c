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

#include "avrutilslib/ringbuffer.h"


void ringbuf_init(struct ring_buffer* a_rb, uint8_t a_size) {
    a_rb->rd = 0;
    a_rb->wr = 0;
    a_rb->size = a_size;
    a_rb->count = 0;
}

uint8_t ringbuf_read(struct ring_buffer* a_rb) {
    if (a_rb->count > 0) {
        a_rb->count--;
        uint8_t v = a_rb->buf[a_rb->rd];
        a_rb->rd++;
        if (a_rb->rd >= a_rb->size)
            a_rb->rd = 0;
        return v;
    }
    return 0;
}

void ringbuf_write(struct ring_buffer* a_rb, uint8_t a_v) {
    if (a_rb->count < a_rb->size) {
        a_rb->count++;
        a_rb->buf[a_rb->wr] = a_v;
        a_rb->wr++;
        if (a_rb->wr >= a_rb->size)
            a_rb->wr = 0;
    }
}

