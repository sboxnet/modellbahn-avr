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


/*******************************
 * 
 * ACHTUNG: Brown-Out Detection und laengste Startup-Time in den Fuses
 * einstellen. Code enthaelt EEPROM- und Flash-Writes. Wenn Vcc schwankt,
 * z.B. beim Ein/Ausstecken, kann es sein, das zufaellig die entsprechenden
 * Routinen angesprungen werden, was zu EEPROM bzw. Flash Korruption fuehrt!
 * 
 * Fuse 2:  0xbf
 * Fuse 4:  0xf2
 * Fuse 5:  0xe2
 * ****************************/

/* TODO
 * 
 * - crc per Tabelle
 * - cmd: reset dbg info
 * - cmd: reset device
 * - watch timeout should disable device
 */

// sind im bootloader
#define BOOTLOADER 1

#include "common.h"

#include <stdint.h>
#include <stddef.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/crc16.h>

#include <string.h>

#include "avrutilslib/defines.h"
#include "avrutilslib/random.h"

/* Speicheraufteilung atxmega128a4u
 * ================================
 * 
 * ----------------------------------
 * - .appcrc=0x10000 (in bytes)
 * - Applikation CRC
 * ----------------------------------
 * - .blrapp=0x10100 (in bytes)
 * - Bootloader Applikation
 * ----------------------------------
 * - .blrinit=0x20000 (in bytes)
 * - Bootloader Init
 * ----------------------------------
 * - .bldrvec=20200 (in bytes)
 * - Bootloader Vectors, die ersten 15 Vektoren
 * - vector 0: bldr_activate
 * - vector 1: bldr_task
 * - vector 2: bldr_process_basic_msg
 * - vector 3: bldr_reg_read
 * - vector 4: sbn_init
 * - vector 5: sbn_receive_msg
 * - vector 6: sbn_send_msg
 * - vector 7: sbn_all_sent
 * - vector 8-15: Sprung zu _bldr_init
 * ----------------------------------
 * - .bootloader=0x20240 (in bytes)
 * - Bootloader Section
 * ----------------------------------
 * - .bldrcrc=0x21dfe (in bytes)
 * - Bootloader CRC
 *   Hier wird die CRC geschrieben die mit crc-ccitt.py berrchnet wird aus dem .avr File
 * ----------------------------------
 * - .bldrstatic=0x21d00 (in bytes)
 * - Bootloader Static Statische Funktion in der Bootloader Sektion wie:
 * -  uint8_t sbldr_prog_page(uint16_t pageaddr, uint8_t *srcbuf, uint8_t fromflash);
 *     EEPROM Seite schreiben
 * -  void sbldr_finish_fwup(uint8_t isapp);
 *     Firmware Update Ende
 * -  void sbldr_e2prom_write(uint8_t* addr, uint8_t v);
 *     Byte in EEPROM schreiben
 * ----------------------------------
 * - .bldrstatvec=0x21ff8 (in bytes)
 * - Bootloader Zeiger aus die statischen Funktionen:
 * -  sbldr_e2prom_write
 * -  sbldr_finish_fwup
 * -  sbldr_prog_page
 * -----------------------------------
 * - .bldrfwheader=
 * - Bootloader Firmware Header: hier liegt der Firmware Header (ausserhalb des Adressbereichs), siehe bldr_firmware_header.
 */
//-- boot loader specific ----------------------------------------------------------

#undef BOOTLOADER_SECTION

// for atxmega128a4u
// zustätzlicher Code für Bootloader
#define BLDR_APP_SECTION       __attribute__ ((section(".bldrapp")))     // 0x10100
// wo startet der Bootloader)
#define BLDR_INIT_SECTION      __attribute__ ((section(".bldrinit")))    // 0x20000
// wo liegen die Bootloader Vektoren
#define BLDR_VEC_SECTION       __attribute__ ((section(".bldrvec")))     // 0x20200
// der eigentliche Bootloader
#define BOOTLOADER_SECTION     __attribute__ ((section (".bootloader"))) // 0x20240
// Bootloader CRC
#define BLDR_CRC_SECTION       __attribute__ ((section(".bldrcrc")))     // 0x21dfe
// Bootloader Static Code
#define BLDR_STATIC_SECTION    __attribute__ ((section(".bldrstatic")))  // 0x21d00
// Bootloader Static Vektoren
#define BLDR_STATVEC_SECTION   __attribute__ ((section(".bldrstatvec"))) // 0x21ff8
// Fireware Header Sektion
#define BLDR_FIRMWAREHEADER_SECTION __attribute__ ((section(".bldrfwheader")))
// .appcrc wird in common.c defniert!

// Grösse des Bootloaders
#define BLDR_SIZE BOOT_SECTION_SIZE
// wo startet der Bootloader
#define BLDR_START_ADDR BOOT_SECTION_START

// Maximale Grösse des Bootloaders
#define BLDR_PAGE_MAX    (FLASHEND+1) // in bytes!

// prüfe ein paar Werte:
#if (BLDR_PAGE_MAX % SPM_PAGESIZE) != 0
# error  "BLDR_PAGE_MAX must be a multiple of SPM_PAGESIZE"
#endif
#if (BLDR_STATIC_ADDR % SPM_PAGESIZE) != 0
# error  "BLDR_STATIC_ADDR must be a multiple of SPM_PAGESIZE"
#endif
#if (BLDR_START_ADDR % SPM_PAGESIZE) != 0
# error  "BLDR_START_ADDR must be a multiple of SPM_PAGESIZE"
#endif
#if (BLDR_SIZE % SPM_PAGESIZE) != 0
# error  "BLDR_SIZE must be a multiple of SPM_PAGESIZE"
#endif

// Bootloader Vars:
struct bldr_ram g_v; // bootloader vars
uint8_t g_dev_state = 0; // Gerätezustand
uint8_t g_dev_errflags = 0; // Geräte Fehlerflags

