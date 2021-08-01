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


#ifndef _AVRUTILSLIB_CRC8_
#define _AVRUTILSLIB_CRC8_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


static INLINE uint8_t crc8_init(void) {
    return 0xff;
}

static uint8_t crc8_add(uint8_t crc, uint8_t v);

static INLINE uint8_t crc8_ok(uint8_t crc) {
    return crc == 0x00;
}

#ifdef __cplusplus
}
#endif

#endif // _AVRUTILSLIB_CRC8_
 
