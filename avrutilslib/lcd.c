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

#include "avrutilslib/defines.h"
#include "avrutilslib/lcd.h"

#include <util/delay.h>
#include <stdlib.h>

static NOINLINE void lcd_ks0070_write4(uint8_t rs, uint8_t value) {
    port_out(LCD_KS0070_DB_PORT) = (port_out(LCD_KS0070_DB_PORT) & ~(0x0f<<LCD_KS0070_DB_SHIFT))
                | ((value & 0x0f) << LCD_KS0070_DB_SHIFT);
    if (rs) {
        port_setbit(LCD_KS0070_RS_PORT, LCD_KS0070_RS_BIT);
    } else {
        port_clrbit(LCD_KS0070_RS_PORT, LCD_KS0070_RS_BIT);
    }
#if LCD_KS0070_USE_BUSY == 1
    port_clrbit(LCD_KS0070_RW_PORT, LCD_KS0070_RW_BIT);
    port_dirout(LCD_KS0070_DB_PORT, (0x0f<<LCD_KS0070_DB_SHIFT)); // set as output
#endif
    port_setbit(LCD_KS0070_E_PORT, LCD_KS0070_E_BIT);
    _delay_us(1);
    port_clrbit(LCD_KS0070_E_PORT, LCD_KS0070_E_BIT);    
}

#if LCD_KS0070_USE_BUSY == 1
static NOINLINE uint8_t lcd_ks0070_read4(uint8_t rs) {
    uint8_t d;
    
    if (rs) {
        port_setbit(LCD_KS0070_RS_PORT, LCD_KS0070_RS_BIT);
    } else {
        port_clrbit(LCD_KS0070_RS_PORT, LCD_KS0070_RS_BIT);
    }
    port_setbit(LCD_KS0070_RW_PORT, LCD_KS0070_RW_BIT);
    
    port_dirin(LCD_KS0070_DB_PORT, 0x0f<<LCD_KS0070_DB_SHIFT); // set as input
    port_pullup_on(LCD_KS0070_DB_PORT, 0x0f<<LCD_KS0070_DB_SHIFT); // pullups on
    
    port_setbit(LCD_KS0070_E_PORT, LCD_KS0070_E_BIT);
    _delay_us(1);
    d = (port_in(LCD_KS0070_DB_PORT) >> LCD_KS0070_DB_SHIFT) & 0x0f;
    port_clrbit(LCD_KS0070_E_PORT, LCD_KS0070_E_BIT);    
    
    return d;
}

static NOINLINE void lcd_ks0070_wait_while_busy(void) {
    uint8_t d;
    do {
        d = lcd_ks0070_read4(0);
        lcd_ks0070_read4(0); // second dummy read
    } while (d & 0x8);  // busy flag
}
#endif

static NOINLINE void lcd_ks0070_write(uint8_t rs, uint8_t value) {
    lcd_ks0070_write4(rs, value >> 4);
    lcd_ks0070_write4(rs, value);
}

static NOINLINE void lcd_ks0070_command(uint8_t cmd) {
#if LCD_KS0070_USE_BUSY == 1
    lcd_ks0070_wait_while_busy();
    lcd_ks0070_write(0, cmd);
#else
    lcd_ks0070_write(0, cmd);
    _delay_us(45);
#endif
}

static NOINLINE void lcd_ks0070_data(uint8_t data) {
#if LCD_KS0070_USE_BUSY == 1
    lcd_ks0070_wait_while_busy();
    lcd_ks0070_write(1, data);
#else
    lcd_ks0070_write(1, data);
    _delay_us(50);
#endif
}