// Bootloader Vektoren
BLDR_VEC_SECTION __ATTR_NAKED __ATTR_USED
static void bldr_vector_table(void) {    
    __asm__ __volatile__ (
        "  .globl bldr_activate\n"
		// Einsprung bldr_activate
        "bldr_activate:          jmp _bldr_activate\n"   // vector 0
        "  .globl bldr_task\n"
		// Einsprung bldr_task
        "bldr_task:              jmp _bldr_task\n"       // vector 1
        "  .globl bldr_process_basic_msg\n"
		// Einsprung bldr_process_basic_msg
        "bldr_process_basic_msg: jmp _bldr_process_basic_msg\n"  // vector 2
        "  .globl bldr_reg_read\n"
		// Einsprung bldr_reg_read
        "bldr_reg_read:          jmp _bldr_reg_read\n"   // vector 3
        "  .globl sbn_init\n"
		// Einsprung sbn_init
        "sbn_init:               jmp sboxnet_init\n"// vector 4
        "  .globl sbn_receive_msg\n"
		// Einsprung sbn_receive_msg
        "sbn_receive_msg:        jmp sboxnet_receive_msg\n"// vector 5
        "  .globl sbn_send_msg\n"
		// Einsprung sbn_send_msg
        "sbn_send_msg:           jmp sboxnet_send_msg\n"// vector 6
        "  .globl sbn_all_sent\n"
		// Einsprung sbn_all_sent
        "sbn_all_sent:           jmp sboxnet_all_sent\n"// vector 7
        "  jmp __bldr_init\n"// vector 8
        "  jmp __bldr_init\n"// vector 9
        "  jmp __bldr_init\n"// vector 10
        "  jmp __bldr_init\n"// vector 11
        "  jmp __bldr_init\n"// vector 12
        "  jmp __bldr_init\n"// vector 13
        "  jmp __bldr_init\n"// vector 14
        "  jmp __bldr_init\n"// vector 15
    );
}


BLDR_FIRMWAREHEADER_SECTION __ATTR_NAKED __ATTR_USED
static void bldr_firmware_header(void) {
    _FIRMWARE_HEADER(MAGIC_BOOTLOADER, 0, 0, BLDR_VERSION, BLDR_VERSION);
}

// forward declaration of BLDR_STATIC_SECTION functions
uint8_t sbldr_prog_page(uint32_t pageaddr, uint8_t *srcbuf, uint8_t fromflash);
__ATTR_NO_RETURN void sbldr_finish_fwup(uint8_t isapp);
void sbldr_e2prom_write(uint8_t* addr, uint8_t v);


// symbols defined by linker.
// if only bootloader is linked, this symbol points to INTERNAL_SRAM_START.
// if application is linked, this symbol points to the end of BSS.
extern uint8_t _end;

// BLDR_STACK_TOP see common.h

#define STACK_MAGIC   0
BOOTLOADER_SECTION
static uint16_t bldr_stack_free(void) {
    uint8_t* p = &_end;
    while (p < (uint8_t*)BLDR_STACK_TOP) {
        if (*p != STACK_MAGIC) {
            break;
        }
        p++;
    }
    return (uint16_t)(p - &_end);
}
BOOTLOADER_SECTION
static uint16_t bldr_stack_size(void) {
    return (uint8_t*)BLDR_STACK_TOP - &_end;
}


#define LED_PORT     PORTE
#define LED_RED_b    1
#define LED_GREEN_b  0

// identify button bit
#define KEY_ID_b     0

//--- SBOXNET -----------------------------------------

/*
#define TCE_PRESCALER TC_CLKSEL_DIV8_gc
#define TCE_PERIOD    (16384-1)
*/
#define SBOXNET_TIMER_PRESCALER TC_CLKSEL_DIV8_gc
#define SBOXNET_TIMER_PERIOD    (16384-1)  // 32Mhz / 8 / (16384-1) = 244 Hz = 4ms

#define SBOXNET_DEVICE_ADDR    g_v.dev_addr

#define SBOXNET_VAR                   (&g_v.sboxnet)
#define SBOXNET_SECTION               BLDR_APP_SECTION
#define SBOXNET_SECTION2              BLDR_APP_SECTION
#define SBOXNET_RxD_PORT              PORTE
#define SBOXNET_RxD_BIT               2
#define SBOXNET_TxD_PORT              PORTE
#define SBOXNET_TxD_BIT               3
#define SBOXNET_USART_RxC_vect        USARTE0_RXC_vect
#define SBOXNET_USART_TxC_vect        USARTE0_TXC_vect
#define SBOXNET_USART_RXCINTLVL       USART_RXCINTLVL_MED_gc
#define SBOXNET_USART_TXCINTLVL       USART_TXCINTLVL_MED_gc
#define SBOXNET_TIMER                 TCE0
#define SBOXNET_TIMER_CC              CCA
#define SBOXNET_TIMER_CCIF            TC0_CCAIF_bp
#define SBOXNET_TIMER_CCINTLVL_MASK   TC0_CCAINTLVL_gm
#define SBOXNET_TIMER_CCINTLVL        TC_CCAINTLVL_MED_gc
#define SBOXNET_TIMER_BIT_TICKS       16
#define SBOXNET_TIMER_PERIOD_MASK     SBOXNET_TIMER_PERIOD
#define SBOXNET_TIMER_RANDOM_MASK     0x7f
#define SBOXNET_HW_SENSOR_INTLVL_MASK PORT_INT0LVL_gm
#define SBOXNET_HW_SENSOR_INTMASK     INT0MASK
#define SBOXNET_HW_SENSOR_INTF_BIT    PORT_INT0IF_bp
#define SBOXNET_USART_ID              USARTE0
#define SBOXNET_USART_DEF_INTR        1
#define SBOXNET_PRNG_SEED             (g_v.prandom)
#include "sboxnet/sboxnet.c"

SBOXNET_SECTION ISR(TCE0_CCA_vect) {
    sboxnet_timer_bit();
}

BOOTLOADER_SECTION ISR(TCE0_OVF_vect) { // every 4ms
    ++g_v.timer;
    
    if (g_v.timer_logon) {
        g_v.timer_logon--;
    }
    if (g_v.timer_leds) {
        g_v.timer_leds--;
    }
    if (g_v.timer_keys) {
        g_v.timer_keys--;
    }
    if (g_v.timer_watchdog) {
        clrbit_atomic(g_dev_state, DEV_STATE_FLG_WATCHDOG_b);
        g_v.timer_watchdog--;
    } else {
        setbit_atomic(g_dev_state, DEV_STATE_FLG_WATCHDOG_b);
    }
}

