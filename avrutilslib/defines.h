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


#ifndef _AVRUTILSLIB_DEFINES_
#define _AVRUTILSLIB_DEFINES_

#include <stdint.h>
#include <avr/io.h>
#include <avr/cpufunc.h>

#define INLINE           inline                          // inline this function
#define NOINLINE         __attribute__((noinline))       // do not inline this function
#define __ATTR_NO_UNUSED_WARN   __attribute__((unused))  // do not warn it function is not used
#define __ATTR_NO_RETURN        __attribute__((noreturn)) // function does not return
#define __ATTR_NAKED            __attribute__((naked))   // emit no function entry oder exit code (register push/pop)
#define __ATTR_OS_MAIN          __attribute__((OS_main)) // for main(), may save register push/pop's
#define __ATTR_WEAK             __attribute__((weak))    // this is a weak symbol
#define __ATTR_USED             __attribute__((used))    // emits function code always, even if the optimizer removes the function, or never referenced

#define Bit(_x)  (1<<(_x))
#define Bsv(_x,_v) (_v << (_x))

#define setbv(_r, _bv) ((_r) |= (_bv))
#define clrbv(_r, _bf) ((_r) &= ~(_bv))
#define setbit(_r,_b)  ((_r) |= Bit(_b))
#define clrbit(_r,_b)  ((_r) &= ~Bit(_b))

#define highbyte(_w) ((uint8_t)((_w) >> 8))
#define lowbyte(_w)  ((uint8_t)(_w))

#define minvalof(_a, _b)  ((_a) < (_b) ? (_a) : (_b))
#define maxvalof(_a, _b)  ((_a) > (_b) ? (_a) : (_b))


#define FORCEPTR(_x) __asm__ __volatile__ ("" : "+r"(_x) ::)


#ifdef __AVR_XMEGA__

#define port_set(_port, _bv)       (_port).OUTSET = (_bv)
#define port_clr(_port, _bv)       (_port).OUTCLR = (_bv)
#define port_tgl(_port, _bv)       (_port).OUTTGL = (_bv)
#define port_dirin(_port, _bv)     (_port).DIRCLR = (_bv)
#define port_dirout(_port, _bv)    (_port).DIRSET = (_bv)
#define port_in(_port)             (_port).IN
#define port_out(_port)            (_port).OUT
#define port_dir(_port)            (_port).DIR

static INLINE void port_ctrl_opc(PORT_t* p, uint8_t bv, uint8_t opc) {
    if (bv & 0x01)
        p->PIN0CTRL = (p->PIN0CTRL & ~PORT_OPC_gm) | opc;
    if (bv & 0x02)
        p->PIN1CTRL = (p->PIN1CTRL & ~PORT_OPC_gm) | opc;
    if (bv & 0x04)
        p->PIN2CTRL = (p->PIN2CTRL & ~PORT_OPC_gm) | opc;
    if (bv & 0x08)
        p->PIN3CTRL = (p->PIN3CTRL & ~PORT_OPC_gm) | opc;
    if (bv & 0x10)
        p->PIN4CTRL = (p->PIN4CTRL & ~PORT_OPC_gm) | opc;
    if (bv & 0x20)
        p->PIN5CTRL = (p->PIN5CTRL & ~PORT_OPC_gm) | opc;
    if (bv & 0x40)
        p->PIN6CTRL = (p->PIN6CTRL & ~PORT_OPC_gm) | opc;
    if (bv & 0x80)
        p->PIN7CTRL = (p->PIN7CTRL & ~PORT_OPC_gm) | opc;
}

#define port_pullup_on(_port, _bv)  port_ctrl_opc(&(_port), _bv, PORT_OPC_PULLUP_gc)
#define port_pullup_off(_port, _bv) port_ctrl_opc(&(_port), _bv, PORT_OPC_TOTEM_gc)

#else // !__AVR_XMEGA__

#define port_in(_port)              (*(&_port - 2))
#define port_out(_port)             (_port)
#define port_dir(_port)             *(&(_port) - 1)
#define port_set(_port, _bv)        port_out(_port) |= (_bv)
#define port_clr(_port, _bv)        port_out(_port) &= ~(_bv)
#define port_tgl(_port, _bv)        port_in(_port) = (_bv)
#define port_dirin(_port, _bv)      port_dir(_port) &= ~(_bv)
#define port_dirout(_port, _bv)     port_dir(_port) |= (_bv)
#define port_pullup_on(_port, _bv)  port_out(_port) |= (_bv)
#define port_pullup_off(_port, _bv) port_out(_port) &= ~(_bv)

#endif // __AVR_XMEGA__

#define port_setbit(_port, _b)  port_set(_port, Bit(_b))
#define port_clrbit(_port, _b)  port_clr(_port, Bit(_b))
#define port_tglbit(_port, _b)  port_tgl(_port, Bit(_b))


#ifdef __AVR_XMEGA__

// there is an error in avrlibc 1.8 and gcc 4.8.3
#undef _MemoryBarrier
#define _MemoryBarrier() __asm__ __volatile__("":::"memory")

#define ioreg_ccp(_pioreg, _v) ({ \
    register uint8_t iov = (_v); \
    _MemoryBarrier(); \
    CPU_CCP = CCP_IOREG_gc; \
    *(_pioreg) = iov; })
    
#endif

static inline uint8_t rol_byte(uint8_t v) {
    return (v << 1) + (v & 0x80 ? 1 : 0);
}

static inline uint16_t rol_word(uint16_t v) {
    return (v << 1) + (v & 0x8000 ? 1 : 0);
}

#endif // _AVRUTILSLIB_DEFINES_
 