void NOINLINE lcd_ks0070_init(void) {
    port_clr(LCD_KS0070_DB_PORT, 0x0f<<LCD_KS0070_DB_SHIFT); // set to 0
    port_dirout(LCD_KS0070_DB_PORT, 0x0f<<LCD_KS0070_DB_SHIFT); // set as output
    port_clrbit(LCD_KS0070_RS_PORT, LCD_KS0070_RS_BIT);
    port_dirout(LCD_KS0070_RS_PORT, Bit(LCD_KS0070_RS_BIT));
    port_clrbit(LCD_KS0070_E_PORT,  LCD_KS0070_E_BIT);
    port_dirout(LCD_KS0070_E_PORT,  Bit(LCD_KS0070_E_BIT));    
#if LCD_KS0070_USE_BUSY == 1
    port_clrbit(LCD_KS0070_RW_PORT, LCD_KS0070_RW_BIT); // write
    port_dirout(LCD_KS0070_RW_PORT, Bit(LCD_KS0070_RW_BIT));  // set as output
#endif
    
    // Initialization Sequence
    lcd_ks0070_write4(0, 0x03);
    _delay_ms(5);
    lcd_ks0070_write4(0, 0x03);
    _delay_us(120);
    lcd_ks0070_write4(0, 0x03);
    _delay_us(120);
    
    // switch to 4pin mode
    lcd_ks0070_write4(0, 0x02);
    _delay_us(120);
    
    // Function Set
#if LCD_KS0070_NUM_LINES == 1
    lcd_ks0070_command(0x20);
#else
    lcd_ks0070_command(0x28);
#endif
    
    lcd_ks0070_set_display(LCD_KS0070_DISPLAY_ON);
    lcd_ks0070_display_clear();
    // Entry Mode Set: inc cursor, no display shift
    lcd_ks0070_set_entry_mode(LCD_KS0070_MODE_CURSOR_INC|LCD_KS0070_MODE_DISPLAY_NOSHIFT);    
}


static NOINLINE void lcd_ks0070_display_clear(void) {
    lcd_ks0070_command(0x01);
#if LCD_KS0070_USE_BUSY != 1
    _delay_ms(2);
#endif
}

static NOINLINE void lcd_ks0070_display_return_home(void) {
    lcd_ks0070_command(0x02);
#if LCD_KS0070_USE_BUSY != 1
    _delay_ms(2);
#endif
}

static void lcd_ks0070_set_cursor_addr(uint8_t addr) {
    lcd_ks0070_command(0x80|(addr & 0x7f));
}

static void lcd_set_cursor(uint8_t x, uint8_t y) {
    uint8_t addr;
    switch (y) {
        case 1: addr = LCD_KS0070_LINE2_ADDR; break;
        case 2: addr = LCD_KS0070_LINE3_ADDR; break;
        case 3: addr = LCD_KS0070_LINE4_ADDR; break;
        default: addr = LCD_KS0070_LINE1_ADDR; break;
    }
    lcd_ks0070_set_cursor_addr(addr + x);
}

static void lcd_move_cursor(uint8_t dir) {
    lcd_ks0070_command(0x10|(dir ? 0x04:0x00));
}

static void lcd_shift_display(uint8_t dir) {
    lcd_ks0070_command(0x18|(dir ? 0x04:0x00));
}

static void lcd_ks0070_set_display(uint8_t onoff) {
    lcd_ks0070_command(0x08|(onoff & 0x07));
}

static void lcd_ks0070_set_entry_mode(uint8_t mode) {
    lcd_ks0070_command(0x04|(mode & 0x03));
}


static void lcd_char(char ch) {
    lcd_ks0070_data(ch);
}

static NOINLINE void lcd_char_at(uint8_t x, uint8_t y, char ch) {
    lcd_set_cursor(x, y);
    lcd_char(ch);
}

static NOINLINE void lcd_str(const char* str) {
    while (*str) {
        lcd_char(*str);
        str++;
    }
}

static NOINLINE void lcd_str_at(uint8_t x, uint8_t y, const char* str) {
    lcd_set_cursor(x, y);
    lcd_str(str);
}

static NOINLINE void lcd_str_P(PGM_P str) {
    uint8_t c = pgm_read_byte(str);
    while (c != 0) {
        lcd_char(c);
        c = pgm_read_byte(++str);
    }
}

static NOINLINE void lcd_str_at_P(uint8_t x, uint8_t y, PGM_P str) {
    lcd_set_cursor(x, y);
    lcd_str_P(str);
}

static NOINLINE void lcd_itoa(int i) {
    char buf[7];
    itoa(i, buf, 10);
    lcd_str(buf);
}

static NOINLINE void lcd_hex(uint16_t i, uint8_t ndigits) {
    PGM_P pc = PSTR("0123456789abcdef");
    for (uint8_t k = 4; k; k--) {
        if (k <= ndigits) {
            lcd_char(pgm_read_byte(pc + ((i >> 12) & 0x0f)));
        }
        i <<= 4;
    }
}