BLDR_APP_SECTION
static void bldr_leds_task(void) {
    // read identify key
    if (g_v.timer_keys == 0) {
        if (bit_is_clear(port_dir(LED_PORT), LED_RED_b)) {
            g_v.timer_keys = 10;
            uint8_t sw = bit_is_set(port_in(LED_PORT), LED_RED_b) ? 0 : Bit(KEY_ID_b);
            port_dirout(LED_PORT, Bit(LED_RED_b));
            debounce_keys(&g_v.key_id, &g_v.key_id_t, sw);
        } else {
            g_v.timer_keys = 1;
            port_dirin(LED_PORT, Bit(LED_RED_b));
        }
    }
    
    if (g_v.timer_leds == 0) { // every 128ms
        g_v.timer_leds = 32;
        
        if (bit_is_set(g_v.key_id, KEY_ID_b)) {
            if (bit_is_clear(g_v.key_id_state, KEY_ID_b)) {
                setbit(g_v.key_id_state, KEY_ID_b);
                ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                    g_dev_state ^= Bit(DEV_STATE_FLG_IDENTIFY_b);
                }
            }
        } else {
            clrbit(g_v.key_id_state, KEY_ID_b);
        }

        uint8_t st = g_dev_state;
        if (st != g_v.dev_old_state) {
            g_v.dev_old_state = st;
            if (bit_is_set(st, DEV_STATE_FLG_IDENTIFY_b)) {
                g_v.led_gn = 0xaa;
                g_v.led_rt = 0x55;
            } else if (bit_is_set(st, DEV_STATE_FLG_REQ_ADDR_b)) {
                g_v.led_gn = 0x00;
                g_v.led_rt = 0xcc;
            } else if (st & DEV_STATE_FLG_FWUP_MASK) {
                g_v.led_gn = 0xaa;
                g_v.led_rt = 0;
            } else if (bit_is_set(st, DEV_STATE_FLG_BOOTLOADER_b)) {
                g_v.led_gn = 0x3e;
                g_v.led_rt = 0;
            } else {
                g_v.led_gn = 0x18;
                g_v.led_rt = 0;
            }
            if (bit_is_clear(st, DEV_STATE_FLG_WATCHDOG_b)) {
                setbit(g_v.led_gn, 7);
            }
        }
        if (g_dev_errflags) {
            g_v.led_rt = 0xff;
        }
        
        g_v.led_gn = rol_byte(g_v.led_gn);
        if (g_v.led_gn & 0x80) {
            port_clr(LED_PORT, Bit(LED_GREEN_b));
        } else {
            port_set(LED_PORT, Bit(LED_GREEN_b));
        }
        
        g_v.led_rt = rol_byte(g_v.led_rt);
        if (g_v.led_rt & 0x80) {
            port_clr(LED_PORT, Bit(LED_RED_b));
        } else {
            port_set(LED_PORT, Bit(LED_RED_b));
        }
    }
}

// must be put explicitly in the bldr_app_section. _crc_ccitt_update is a "static inline", but the compiler can generate a function that is put into the .text section!
BLDR_APP_SECTION
static uint16_t crc_ccitt_update(uint16_t crc, uint8_t data) {
    return _crc_ccitt_update(crc, data);
}

BLDR_APP_SECTION NOINLINE __ATTR_USED
static uint8_t _bldr_process_basic_msg(struct sboxnet_msg_header *pmsg) {
    switch (pmsg->cmd) {        
        case SBOXNET_CMD_DEV_FW_UPD_START: {
            uint16_t* pdata = (uint16_t*)pmsg->data;
            uint8_t msglen = pmsg->opt.len;
            clrbit_atomic(g_dev_state, DEV_STATE_FLG_FWUP_BOOTLOADER_b);
            clrbit_atomic(g_dev_state, DEV_STATE_FLG_FWUP_APPLICATION_b);
            if (msglen != (3+32)) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            uint16_t size = pdata[0];
            struct firmware_header* pfwh = (struct firmware_header*)(pmsg->data + 3);
            if (pfwh->magic == MAGIC_BOOTLOADER) {
                if (bit_is_set(pmsg->data[2], 0)) {
                    return SBOXNET_ACKRC_FWUP_INVMODE;
                }
                if (size != BLDR_SIZE) {
                    return SBOXNET_ACKRC_FWUP_OVERFLOW;
                }
                setbit_atomic(g_dev_state, DEV_STATE_FLG_FWUP_BOOTLOADER_b);
            } else if (pfwh->magic == MAGIC_APPLICATION) {
                if (bit_is_clear(pmsg->data[2], 0)) {
                    return SBOXNET_ACKRC_FWUP_INVMODE;
                }
                uint16_t e_prdid = e2prom_get_word(&bldr_eeprom.productid);
                uint16_t e_venid = e2prom_get_word(&bldr_eeprom.vendorid);
                if (e_prdid == 0xffff && e_venid == 0xffff) {
                    // no application code ever loaded
                } else if (pfwh->productid != e_prdid || pfwh->vendorid != e_venid) {
                    return SBOXNET_ACKRC_FWUP_INVDEV;
                }
             /*   if (pfwh->bootloader_version != BLDR_VERSION) {
                    return SBOXNET_ACKRC_FWUP_INVBLDR;
                }
             */
                if (size == 0 || size >= BLDR_PAGE_MAX) {
                    return SBOXNET_ACKRC_FWUP_OVERFLOW;
                }
                setbit_atomic(g_dev_state, DEV_STATE_FLG_FWUP_APPLICATION_b);
            }

            g_v.fwup_pageaddr = 0;
            g_v.fwup_pageoffs = 0;
            g_v.fwup_size = size;
            
            uint16_t crc = 0xffff;
            uint8_t* psrc = (uint8_t*)pfwh;
            for (uint8_t length = 32; length > 0; length--) {
                crc = crc_ccitt_update(crc, *psrc);
                psrc++; // produces better code ...
            }
            g_v.fwup_crc = crc;

            pmsg->opt.len = 0;
            return SBOXNET_ACKRC_OK;
        }
        
        case SBOXNET_CMD_DEV_FW_UPD_LOAD: {
            return SBOXNET_ACKRC_FWUP_INACTIVE;
        }
        
        case SBOXNET_CMD_DEV_GET_DESC: {
            uint8_t id = 0;
            if (pmsg->opt.len > 1) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            if (pmsg->opt.len == 1) {
                id = pmsg->data[0];
            }
            if (id >= DEV_NUM_DESCS) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            uint8_t* pdesc = bldr_eeprom.dev_desc[id];
            uint8_t n = 0;
            while (n < DEV_DESC_MAX_SIZE) {
                uint8_t c = e2prom_get_byte(pdesc++);
                if (c == 0 || c == 0xff) {
                    break;
                }
                pmsg->data[n++] = c;
            }
            pmsg->opt.len = n;
            return SBOXNET_ACKRC_OK;
        }
        case SBOXNET_CMD_DEV_IDENTIFY: {
            if (pmsg->opt.len != 1) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            uint8_t on = pmsg->data[0] & 0x01;
            if (on) {
                setbit_atomic(g_dev_state, DEV_STATE_FLG_IDENTIFY_b);
            } else {
                clrbit_atomic(g_dev_state, DEV_STATE_FLG_IDENTIFY_b);
            }
            pmsg->opt.len = 0;
            return SBOXNET_ACKRC_OK;
        }
    }
    return SBOXNET_ACKRC_CMD_UNKNOWN;
}

