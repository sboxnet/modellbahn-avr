/***************************************************************************
 *   Copyright (C) 2012-2014
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


#ifndef _AVRUTILSLIB_RANDOM_
#define _AVRUTILSLIB_RANDOM_

#include <stdint.h>

static inline uint8_t prng_xorshift8(uint8_t x) {
    x ^= (x << 1);
    x ^= (x >> 1);
    x ^= (x * 4); // (x << 2)
    return x;
}

static inline uint16_t prng_xorshift16(uint16_t x) {
    x ^= (x << 1);
    x ^= (x >> 5);
    x ^= (x << 2);
    return x;
}

#endif // _AVRUTILSLIB_RANDOM_
