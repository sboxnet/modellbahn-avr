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


#ifdef LCD_KS0070_DB_DDR
# error "lcd.h: please use new defines!"
#endif

#ifndef LCD_KS0070_DB_PORT
# error "LCD_KS0070_PORT_DB not defined!"
#endif
#ifndef LCD_KS0070_DB_SHIFT
# error "LCD_KS0070_DB_SHIFT not defined!"
#endif

#ifndef LCD_KS0070_RS_PORT
# error "LCD_KS0070_PORT_RS not defined!"
#endif
#ifndef LCD_KS0070_RS_BIT
# error "LCD_KS0070_RS_BIT not defined!"
#endif

#ifndef LCD_KS0070_E_PORT
# error "LCD_KS0070_E_PORT not defined!"
#endif
#ifndef LCD_KS0070_E_BIT
# error "LCD_KS0070_E_BIT not defined!"
#endif

#ifndef LCD_KS0070_USE_BUSY
# error "LCD_KS0070_USE_BUSY not defined!"
#endif
#if LCD_KS0070_USE_BUSY == 1
# ifndef LCD_KS0070_RW_PORT
#  error "LCD_KS0070_RW_PORT not defined!"
# endif
# ifndef LCD_KS0070_RW_BIT
#  error "LCD_KS0070_RW_BIT not defined!"
# endif
#endif

#ifndef LCD_KS0070_NUM_LINES
# error "LCD_KS0070_NUM_LINES not defined"
#endif

#ifndef _AVRUTILSLIB_LCD_
#define _AVRUTILSLIB_LCD_

#include <stdint.h>
#include <avr/pgmspace.h>

#ifdef __cplusplus
extern "C" {
#endif

static NOINLINE void lcd_ks0070_init(void);
static NOINLINE void lcd_ks0070_display_clear(void);
static NOINLINE void lcd_ks0070_display_return_home(void);

static inline void lcd_display_clear(void) {
    lcd_ks0070_display_clear();
}

static inline void lcd_display_return_home(void) {
    lcd_ks0070_display_return_home();
}

#define LCD_KS0070_LINE1_ADDR   0x00
#define LCD_KS0070_LINE2_ADDR   0x40
#define LCD_KS0070_LINE3_ADDR   0x14
#define LCD_KS0070_LINE4_ADDR   0x54
static __ATTR_NO_UNUSED_WARN void lcd_ks0070_set_cursor_addr(uint8_t addr);

static __ATTR_NO_UNUSED_WARN void lcd_set_cursor(uint8_t x, uint8_t y);

#define LCD_DIREC_RIGHT    1
#define LCD_DIREC_LEFT     0
static __ATTR_NO_UNUSED_WARN void lcd_move_cursor(uint8_t dir);
static __ATTR_NO_UNUSED_WARN void lcd_shift_display(uint8_t dir);

#define LCD_KS0070_DISPLAY_ON   0x04
#define LCD_KS0070_DISPLAY_OFF  0x00
#define LCD_KS0070_CURSOR_ON    0x02
#define LCD_KS0070_CURSOR_OFF   0x00
#define LCD_KS0070_CURSOR_BLINK 0x03
static __ATTR_NO_UNUSED_WARN void lcd_ks0070_set_display(uint8_t onoff);

#define LCD_KS0070_MODE_CURSOR_INC 0x02
#define LCD_KS0070_MODE_CURSOR_DEC 0x00
#define LCD_KS0070_MODE_DISPLAY_SHIFT 0x01
#define LCD_KS0070_MODE_DISPLAY_NOSHIFT 0x00
static __ATTR_NO_UNUSED_WARN void lcd_ks0070_set_entry_mode(uint8_t mode);

static __ATTR_NO_UNUSED_WARN void lcd_char(char ch);
static __ATTR_NO_UNUSED_WARN NOINLINE void lcd_char_at(uint8_t x, uint8_t y, char ch);
static __ATTR_NO_UNUSED_WARN NOINLINE void lcd_str(const char* str);
static __ATTR_NO_UNUSED_WARN NOINLINE void lcd_str_at(uint8_t x, uint8_t y, const char* str);
static __ATTR_NO_UNUSED_WARN NOINLINE void lcd_str_P(PGM_P str);
static __ATTR_NO_UNUSED_WARN NOINLINE void lcd_str_at_P(uint8_t x, uint8_t y, PGM_P str);
static __ATTR_NO_UNUSED_WARN NOINLINE void lcd_itoa(int i);
static __ATTR_NO_UNUSED_WARN NOINLINE void lcd_hex(uint16_t i, uint8_t ndigits);

#ifdef __cplusplus
}
#endif

#endif