BLDR_APP_SECTION NOINLINE __ATTR_USED
static uint8_t _bldr_reg_read(uint16_t reg, uint16_t *pdata) {
    switch (reg) {
        case R_PUID_L: *pdata = (uint16_t)g_v.dev_puid; return 0;
        case R_PUID_H: *pdata = (uint16_t)(g_v.dev_puid >> 16); return 0;
        case R_PRODUCTID: *pdata = e2prom_get_word(&bldr_eeprom.productid); return 0; 
        case R_VENDORID:  *pdata = e2prom_get_word(&bldr_eeprom.vendorid); return 0; 
        case R_BOOTLOADER_VERSION: *pdata = BLDR_VERSION; return 0;
        case R_PROTOKOLL_VERSION:  *pdata = SBOXNET_PROTOCOL_VERSION; return 0;
        case R_DEV_ADDR: *pdata = g_v.dev_addr; return 0;
        case R_DEV_STATE: *pdata = g_dev_state; return 0;
        case R_DEV_ERROR: *pdata = g_dev_errflags; return 0;
        case R_DEV_BOOTSTATE: *pdata = e2prom_get_byte(&bldr_eeprom.firmware_update); return 0;
        case R_DEV_NUM_DESCS: *pdata = DEV_NUM_DESCS; return 0;
        
        case R_DEBUG_BLDR_RAM_OFFS:  *pdata = (uint16_t)&g_v - INTERNAL_SRAM_START; return 0;
        case R_DEBUG_COM_RAM_OFFS:   *pdata = 0; return 0;
        case R_DEBUG_SBN_RAM_OFFS:   *pdata = (uint16_t)&g_v.sboxnet - INTERNAL_SRAM_START; return 0;
        case R_DEBUG_BLDR_EEPROM_OFFS: *pdata = BLDR_EEPROM_ADDR; return 0;
        case R_DEBUG_SRAM_SIZE:      *pdata = INTERNAL_SRAM_SIZE; return 0;
        case R_DEBUG_EEPROM_SIZE:    *pdata = EEPROM_SIZE; return 0;
        case R_DEBUG_STACK_SIZE:     *pdata = bldr_stack_size(); return 0;
        case R_DEBUG_STACK_FREE:     *pdata = bldr_stack_free(); return 0;
        
        case R_DEBUG_SBN_STATE: *pdata = g_v.sboxnet.state; return 0;
        case R_DEBUG_SBN_FLAGS: *pdata = g_v.sboxnet.flags; return 0;
    }
    if (reg >= R_DEBUG_SBN_0 && reg < R_DEBUG_SBN_0 + NUM_R_DEBUG_SBN) {
       if ((reg - R_DEBUG_SBN_0) < (sizeof(struct sboxnet_debug_info)+1)/2) {
            *pdata = *((uint16_t*)(&g_v.sboxnet.debug) + (reg - R_DEBUG_SBN_0));
        } else {
            *pdata = 0;
        }
        return 0;
    }
    if (reg >= R_DEBUG_EEPROM_BASE && reg < R_DEBUG_EEPROM_BASE + NUM_R_DEBUG_EEPROM) {
        uint16_t addr = 2*(reg - R_DEBUG_EEPROM_BASE);
        if (addr < EEPROM_SIZE) {
            *pdata = e2prom_get_word(addr);
            return 0;
        }
        return SBOXNET_ACKRC_REG_INVALID;
    }
    if (reg >= R_DEBUG_SRAM_BASE && reg < R_DEBUG_SRAM_BASE + NUM_R_DEBUG_SRAM) {
        uint16_t addr = 2*(reg - R_DEBUG_SRAM_BASE);
        if (addr < INTERNAL_SRAM_SIZE) {
            *pdata = *(uint16_t*)(INTERNAL_SRAM_START + addr);
            return 0;
        }
        return SBOXNET_ACKRC_REG_INVALID;
    }
    return SBOXNET_ACKRC_REG_INVALID;
}

BLDR_APP_SECTION
static uint8_t bldr_cmd_reg_read(struct sboxnet_msg_header *pmsg) {
    uint16_t *pdata = (uint16_t*)pmsg->data;
    uint16_t regaddr = *pdata;
    uint8_t num = pmsg->data[2];
    if (pmsg->opt.len != 3 || num > SBOXNET_MSG_DATA_LEN/2 ) {
        return SBOXNET_ACKRC_INVALID_ARG;
    }
    pmsg->opt.len = 2*num;
    while (num) {
        uint8_t rc = bldr_reg_read(regaddr++, pdata++);
        if (rc != 0) {
            return rc;
        }
        num--;
    }
    return SBOXNET_ACKRC_OK;
}

BLDR_APP_SECTION
static uint8_t bldr_cmd_reg_read_multi(struct sboxnet_msg_header *pmsg) {
    uint16_t *pdata = (uint16_t*)pmsg->data;
    if (pmsg->opt.len > SBOXNET_MSG_DATA_LEN || (pmsg->opt.len & 0x01)) {
        return SBOXNET_ACKRC_INVALID_ARG;
    }
    uint8_t n = pmsg->opt.len / 2;
    while (n > 0) {
        uint8_t rc = bldr_reg_read(*pdata, pdata);
        if (rc != 0) {
            return rc;
        }
        pdata++;
        n--;
    }
    return SBOXNET_ACKRC_OK;
}

BLDR_APP_SECTION
static void bldr_process_cmd_net_reset(void) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        g_v.dev_addr = SBOXNET_ADDR_BROADCAST;
        setbit(g_dev_state, DEV_STATE_FLG_REQ_ADDR_b);
    }
}

