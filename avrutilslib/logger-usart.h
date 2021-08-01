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


#ifndef _AVRUTILSLIB_LOGGERUSART_H
#define _AVRUTILSLIB_LOGGERUSART_H

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

#ifndef USE_LOGGER_USART
#define USE_LOGGER_USART 0
#endif

#if USE_LOGGER_USART == 1
    
#ifndef LOGGER_USART_BUF_SIZE
# define LOGGER_USART_BUF_SIZE 64
# warning "macro LOGGER_USART_BUF_SIZE not defined, assuming 64 bytes."
#endif

#ifndef LOGGER_USART_ID
# error "macro LOGGER_USART_ID is not defined, do not known which USART to use!"
#endif
    
#ifndef LOGGER_USART_BAUD_UBRR
# error "macro LOGGER_USART_BAUD_UBRR not defined!"
#endif

extern void logger_usart_init(void);

extern void logger_usart_check(void);

extern void logger_usart_str_P(const char* a_str);

extern void logger_usart_hex_byte(uint8_t a_v);

extern void logger_usart_hex_word(uint16_t a_v);

#define DINIT() logger_usart_init()
#define DCHECK() logger_usart_check()
#define DPSTR(_x) logger_usart_str_P(PSTR(_x))
#define DHEXB(_x) logger_usart_hex_byte(_x)
#define DHEXW(_x) logger_usart_hex_word(_x)

#else // USE_LOGGER_USART

#define DINIT()
#define DCHECK()
#define DPSTR(_x)
#define DHEXB(_x)
#define DHEXW(_x)

#endif


#ifdef __cplusplus
}
#endif

#endif /* _AVRUTILSLIB_LOGGERUSART_H */