BLDR_APP_SECTION
static uint8_t bldr_process_std_msg(struct sboxnet_msg_header *pmsg, uint8_t* pagebuf) {
    switch (pmsg->cmd) {
        case SBOXNET_CMD_NET_RESET: {
            if (pmsg->srcaddr == 0) {
                bldr_process_cmd_net_reset();
            }
            return SBOXNET_ACKRC_SEND_NO_ANSWER;
        }
        case SBOXNET_CMD_NET_WATCHDOG: {
            return SBOXNET_ACKRC_SEND_NO_ANSWER;
        }
        case SBOXNET_CMD_DEV_RESET: {
            setbit_atomic(g_dev_state, DEV_STATE_FLG_RESET_b);
            if (pmsg->dstaddr == SBOXNET_ADDR_BROADCAST) {
                return SBOXNET_ACKRC_SEND_NO_ANSWER;
            } else {
                pmsg->opt.len = 0;
                return SBOXNET_ACKRC_OK;
            }
        }
    }
    
    if (pmsg->dstaddr == SBOXNET_ADDR_BROADCAST) {
        return SBOXNET_ACKRC_SEND_NO_ANSWER;
    }

    switch (pmsg->cmd) {
        case SBOXNET_CMD_REG_READ: {
            return bldr_cmd_reg_read(pmsg);
        }
        case SBOXNET_CMD_REG_READ_MULTI: {
            return bldr_cmd_reg_read_multi(pmsg);
        }
        case SBOXNET_CMD_DEV_FW_UPD_LOAD: {
            if ((g_dev_state & DEV_STATE_FLG_FWUP_MASK) == 0) {
                return SBOXNET_ACKRC_FWUP_INACTIVE;
            }
            uint16_t* pdata = (uint16_t*)pmsg->data;
            uint8_t len = pmsg->opt.len;
            if (len < 3 || len > 35) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            len -= 3;
            uint16_t paddr = pdata[0];
            uint8_t flags = pmsg->data[2];
            if (flags & 0x01) { // firmware
                if ((g_dev_state & DEV_STATE_FLG_FWUP_MASK) != DEV_STATE_FLG_FWUP_APPLICATION)
                    return SBOXNET_ACKRC_FWUP_INACTIVE;
                if (paddr + len > BLDR_SIZE) {
                    return SBOXNET_ACKRC_FWUP_OVERFLOW;
                }
            } else { // bootloader
                if ((g_dev_state & DEV_STATE_FLG_FWUP_MASK) != DEV_STATE_FLG_FWUP_BOOTLOADER)
                    return SBOXNET_ACKRC_FWUP_INACTIVE;
                if (paddr + len > BLDR_PAGE_MAX) {
                    return SBOXNET_ACKRC_FWUP_OVERFLOW;
                }
            }
            if (paddr != g_v.fwup_pageaddr + g_v.fwup_pageoffs) {
                return SBOXNET_ACKRC_FWUP_INVADDR;
            }
            if (len == 0) {
                if (g_v.fwup_pageoffs > 0) {
                    if (sbldr_prog_page(g_v.fwup_pageaddr, pagebuf, 0) != 0) {
                        return SBOXNET_ACKRC_FWUP_VERIFY_FAILED;
                    }
                }
                if (((g_dev_state & DEV_STATE_FLG_FWUP_MASK) == DEV_STATE_FLG_FWUP_BOOTLOADER)
                        && g_v.fwup_pageaddr + g_v.fwup_pageoffs != BLDR_SIZE) {
                    return SBOXNET_ACKRC_FWUP_OVERFLOW;
                } else if (((g_dev_state & DEV_STATE_FLG_FWUP_MASK) == DEV_STATE_FLG_FWUP_APPLICATION)
                        && g_v.fwup_pageaddr + g_v.fwup_pageoffs != g_v.fwup_size) {
                    return SBOXNET_ACKRC_FWUP_OVERFLOW;
                }
                // check crc
                uint16_t crc = g_v.fwup_crc;
                uint16_t length = g_v.fwup_size;
                uint16_t psrc = 0;
                for (; length > 0; length--) {
                    crc = crc_ccitt_update(crc, pgm_read_byte(psrc++));
                }
                if (crc != 0) {
                    return SBOXNET_ACKRC_FWUP_INVCRC;
                }
                return SBOXNET_ACKRC_FWUP_FINISHED;
            }
            for (uint8_t i = 3; i < pmsg->opt.len; i++) {
                pagebuf[g_v.fwup_pageoffs++] = pmsg->data[i];
            
                if (g_v.fwup_pageoffs == SPM_PAGESIZE) {
                    if (sbldr_prog_page(g_v.fwup_pageaddr, pagebuf, 0) != 0) {
                        return SBOXNET_ACKRC_FWUP_VERIFY_FAILED;
                    }
                    g_v.fwup_pageaddr += SPM_PAGESIZE;
                    g_v.fwup_pageoffs = 0;
                }
            }
            pmsg->opt.len = 0;
            return SBOXNET_ACKRC_OK;
        }
        default:
            return bldr_process_basic_msg(pmsg);        
    }
    return SBOXNET_ACKRC_CMD_UNKNOWN;
}

BLDR_APP_SECTION
static uint8_t NOINLINE bldr_process_msg(struct sboxnet_msg_header* pmsg, uint8_t* pagebuf) {    
    uint8_t rc = 0;
    
    if (pmsg->srcaddr == 0) {
        g_v.timer_watchdog = WATCHDOG_VAL;
    }

    if (pmsg->cmd & 0x80) {
        return rc;  // discard Ack Answers
    }
    
    rc = bldr_process_std_msg(pmsg, pagebuf);
    if (rc == SBOXNET_ACKRC_SEND_NO_ANSWER) {
        return rc;
    }
    if (rc == SBOXNET_ACKRC_OK || rc == SBOXNET_ACKRC_FWUP_FINISHED) {
        goto send_ok;
    }
    
//send_error:
    pmsg->opt.len = 2;
    pmsg->data[0] = pmsg->cmd;
    pmsg->data[1] = rc;
    pmsg->cmd = 0x80;

send_ok:
    // send anwser
    pmsg->dstaddr = pmsg->srcaddr;
    pmsg->cmd |= 0x80;
    sboxnet_send_msg(pmsg);
    return rc;
}


/* Den Chip auf 32MHz umschalten.
 * Chip läuft nach dem Start mit 2MHz.
 */
//BOOTLOADER_SECTION
BLDR_APP_SECTION
static void bldr_switch_to_32MHz(void) {
    // zuerst auf internen 32 MHz Osz Umschalten
	OSC.CTRL |= OSC_RC32MEN_bm;
	//warte bis der Stabil ist
	while (bit_is_clear(OSC.STATUS,OSC_RC32MRDY_bp)) {
		_NOP();
	}
	// die RC32MEN als Taktquelle einstellen
	ioreg_ccp(&CLK.CTRL, CLK_SCLKSEL_RC32M_gc);
	// den internen 2MHz Takt stoppen
	clrbit(OSC.CTRL, OSC_RC2MEN_bp);
	//auf externen 16Mhz OSC Schalten
	// Frequenzberich 12-16MHz wählen und Startup Zeit 16k
	OSC.XOSCCTRL = OSC_FRQRANGE_12TO16_gc|OSC_XOSCSEL_XTAL_16KCLK_gc;
	// Externen Oszillator einschalten
	setbit(OSC.CTRL, OSC_XOSCEN_bp);
	// warte bis ext. Oszillator stabil ist
	// 200 Mal versuchen umzuschalten
	uint8_t is32 = 0;
	uint8_t cnt = 200;
	while (cnt--) {
		if (bit_is_set(OSC.STATUS, OSC_XOSCRDY_bp)) {
			is32 = 1;
			break;
		}
		_NOP();
		_NOP();
		_NOP();
	}
	// Wenn externer Osc ist stabil?
	if (is32) {
		// dann die PLL auf 2x stellen == 32MHz
		OSC.PLLCTRL = OSC_PLLSRC_XOSC_gc|2; // PLL multiplier: 2x
		// PLL aktivieren
		setbit(OSC.CTRL, OSC_PLLEN_bp);
		// warte bis PLL stabil ist
		while (bit_is_clear(OSC.STATUS, OSC_PLLRDY_bp)){
			_NOP();
		}
		// die PLL als Taktquelle einstellen
		ioreg_ccp(&CLK.CTRL, CLK_SCLKSEL_PLL_gc);
		// den internen 32MHz Takt stoppen
		clrbit(OSC.CTRL, OSC_RC32MRDY_bp);
	}
}

BOOTLOADER_SECTION
static uint16_t bldr_random(void) {
    uint16_t r;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        r = prng_xorshift16(g_v.prandom);
        g_v.prandom = r;
    }
    return r;
}

BOOTLOADER_SECTION
void bldr_init_ports(void) {
	PORTA.DIR = 0;
	PORTCFG_MPCMASK = 0xff; // all pins
	PORTA.PIN0CTRL = PORT_OPC_PULLDOWN_gc;
	    
	PORTB.DIR = 0;
	PORTCFG_MPCMASK = 0xff; // all pins
	PORTB.PIN0CTRL = PORT_OPC_PULLDOWN_gc;
	    
	PORTC.DIR = 0;
	PORTCFG_MPCMASK = 0xff; // all pins
	PORTC.PIN0CTRL = PORT_OPC_PULLDOWN_gc;

	PORTD.DIR = 0;
	PORTCFG_MPCMASK = 0xff; // all pins
	PORTD.PIN0CTRL = PORT_OPC_PULLDOWN_gc;

	PORTE.DIR = 0;
	PORTCFG_MPCMASK = 0xff; // all pins
	PORTE.PIN0CTRL = PORT_OPC_PULLUP_gc;
	    
	// virtual port maps: VP0: PORTA, VP1: PORTB, VP2: PORTC, VP3: PORTD
	PORTCFG_VPCTRLA = PORTCFG_VP1MAP_PORTB_gc|PORTCFG_VP0MAP_PORTA_gc;
	PORTCFG_VPCTRLB = PORTCFG_VP3MAP_PORTD_gc|PORTCFG_VP2MAP_PORTC_gc;

	port_dirout(PORTE, 0x03); // LEDs on
	port_clr(PORTE, 0x03);
	g_v.led_gn = 0x03;
	g_v.led_rt = 0x00;   
}

BOOTLOADER_SECTION
void bldr_init_devvars(void) {
	    g_dev_state = 0;
	    g_dev_errflags = 0;
}

BOOTLOADER_SECTION
void bldr_init_pr(void) {
#ifdef __AVR_HAVE_PRGEN_USB
	PR.PRGEN = Bit(PR_AES_bp)|Bit(PR_USB_bp)|Bit(PR_RTC_bp);
#else
	PR.PRGEN = Bit(PR_AES_bp)|Bit(PR_RTC_bp);
#endif
	PR.PRPE = Bit(PR_TWI_bp)|Bit(PR_USART1_bp)/*|Bit(PR_USART0_bp)*/|Bit(PR_SPI_bp)|Bit(PR_HIRES_bp)|Bit(PR_TC1_bp); // PR_TC0_bp;
}

BOOTLOADER_SECTION
void bldr_init_sboxnettimer(void) {
	// timer for sboxnet
	SBOXNET_TIMER.CTRLB = TC_WGMODE_NORMAL_gc;
	SBOXNET_TIMER.CTRLD = 0;
	SBOXNET_TIMER.CTRLE = 0;
	SBOXNET_TIMER.INTCTRLA = TC_OVFINTLVL_MED_gc;
	SBOXNET_TIMER.INTCTRLB = 0;
	SBOXNET_TIMER.INTFLAGS = 0xff;
	SBOXNET_TIMER.PER = SBOXNET_TIMER_PERIOD;
	SBOXNET_TIMER.CTRLA = SBOXNET_TIMER_PRESCALER;    
}

BOOTLOADER_SECTION
void bldr_init_vars(void) {
	g_v.dev_puid = e2prom_get_long(&bldr_eeprom.puid);
	g_v.dev_addr = 0xff;
	    
	g_v.fwup_pageaddr = 0;
	g_v.fwup_pageoffs = 0;
	g_v.fwup_size = 0;
	    
	g_v.prandom = (uint16_t)g_v.dev_puid; // seed random value
	g_v.timer = (uint8_t)bldr_random(); // init timer to a random value
	
#if defined(__AVR_HAVE_PRGEN_USB)
	g_v.is_a4u = 1;
#else
	g_v.is_a4u = 0;
#endif
}

BOOTLOADER_SECTION
void bldr_init_modules(void) {
	// PB0..3 zur abfrage des Modul Typs
	
	// PB auf Eingabe PullUps ON
	PORTB.DIR = 0x08;
	PORTCFG_MPCMASK = 0xff; // all pins
	PORTB.PIN0CTRL = PORT_OPC_PULLUP_gc;
	// lese Modultyp
	volatile uint16_t module_typ = PORTB.IN;
	g_v.module = module_typ;
	volatile uint8_t a = 0;
}

BOOTLOADER_SECTION
void bldr_init_timers(void) {
	// alle Timer zuruecksetzen

	// TCC0
	TCC0.CTRLA = 0;
	TCC0.CTRLB = 0;
	TCC0.CTRLC = 0;
	TCC0.CTRLD = 0;
	TCC0.CTRLE = 0;
	TCC0.INTCTRLA = 0;
	TCC0.INTCTRLB = 0;
	TCC0.INTFLAGS = 0xff;
	TCC0.CNT = 0;
	TCC0.PER = 0;
	TCC0.CCA = 0;
	TCC0.CCB = 0;
	TCC0.CCC = 0;
	TCC0.CCD = 0;

	// TCC1
	TCC1.CTRLA = 0;
	TCC1.CTRLB = 0;
	TCC1.CTRLC = 0;
	TCC1.CTRLD = 0;
	TCC1.CTRLE = 0;
	TCC1.INTCTRLA = 0;
	TCC1.INTCTRLB = 0;
	TCC1.INTFLAGS = 0xff;
	TCC1.CNT = 0;
	TCC1.PER = 0;
	TCC1.CCA = 0;
	TCC1.CCB = 0;
	
	// TCD0
	TCD0.CTRLA = 0;
	TCD0.CTRLB = 0;
	TCD0.CTRLC = 0;
	TCD0.CTRLD = 0;
	TCD0.CTRLE = 0;
	TCD0.INTCTRLA = 0;
	TCD0.INTCTRLB = 0;
	TCD0.INTFLAGS = 0xff;
	TCD0.CNT = 0;
	TCD0.PER = 0;
	TCD0.CCA = 0;
	TCD0.CCB = 0;
	TCD0.CCC = 0;
	TCD0.CCD = 0;

	// TCD1
	TCD1.CTRLA = 0;
	TCD1.CTRLB = 0;
	TCD1.CTRLC = 0;
	TCD1.CTRLD = 0;
	TCD1.CTRLE = 0;
	TCD1.INTCTRLA = 0;
	TCD1.INTCTRLB = 0;
	TCD1.INTFLAGS = 0xff;
	TCD1.CNT = 0;
	TCD1.PER = 0;
	TCD1.CCA = 0;
	TCD1.CCB = 0;
}

BOOTLOADER_SECTION
static void bldr_init_system(void) {
	bldr_init_timers();
	bldr_init_ports();
	bldr_init_modules();
	bldr_init_devvars();
    bldr_switch_to_32MHz();
	bldr_init_pr();
	bldr_init_sboxnettimer();
    bldr_init_vars();
}

BLDR_APP_SECTION
static void bldr_sboxnet_logon(struct sboxnet_msg_max* pmsg) {
    uint32_t *ppuid = (uint32_t*)pmsg->data;
    uint16_t *pprodid = (uint16_t*)(pmsg->data + 4);
    uint16_t *pvendid = (uint16_t*)(pmsg->data + 6);
    
    uint16_t productid = e2prom_get_word(&bldr_eeprom.productid);
    uint16_t vendorid = e2prom_get_word(&bldr_eeprom.vendorid);
    
    int8_t rc = sboxnet_receive_msg(&pmsg->msgh, sizeof(*pmsg));
    if (rc > 0) {
        if (pmsg->msgh.srcaddr == 0) {
            if (g_v.timer_watchdog == 0) {
                g_v.timer_logon = 1 + (bldr_random() & 0x1f);
            }
            g_v.timer_watchdog = WATCHDOG_VAL;
        }
        if (pmsg->msgh.srcaddr == 0 && pmsg->msgh.dstaddr == SBOXNET_ADDR_BROADCAST && pmsg->msgh.cmd == SBOXNET_CMD_DEV_SET_ADDR) {
           if (pmsg->msgh.opt.len == 9 && *ppuid == g_v.dev_puid && *pprodid == productid && *pvendid == vendorid) {
                g_v.dev_addr = pmsg->msgh.data[8];
                
                pmsg->msgh.dstaddr = 0;
                pmsg->msgh.cmd |= 0x80;
                pmsg->msgh.seq = 0;
                pmsg->msgh.opt.len = 0;
                sboxnet_send_msg(&pmsg->msgh);
                
                clrbit_atomic(g_dev_state, DEV_STATE_FLG_REQ_ADDR_b);
                return;
            }
        }
    }
    if (g_v.timer_watchdog && g_v.timer_logon == 0) {
        pmsg->msgh.dstaddr = 0;
        pmsg->msgh.seq = 0;
        pmsg->msgh.opt.len = 8;
        pmsg->msgh.cmd = SBOXNET_CMD_DEV_REQ_ADDR;
        if (sboxnet_can_send_msg(&pmsg->msgh)) {
            g_v.timer_logon = 10 + (bldr_random() & 0x1f);
            *ppuid = g_v.dev_puid;
            *pprodid = productid;
            *pvendid = vendorid;
            sboxnet_send_msg(&pmsg->msgh);
        }
    }
}

BLDR_APP_SECTION NOINLINE __ATTR_USED
static uint8_t _bldr_task(struct sboxnet_msg_max* pmsg) {
    bldr_leds_task();
    
    if (bit_is_set(g_dev_state, DEV_STATE_FLG_RESET_b)) {
        while (!sboxnet_all_sent());
        ioreg_ccp(&RST.CTRL, Bit(RST_SWRST_bp));
        while(1);
    }
    
    if (bit_is_set(g_dev_state, DEV_STATE_FLG_REQ_ADDR_b)) {
        bldr_sboxnet_logon(pmsg);
        return 1;
    }
    return 0;
}

BLDR_APP_SECTION NOINLINE
static void bldr_main(uint8_t boot) {
    struct sboxnet_msg_max msg;
    uint8_t pagebuf[SPM_PAGESIZE];
   
    // enable boot section vector table
    ioreg_ccp(&PMIC.CTRL, Bit(PMIC_IVSEL_bp)|Bit(PMIC_MEDLVLEN_bp)); // only med level interrupts for sboxnet

    setbit_atomic(g_dev_state, DEV_STATE_FLG_BOOTLOADER_b);
    
    if (boot) {
        bldr_process_cmd_net_reset();
        
        sboxnet_init();
    }
    
    sei();
        
    while(1) {
        if (!bldr_task(&msg)) {
            if (sboxnet_receive_msg(&msg.msgh, sizeof(msg)) > 0) {
                if (bldr_process_msg(&msg.msgh, pagebuf) == SBOXNET_ACKRC_FWUP_FINISHED) {
                    while (!sboxnet_all_sent()); // wait until sboxnet transmitter is empty
                    sbldr_finish_fwup(bit_is_set(g_dev_state, DEV_STATE_FLG_FWUP_APPLICATION_b));
                
                    // never reached...
                }
            }
        }
        
        1; //sleep_cpu();
    }
}

BOOTLOADER_SECTION NOINLINE
void bldr_start(void) {
    // disable watchdog
    wdt_reset();
    ioreg_ccp(&WDT.CTRL, Bsv(WDT_ENABLE_bp,0)|Bsv(WDT_CEN_bp,1));
    
    bldr_init_system();
    
    g_v.timer_watchdog = 0;

    if (e2prom_get_byte(&bldr_eeprom.firmware_update) != 0) { // check Firmware Update flag in EEPROM address 0
        // enable app sector vector table: IVSEL == 0 app vector table
        ioreg_ccp(&PMIC.CTRL, Bit(PMIC_HILVLEN_bp)|Bit(PMIC_MEDLVLEN_bp)|Bit(PMIC_LOLVLEN_bp));
        __asm__ __volatile__ (
            "jmp  0\n"    // jump to application start code
        );
    }
    bldr_main(1);
    
    while(1);
}

BLDR_APP_SECTION NOINLINE __ATTR_USED
static void _bldr_activate(void) {
    cli();
    sbldr_e2prom_write(&bldr_eeprom.firmware_update, 0xff);
    // reboot to bootloader
    bldr_main(0);
    while(1);
}


/* Reserve 2 bytes in the bootloader section for the firmware update CRC.
 * Prevents the linker to use this space.
 */
BLDR_CRC_SECTION NOINLINE __ATTR_NAKED __ATTR_USED
static void bldr_crc_section(void) {
    __asm__ __volatile__ (
        "bldr_crc:  .word 0xffff\n"
    );
}



// boot loader static section ------

#define xsbldr_spm(_address) \
    __asm__ __volatile__ ( \
        "out %[cpuccp],%[ccpspm]\n" \
        "spm\n" \
        : /* no output */ \
        : [cpuccp] "I" (_SFR_IO_ADDR(CPU_CCP)), \
          [ccpspm] "r" (CCP_SPM_gc), \
          "z" ((uint16_t)(_address)) \
        : \
    )
#define xsbldr_spm_word(_address, _w) \
    __asm__ __volatile__ ( \
        "mov r0, %[wlo]\n" \
        "mov r1, %[whi]\n" \
        "out %[cpuccp],%[ccpspm]\n" \
        "spm\n" \
        "clr r1\n" \
        : /* no output */ \
        : [cpuccp] "I" (_SFR_IO_ADDR(CPU_CCP)), \
          [ccpspm] "r" (CCP_SPM_gc), \
          [wlo] "r" (lowbyte(_w)), \
          [whi] "r" (highbyte(_w)), \
          "z" ((uint16_t)(_address)) \
        : "r0","r1" \
    )
#define xsbldr_cmdex(_address) \
    __asm__ __volatile__ ( \
        "out %[cpuccp],%[ccpspm]\n" \
        "sts %[nvmctrl],%[nvmcmdex]\n" \
        : /* no output */ \
        : [cpuccp] "I" (_SFR_IO_ADDR(CPU_CCP)), \
          [ccpspm] "r" (CCP_IOREG_gc), \
          [nvmctrl] "i" (_SFR_MEM_ADDR(NVM_CTRLA)), \
          [nvmcmdex] "r" (Bit(NVM_CMDEX_bp)), \
          "z" ((uint16_t)(_address)) \
        : \
    )
BLDR_STATIC_SECTION __ATTR_USED NOINLINE
static uint8_t xsbldr_prog_page(uint16_t pageaddr, uint8_t *srcbuf, uint8_t fromflash) {
    nvmc_wait_busy();
    pageaddr &= ~(SPM_PAGESIZE-1);

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        NVM.CMD = NVM_CMD_ERASE_FLASH_BUFFER_gc;
        xsbldr_cmdex(pageaddr);
        NVM.CMD = NVM_CMD_NO_OPERATION_gc;
    }
    nvmc_wait_busy();
    
    for (uint16_t i = 0; i < SPM_PAGESIZE; i += 2)
    {
        // Set up little-endian word.
        uint16_t w;
        if (fromflash) {
            w = pgm_read_word((uint16_t)srcbuf + i);
        } else {
            w = *(uint16_t*)(srcbuf + i);
        }
        
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            NVM.CMD = NVM_CMD_LOAD_FLASH_BUFFER_gc;
            xsbldr_spm_word(pageaddr + i, w);
            NVM.CMD = NVM_CMD_NO_OPERATION_gc;
        }
    }
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (pageaddr >= BOOT_SECTION_START) {
            NVM.CMD = NVM_CMD_ERASE_WRITE_BOOT_PAGE_gc;
        } else {
            NVM.CMD = NVM_CMD_ERASE_WRITE_APP_PAGE_gc;
        }
        xsbldr_spm(pageaddr);
        NVM.CMD = NVM_CMD_NO_OPERATION_gc;
    }
    nvmc_wait_busy();
     
    // verify
    uint8_t ret = 0;
    for (uint16_t i = 0; i < SPM_PAGESIZE; i++, srcbuf++) {
        uint8_t v;
        if (fromflash) {
            v = pgm_read_byte((uint16_t)srcbuf);
        } else {
            v = *srcbuf;
        }
        if (pgm_read_byte(pageaddr + i) != v) {
            ret = 1; // verify error
            break;
        }
    }
    return ret;
}

BLDR_STATIC_SECTION __ATTR_USED NOINLINE
static void xsbldr_e2prom_write(uint8_t* addr, uint8_t v) {
    e2prom_wait_busy();
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (e2prom_get_byte((uint16_t)addr) != v) {
            clrbit(NVM.CTRLB, NVM_EEMAPEN_bp);
            // erase eeprom page buffer
            NVM.CMD = NVM_CMD_ERASE_EEPROM_BUFFER_gc;
            ioreg_ccp(&NVM.CTRLA, Bit(NVM_CMDEX_bp));
            nvmc_wait_busy();
            // load eeprom page buffer
            NVM.CMD = NVM_CMD_LOAD_EEPROM_BUFFER_gc;
            NVM.ADDR0 = lowbyte((uint16_t)addr);
            NVM.ADDR1 = highbyte((uint16_t)addr);
            NVM.ADDR2 = 0;
            NVM.DATA0 = v;
            // erase and write eeprom page
            NVM.CMD = NVM_CMD_ERASE_WRITE_EEPROM_PAGE_gc;
            ioreg_ccp(&NVM.CTRLA, Bit(NVM_CMDEX_bp));
            
            NVM.CMD = NVM_CMD_NO_OPERATION_gc;
        }
    }
    e2prom_wait_busy();
}

BLDR_STATIC_SECTION __ATTR_NO_RETURN __ATTR_USED NOINLINE
static void xsbldr_finish_fwup(uint8_t isapp) {
    cli();
    
    uint8_t verifyerr = 0;
    
    if (!isapp) {
        uint32_t pageaddr = BLDR_START_ADDR;
        uint32_t srcaddr  = 0;
        while (pageaddr < BLDR_START_ADDR + BLDR_SIZE) {
            verifyerr |= sbldr_prog_page(pageaddr, (uint8_t*)(uintptr_t)srcaddr, 1);
            pageaddr += SPM_PAGESIZE;
            srcaddr  += SPM_PAGESIZE;
        }
    }
    
    uint8_t upd = 0xff;
    if (verifyerr) {
        upd = 0xfe;
    } else if (isapp) {
        upd = 0;
    }
    sbldr_e2prom_write(&bldr_eeprom.firmware_update, upd);
    
    // all done, reboot
    ioreg_ccp(&RST.CTRL, Bit(RST_SWRST_bp));
    while(1);
}

BLDR_STATVEC_SECTION __ATTR_NAKED __ATTR_USED
static void sbldr_static_vectors(void) {
    __asm__ __volatile__ (
                         "0: rjmp 0b\n"  // Barrier fuer Amok laufende CPU
                         ".global sbldr_e2prom_write\n"
        "sbldr_e2prom_write: rjmp xsbldr_e2prom_write\n"
        "sbldr_finish_fwup:  rjmp xsbldr_finish_fwup\n"
        "sbldr_prog_page:    rjmp xsbldr_prog_page\n"
    );
}

