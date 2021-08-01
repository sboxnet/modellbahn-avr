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
 
 
// sind im Bootloader
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

/* Speicheraufteilung
 * ==================
 * 
 * ----------------------------------
 * - appcrc=6ffe
 * - Applikation CRC
 * ----------------------------------
 * - blrapp=7000
 * - Bootloader Applikation
 * ----------------------------------
 * - blrinit=8000
 * - Bootloader Init
 * ----------------------------------
 * - bldrvec=8200
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
 * - bootloader=8240
 * - Bootloader Section
 * ----------------------------------
 * - bldrcrc=8dfe
 * - Bootloader CRC
 * - Hier wird die CRC geschrieben die mit crc-ccitt.py berchnet wird aus dem .avr File
 * ----------------------------------
 * - bldrstatic=8e00
 * - Bootloader Static Statische Funktion in der Bootloader Sektion wie:
 * -  uint8_t sbldr_prog_page(uint16_t pageaddr, uint8_t *srcbuf, uint8_t fromflash);
 *     EEPROM Seite schreiben
 * -  void sbldr_finish_fwup(uint8_t isapp);
 *     Firmware Update Ende
 * -  void sbldr_e2prom_write(uint8_t* addr, uint8_t v);
 *     Byte in EEPROM schreiben
 * ----------------------------------
 * - bldrstatvec=8ff8
 * - Bootloader Zeiger aus die statischen Funktionen:
 * -  sbldr_e2prom_write
 * -  sbldr_finish_fwup
 * -  sbldr_prog_page
 * -----------------------------------
 * - bldrfwheader=9000
 * - Bootloader Firmware Header: hier liegt der Firmware Header (ausserhalb des Adressbereichs), siehe bldr_firmware_header.
 */

//-- Sektionen im Bootloader ----------------------------------------------------------

#undef BOOTLOADER_SECTION
// die Sektion für den Bootloader
#define BOOTLOADER_SECTION     __attribute__ ((section (".bootloader")))

// Zusätzlicher Code für Bootloader 
#define BLDR_APP_SECTION       __attribute__ ((section(".bldrapp")))
// Bootloader Vektoren
#define BLDR_VEC_SECTION       __attribute__ ((section(".bldrvec")))
// Bootloader Init (siehe bootloader-crt0.c)
#define BLDR_INIT_SECTION      __attribute__ ((section(".bldrinit")))
// Bereich für die CRC
#define BLDR_CRC_SECTION       __attribute__ ((section(".bldrcrc")))
// Bootloader statische Funktionen
#define BLDR_STATIC_SECTION    __attribute__ ((section(".bldrstatic")))
// Bootloader Zeiger auf die statischen Funktionen
#define BLDR_STATVEC_SECTION   __attribute__ ((section(".bldrstatvec")))
// hier ist der Firmware Header abgelegt
#define BLDR_FIRMWAREHEADER_SECTION __attribute__ ((section(".bldrfwheader")))

// 0x9000 - 0x200 + 1 = 0x8e01                              atxmega16a4  atxmega32a4
#define BLDR_STATIC_ADDR  (FLASHEND - 0x200 + 1)         // 0x4e00        0x8e00
#define BLDR_SIZE         0x1e00                         // 0x1e00        0x1e00
#define BLDR_START_ADDR   (BLDR_STATIC_ADDR - BLDR_SIZE) // 0x3000        0x7000


#define BLDR_PAGE_MAX  BLDR_START_ADDR  // in bytes!

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

// Bootloader Vektoren
BLDR_VEC_SECTION __ATTR_NAKED __ATTR_USED
static void bldr_vector_table(void) {    
    __asm__ __volatile__ (
        "  .globl bldr_activate\n"
        "bldr_activate:          jmp _bldr_activate\n"   // vector 0
        "  .globl bldr_task\n"
        "bldr_task:              jmp _bldr_task\n"       // vector 1
        "  .globl bldr_process_basic_msg\n"
        "bldr_process_basic_msg: jmp _bldr_process_basic_msg\n"  // vector 2
        "  .globl bldr_reg_read\n"
        "bldr_reg_read:          jmp _bldr_reg_read\n"   // vector 3
        "  .globl sbn_init\n"
        "sbn_init:               jmp sboxnet_init\n"// vector 4
        "  .globl sbn_receive_msg\n"
        "sbn_receive_msg:        jmp sboxnet_receive_msg\n"// vector 5
        "  .globl sbn_send_msg\n"
        "sbn_send_msg:           jmp sboxnet_send_msg\n"// vector 6
        "  .globl sbn_all_sent\n"
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

// Bootloader Firmware Header
BLDR_FIRMWAREHEADER_SECTION __ATTR_NAKED __ATTR_USED
static void bldr_firmware_header(void) {
    _FIRMWARE_HEADER(MAGIC_BOOTLOADER, 0, 0, BLDR_VERSION, BLDR_VERSION);
}

// forward declaration of BLDR_STATIC_SECTION functions
uint8_t sbldr_prog_page(uint16_t pageaddr, uint8_t *srcbuf, uint8_t fromflash);
__ATTR_NO_RETURN void sbldr_finish_fwup(uint8_t isapp);
void sbldr_e2prom_write(uint8_t* addr, uint8_t v);


// Symbol das durch den Linker defniert ist.
// Wenn nur der Bootloader gelinkt ist, das Symbol zeigt auf INTERNAL_SRAM_START.
// Ist eine Applikation gelinkt, _end verweist auf das Ende von BSS.
extern uint8_t _end;

// BLDR_STACK_TOP = ((INTERNAL_SRAM_END - sizeof(struct bldr_ram) - 1) - 1)
#define STACK_MAGIC   0
// stack wird mit 0 gefüllt
//BOOTLOADER_SECTION
//BLDR_APP_SECTION
static uint16_t bldr_stack_free(void) {
    uint8_t* p = &_end;
    // prüfe abwärts ob was anderes als eine 0 auf dem Stack liegt
    while (p < (uint8_t*)BLDR_STACK_TOP) {
        if (*p != STACK_MAGIC) {
            break;
        }
        p++;
    }
    return (uint16_t)(p - &_end);
}
// Stack Grösse
//BOOTLOADER_SECTION
//BLDR_APP_SECTION
static uint16_t bldr_stack_size(void) {
    return (uint8_t*)BLDR_STACK_TOP - &_end;
}
 /*
BOOTLOADER_SECTION
void write_errinfo(uint8_t code) {
    // get cur index
    uint8_t ci = e2prom_get_byte(&bldr_eeprom.errinfo[0]);
    ci++;
    sbldr_e2prom_write(&bldr_eeprom.errinfo[ci], code);
    ci++;
    sbldr_e2prom_write(&bldr_eeprom.errinfo[0], ci);
}
*/
/*BOOTLOADER_SECTION 
void write_errinfo(uint8_t code, uint8_t *errstr) {
    uint8_t i = 0;
    uint8_t c = 0;
    uint8_t *p = &bldr_eeprom.errinfo[0];
    uint8_t f = 0;
    uint8_t *e2end = &bldr_eeprom.errinfo[0];
    
    // find code
    for (;;) {
        uint8_t *e2addr = p+i;
        c = e2prom_get_byte(e2addr);
        if (c == code) {
            f = 1;
            break;
        }
    }
    // code not found
    if (!f) {
        // find last 0
        for (i = 0;;i++){
            c = e2prom_get_byte(p+i);
            if ( c == 0 ){
                e2end = p + i;
                break;
            }
        }
        // then insert it
        sbldr_e2prom_write(e2end, code);
        e2end++;
        for (i = 0; (c = errstr[i]) != 0; i++) {
            sbldr_e2prom_write(e2end++, c);
        }
        sbldr_e2prom_write(e2end+1, 0);
    }
}
*/

// Status LEDs
#define LED_PORT     PORTE // LED Port
#define LED_RED_b    1  // rote LED (Bitnummer)
#define LED_GREEN_b  0  // grüne LED (Bitnummer)

// "Identify" Bit: teilt sich mit der roten LED ein Bit am LED Port 
#define KEY_ID_b     0

/*--- SBOXNET -----------------------------------------
 * - benutzt Timer TCE0
 * - OVF und CCA Interrupt -> bootloader.c
 * - benutzt USARTE0 für die Kommunikation
 * - 250kBaud
 * - 9Bits, 1 Stopbit, Gerade Parität
 * - Multiprocessor Communication Mode
 * - über ein CAN Leitung wird gesendet und empfangen
 * - über den RxD Port wird die Leitung überwacht
 */
// SBOXNET Timer Prescaler: 32Mhz / 8
#define SBOXNET_TC_PRESCALER TC_CLKSEL_DIV8_gc
// SBOXNET Timer Periode, entspricht ca 244 Hz.
#define SBOXNET_TC_PERIOD    (16384-1) // 0x3fff

// hier wird die Geräte Adresse abgelegt im bootloader RAM -> common.h
#define SBOXNET_DEVICE_ADDR    g_v.dev_addr

// der sboxnet_device Bereich für sboxnet im Bootloader RAM
#define SBOXNET_VAR                   (&g_v.sboxnet)
// die Sektion für Sboxnet Standardsektion für Sboxnet
#define SBOXNET_SECTION               //BLDR_APP_SECTION
// 2.te Sektion für Sboxnet
#define SBOXNET_SECTION2              //BLDR_APP_SECTION
// Port für Receiver Bit (RxD)
#define SBOXNET_RxD_PORT              PORTE
// RxD Bitnummer im Port
#define SBOXNET_RxD_BIT               2
// Port für Transmitter (TxD)
#define SBOXNET_TxD_PORT              PORTE
// TxD Bitnummer im Port
#define SBOXNET_TxD_BIT               3
// der USART Receiver Interrupt Vector
#define SBOXNET_USART_RxC_vect        USARTE0_RXC_vect
// der USART Transceiver Interrupt
#define SBOXNET_USART_TxC_vect        USARTE0_TXC_vect
// Medium Interrupt Levels
#define SBOXNET_USART_RXCINTLVL       USART_RXCINTLVL_MED_gc //Receiver
#define SBOXNET_USART_TXCINTLVL       USART_TXCINTLVL_MED_gc //Transmitter
//benutzter Timer
#define SBOXNET_TIMER                 TCE0
// CC Kanale
#define SBOXNET_TIMER_CC              CCA
// CC Kanal Buffer
#define SBOXNET_TIMER_CCBUF           CCABUF
// CC Interrupt Flag
#define SBOXNET_TIMER_CCIF            TC0_CCAIF_bp
// CC Interrupt Level Maske
#define SBOXNET_TIMER_CCINTLVL_MASK   TC0_CCAINTLVL_gm
// CC Interupt Level
#define SBOXNET_TIMER_CCINTLVL        TC_CCAINTLVL_MED_gc
// Wieviel Timer Bits sollen vergehen
#define SBOXNET_TIMER_BIT_TICKS       32
// Maske für CC Wert
#define SBOXNET_TIMER_PERIOD_MASK     SBOXNET_TC_PERIOD
// Maske für Random Wert
#define SBOXNET_TIMER_RANDOM_MASK     0x7f
// Hardware Sensor Port
#define SBOXNET_HW_SENSOR_PORT        PORTE
// Hardware Sensor ist an RxD Pin
// RxD Sensor port interrupt level maske
#define SBOXNET_HW_SENSOR_INTLVL_MASK PORT_INT0LVL_gm
// RxD Sensor port interrupt 0
#define SBOXNET_HW_SENSOR_INTMASK     INT0MASK
// Sensor Interupt Flag
#define SBOXNET_HW_SENSOR_INTF_BIT    PORT_INT0IF_bp
// USART der genutzt werden soll
#define SBOXNET_USART_ID              USARTE0 // use defines for __SELECT_USART == 0
// Definiere Interrupts
#define SBOXNET_USART_DEF_INTR        1
// Sboxnet Randomfunktion
#define SBOXNET_PRNG_SEED             (g_v.prandom)
// den Code für Sboxnet einbinden
#include "sboxnet.c"

//BOOTLOADER_SECTION
uint8_t g_dev_state = 0;

//BOOTLOADER_SECTION
uint8_t g_dev_errflags = 0;

//BOOTLOADER_SECTION
uint8_t g_dev_errflags2 = 0;

struct bldr_ram g_v;

// TCE0 CCA Interrupt
//SBOXNET_SECTION ISR(TCC0_CCA_vect) {
#ifdef WITH_SECS
BLDR_APP_SECTION
#endif
ISR(TCE0_CCA_vect) {
    sboxnet_timer_bit(); // Sboxnet Timer pro Bit
}

// TCE0 OVF Interrupt, Timer verwalten
//BOOTLOADER_SECTION ISR(TCC0_OVF_vect) { // every 4ms
#ifdef WITH_SECS
BLDR_APP_SECTION
#endif
ISR(TCE0_OVF_vect) { // every 4ms
    // Timer im bldr_ram erhöhen
    ++g_v.timer;
    // wenn Logon timer gesetzt ist -> dec
    if (g_v.timer_logon) {
        g_v.timer_logon--;
    }
    // wenn LEDs timer gesetzt ist -> dec
    // nach welcher Zeit sollen die Status LED geprüft werden
    // 32*4ms = 128ms
    if (g_v.timer_leds) {
        g_v.timer_leds--;
    }
    // wenn Key Timer gesetzt ist -> dec
    // wann soll der Identify Key geprüft werden
    // 10*4ms = 40ms
    if (g_v.timer_keys) {
        g_v.timer_keys--;
    }
    // wenn eine Nachricht von der Adresse 0 gekommen ist -> wird timer_watchdog = WATCHDOG_VAL
    // wenn Watchdog Timer gesetzt ist
    if (g_v.timer_watchdog) {
        // Lösche Status Bit für Wartchdog
        clrbit_atomic(g_dev_state, DEV_STATE_FLG_WATCHDOG_b);
        // dec Watchdog timer
        g_v.timer_watchdog--;
    } else {
        // ansonsten setzte Statusbit für Watchdog
        setbit_atomic(g_dev_state, DEV_STATE_FLG_WATCHDOG_b);
    }
}

void timer_register(struct timer* t, uint8_t resolution) {
	if (resolution == TIMER_RESOLUTION_16MS) {
		t->next = g_com.timer_16ms;
		g_com.timer_16ms = t;
		} else {
		t->next = g_com.timer_1ms;
		g_com.timer_1ms = t;
	}
	t->value = 0;
}

uint8_t timer_timedout(struct timer* t) {
	return t->value == 0;
}

void timer_set(struct timer* t, int8_t value) {
	t->value = value;
}


/* void bldr_leds_task(void)
 * Status LEDs ansteuern.
 */
#ifdef WITH_SECS
BLDR_APP_SECTION
#endif
static void bldr_leds_task(void) {
    
    // Zuerst Identify Key lesen.
    // Wann der Identify Key gedrückt ist -> Bit 1 von Port E LOW
	
	if (g_v.timer_keys == 0) { // alle 40ms
        
		// Direction von PORTE Bit 1 == LED_RED_b
        
        // Direction von PORTE Bit 1?
		// bei PE1 == 0
         if (bit_is_clear(port_dir(LED_PORT), LED_RED_b)) {
            // ist Eingabe, setzte timer_keys auf 10
            g_v.timer_keys = 10;
            // lese PE1, wenn gesetzt Bit KEY_ID_b
            uint8_t sw = bit_is_set(port_in(LED_PORT), LED_RED_b) ? 0 : Bit(KEY_ID_b);
            // PE1 auf Ausgabe
            port_dirout(LED_PORT, Bit(LED_RED_b));
            // Bit entprellen
            debounce_keys(&g_v.key_id, &g_v.key_id_t, sw);
        } else {
            // ist Ausgabe, dann nächstenmal prüfen
            g_v.timer_keys = 1;
            // PE1 als Eingabe
            port_dirin(LED_PORT, Bit(LED_RED_b));
        }
    }
    
    // alle 128 ms
    if (g_v.timer_leds == 0) {
        g_v.timer_leds = 32; // 32 * 4ms = 128ms
        
        // ist der entprellte Identify Bit gesetzt?
        if (bit_is_set(g_v.key_id, KEY_ID_b)) {
            // ist der Status in g_v.key_id_state NICHT gesetzt?
            if (bit_is_clear(g_v.key_id_state, KEY_ID_b)) {
                // dann setzen im g_v.key_id_state
                setbit(g_v.key_id_state, KEY_ID_b);
                ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { // Atomare Operation
                    // Bit DEV_STATE_FLG_IDENTIFY_b im g_dev_state togglen
                    g_dev_state ^= Bit(DEV_STATE_FLG_IDENTIFY_b);
                }
            }
        } else {
            // ist Identify Key NIcht gedrückt -> key_id_state zurücksetzen 
            clrbit(g_v.key_id_state, KEY_ID_b);
        }
        
        // Status überprüfen
        volatile uint8_t st = g_dev_state; // aktueller Status
		volatile uint8_t st_old = g_v.dev_old_state;
		if (st != st_old) { // ist aktueller STatus != alter Status
			
            g_v.dev_old_state = st; // dann alter Status gleich aktueller Status
           
            // Ist Identify Flag gesetzt?
            if (bit_is_set(st, DEV_STATE_FLG_IDENTIFY_b)) {
                // ja, LEDs abwechseln Blinken
                g_v.led_gn = 0xaa;
                g_v.led_rt = 0x55;
            // Wird eine Adresse angefordert?
            } else if (bit_is_set(st, DEV_STATE_FLG_REQ_ADDR_b)) {
                // rote LED blinken
                g_v.led_gn = 0;
                g_v.led_rt = 0x55;
				
            // sind wir im Firmware Update Modus?
            /*} else if (bit_is_set(st, DEV_STATE_FLG_FWUP_MASK)) {
                // grüne LED schnell blinken
                g_v.led_gn = 0xaa;
                g_v.led_rt = 0;*/
            // ist der bootloader aktiv?
            } else if (bit_is_set(st, DEV_STATE_FLG_BOOTLOADER_b)) {
                // grüne LED langsam Blinken
                g_v.led_gn = 0x3e;
                g_v.led_rt = 0;
            } else {
                // absonsten grün LED Kurz Blinken
                g_v.led_gn = 0x18;
                g_v.led_rt = 0;
            }

            // war eine Watchdog Nachricht und keine Adressanforderung
			/*if (bit_is_set(st, DEV_STATE_FLG_WATCHDOG_b) && !bit_is_set(st, DEV_STATE_FLG_REQ_ADDR_b)) {
                // rote LED Blinken
				g_v.led_rt = 7;
			}*/
        }
        // war ein Fehler (Bits in g_dev_errflags oder g_dev_errflags2 gesetzt, DEV_ERR_FLG_*)
        if (g_dev_errflags || g_dev_errflags2) {
            // rote LED dauerhaft Ein
            g_v.led_rt = 0xff;
        }
        // LEDs ansteuern
        // werden über Bitmaske angesteuert
        // der Wert in led_gn/rt wird nach links gerollt, wenn Bit 7 gesetzt ist --> LED ein
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
		uint8_t a = 0;
    }
}

/* uint16_t crc_ccitt_update(uint16_t crc, uint8_t data)
 * einen CRC Wert berechnen. Fügt ein Byte hinzu.
 * Muss explizit in der bldr_app_section sein! _crc_ccitt_update ist eine "static inline", aber der Compiler kann eine Funktion erzeugen die in der .text Sektion liegt.
 * 
 * siehe avr libc https://www.nongnu.org/avr-libc/user-manual/group__util__crc.html, https://www.nongnu.org/avr-libc/user-manual/group__util__crc.html#ga1c1d3ad875310cbc58000e24d981ad20
 */
#ifdef WITH_SECS
BLDR_APP_SECTION
#endif
static uint16_t crc_ccitt_update(uint16_t crc, uint8_t data) {
    return _crc_ccitt_update(crc, data);
}

/* uint8_t _bldr_process_basic_msg(struct sboxnet_msg_header *pmsg)
 * Basis Nachrichten verarbeiten:
 *  - SBOXNET_CMD_DEV_FW_UPD_START: Firmware Update Start
 *  - SBOXNET_CMD_DEV_FW_UPD_LOAD: Firmware Update Laden -> erst SBOXNET_ACKRC_FWUP_INACTIVE
 *  - SBOXNET_CMD_DEV_GET_DESC: Gerätebeschreibung lesen
 *  - SBOXNET_CMD_DEV_IDENTIFY: Identify Flag setzen
 */
#ifdef WITH_SECS
BLDR_APP_SECTION
#endif
NOINLINE __ATTR_USED
static uint8_t _bldr_process_basic_msg(struct sboxnet_msg_header *pmsg) {
    switch (pmsg->cmd) {        
        /* Firmware Update Start
         * Ein Firmware Update starten.
         */
        case SBOXNET_CMD_DEV_FW_UPD_START: {
            // Zeiger auf Daten in der Nachricht
            uint16_t* pdata = (uint16_t*)pmsg->data;
            // Nachrichtlänge
            uint8_t msglen = pmsg->opt.len;
            // Statusflags löschen:
            // Flag Bootloader
            // Flag Applikation
            clrbit_atomic(g_dev_state, DEV_STATE_FLG_FWUP_BOOTLOADER_b);
            clrbit_atomic(g_dev_state, DEV_STATE_FLG_FWUP_APPLICATION_b);
            // ist die Nachrichtenlänge OK?
            // Daten:
            //  - LOW Byte Adresse
            //  - High Byte Adresse
            //  - Flags
            //  - 32 Daten-Bytes 
            if (msglen != (3+32)) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            // Grösse der Daten
            uint16_t size = pdata[0];
            // Zeiger auf Firmware Header
            struct firmware_header* pfwh = (struct firmware_header*)(pmsg->data + 3);
            // sind die Daten ein Bootloader?
            if (pfwh->magic == MAGIC_BOOTLOADER) {
                // ist Bit 0 gesetzt , dann kein Bootloader
                if (bit_is_set(pmsg->data[2], 0)) {
                    return SBOXNET_ACKRC_FWUP_INVMODE;
                }
                // sind die Daten ungleich BLDR_SIZE (0x1e00)
                if (size != BLDR_SIZE) {
                    return SBOXNET_ACKRC_FWUP_OVERFLOW;
                }
                // dann ist es ein Bootloader
                setbit_atomic(g_dev_state, DEV_STATE_FLG_FWUP_BOOTLOADER_b);
            
            // ist eine Applikation?
            } else if (pfwh->magic == MAGIC_APPLICATION) {
                // ist Bit 0 gelöscht, dann keine Applikation
                if (bit_is_clear(pmsg->data[2], 0)) {
                    return SBOXNET_ACKRC_FWUP_INVMODE;
                }
                // Product ID aus aktuell EEPROM holen
                uint16_t e_prdid = e2prom_get_word(&bldr_eeprom.productid);
                // Vendor ID aus aktuell EEPROM holen
                uint16_t e_venid = e2prom_get_word(&bldr_eeprom.vendorid);
                // sind Product ID und Vendor ID nicht gesetzt?
                if (e_prdid == 0xffff && e_venid == 0xffff) {
                    // Applikation noch nie geladen
                    
                    // stimmen Firmware Product und Vendor ID mit dieser Applikation überein?
                } else if (pfwh->productid != e_prdid || pfwh->vendorid != e_venid) {
                    return SBOXNET_ACKRC_FWUP_INVDEV;
                }
                // sind die Daten > als Maximum EEPROM Seite?
                if (size == 0 || size >= BLDR_PAGE_MAX) {
                    return SBOXNET_ACKRC_FWUP_OVERFLOW;
                }
                // dann ist es eine Applikation Firmware Update
                setbit_atomic(g_dev_state, DEV_STATE_FLG_FWUP_APPLICATION_b);
            }

            // Startwerte für pageaddress und pageoffs
            g_v.fwup_pageaddr = 0;
            g_v.fwup_pageoffs = 0;
            g_v.fwup_size = size;
            
            // CRC Init
            uint16_t crc = 0xffff;
            // CRC berechnen
            uint8_t* psrc = (uint8_t*)pfwh;
            for (uint8_t length = 32; length > 0; length--) {
                crc = crc_ccitt_update(crc, *psrc);
                psrc++; // produces better code ...
            }
            g_v.fwup_crc = crc;

            pmsg->opt.len = 0;
            return SBOXNET_ACKRC_OK;
        }
        
        // SBOXNET_CMD_DEV_FW_UPD_LOAD: erstmal RC=SBOXNET_ACKRC_FWUP_INACTIVE
        // siehe bldr_process_std_msg()
        case SBOXNET_CMD_DEV_FW_UPD_LOAD: {
            return SBOXNET_ACKRC_FWUP_INACTIVE;
        }
        
        // Gerätebeschreibung lesen
        case SBOXNET_CMD_DEV_GET_DESC: {
            // ID der Beschreibung
            uint8_t id = 0;
            // Ist die Nachrichtenlänge gültig?
            if (pmsg->opt.len > 1) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            // ID holen
            if (pmsg->opt.len == 1) {
                id = pmsg->data[0];
            }
            // Ist ID gültig?
            if (id >= DEV_NUM_DESCS) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            // Zeiger auf Beschreibung im EEPROM
            uint8_t* pdesc = bldr_eeprom.dev_desc[id];
            // Beschreibung Byte-weise lesen, bis eine 0 oder 0xff 
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
        
        // Identify Bit setzen
        case SBOXNET_CMD_DEV_IDENTIFY: {
            // Nachrichtenlänge gültig?
            if (pmsg->opt.len != 1) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            // ON oder OFF
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

/* uint8_t _bldr_reg_read(uint16_t reg, uint16_t *pdata)
 * Standard-Register lesen.
 * - R_PUID_L: Low von der Pseudo UID
 * - R_PUID_H: High von der Pseudo UID
 * - R_PRODUCTID: Product ID
 * - R_VENDORID: Vendor ID
 * - R_BOOTLOADER_VERSION: Version des Bootloaders
 * - R_PROTOKOLL_VERSION: Sboxnet Protokoll Version
 * - R_DEV_ADDR: Geräteadresse
 * - R_DEV_STATE: Gerätezustand
 * - R_DEV_ERROR: Geräte Error flags (g_dev_errflags|g_dev_errflags2)
 * - R_DEV_BOOTSTATE: soll in die Applikation gebootet werden? 0=ja <>0=im bootloader beleiben
 * - R_DEV_NUM_DESCS: Anzahl der Beschreibungen
 * - R_DEBUG_BLDR_RAM_OFFS: DEBUG Offset der Bootloader Variablen (g_v)
 * - R_DEBUG_COM_RAM_OFFS:  DEBUG Offset der COMMON Variablen
 * - R_DEBUG_SBN_RAM_OFFS:  DEBUG Offset der Variablen für Sboxnet im Internal SRAM
 * - R_DEBUG_BLDR_EEPROM_OFFS: DEBUG Offset der Bootloader Variablen im EEPROM
 * - R_DEBUG_SRAM_SIZE:     DEBUG Grösse des Internen RAMs
 * - R_DEBUG_EEPROM_SIZE:   DEBUG Grösse des EEPROMs
 * - R_DEBUG_STACK_SIZE:    DEBUG Grösse des Stacks
 * - R_DEBUG_STACK_FREE:    DEBUG Freier Platz im Stack
 * - R_DEBUG_SBN_STATE:     DEBUG Status vom Sboxnet (SBOXNET_STATE_*)
 * - R_DEBUG_SBN_FLAGS:     DEBUG Flags vom Sboxnet (SBOXNET_FLAG_*)
 * - R_DEBUG_SBN_0..R_DEBUG_SBN_15: Debug Register lesen
 * - R_DEBUG_EEPROM_BASE..    DEBUG EEPROM lesen (Wortweise)
 * - R_DEBUG_SRAM_BASE..      DEBUG SRAM lesen (Wortweise)
 */
#ifdef WITH_SECS
BLDR_APP_SECTION
#endif
NOINLINE __ATTR_USED
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
        case R_DEV_ERROR: *pdata = g_dev_errflags|g_dev_errflags2; return 0;
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

/* Kommando Register (fortlaufend) ab erstem Register lesen
 * uint8_t bldr_cmd_reg_read(struct sboxnet_msg_header *pmsg)
 *   pmsg Zeiger auf Nachricht
 */
#ifdef WITH_SECS
BLDR_APP_SECTION
#endif
static uint8_t bldr_cmd_reg_read(struct sboxnet_msg_header *pmsg) {
    // zeiger auf Nachricht Daten
    uint16_t *pdata = (uint16_t*)pmsg->data;
    // Registernummer
    uint16_t regaddr = *pdata;
    // Anzahl der Register
    uint8_t num = pmsg->data[2];
    // Ist die Registeranzahl gültig?
    if (pmsg->opt.len != 3 || num > SBOXNET_MSG_DATA_LEN/2 ) {
        return SBOXNET_ACKRC_INVALID_ARG;
    }
    // Pro Register lesen
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

/* Kommando: mehrere Register lesen
 * uint8_t bldr_cmd_reg_read_multi(struct sboxnet_msg_header *pmsg)
 *  pmsg  Zeiger auf Nachricht
 */
#ifdef WITH_SECS
BLDR_APP_SECTION
#endif
static uint8_t bldr_cmd_reg_read_multi(struct sboxnet_msg_header *pmsg) {
    // Zeiger auf Nachricht
    uint16_t *pdata = (uint16_t*)pmsg->data;
    // ist Nachricht gültig?
    if (pmsg->opt.len > SBOXNET_MSG_DATA_LEN || (pmsg->opt.len & 0x01)) {
        return SBOXNET_ACKRC_INVALID_ARG;
    }
    // pro Register lesen
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

/* Führe die Aktion NET Reset aus:
 * void bldr_process_cmd_net_reset(void)
 * - setzt als Geräteadresse auf die Broadcastadresse
 * - setzt State Flag DEV_STATE_FLG_REQ_ADDR_b, erwarte Adresse
 */
#ifdef WITH_SECS
BLDR_APP_SECTION
#endif
static void bldr_process_cmd_net_reset(void) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        g_v.dev_addr = SBOXNET_ADDR_BROADCAST;
        setbit(g_dev_state, DEV_STATE_FLG_REQ_ADDR_b);
    }
}

/* Behandele Standard Nachricht.
 * uint8_t bldr_process_std_msg(struct sboxnet_msg_header *pmsg, uint8_t* pagebuf)
 *  pmsg Zeiger auf Nachricht
 *  pagebuf Buffer für Flash Seite.
 * 
 * Nachrichten:
 * - SBOXNET_CMD_NET_RESET: Verarbeite NETZ Reset
 * - SBOXNET_CMD_NET_WATCHDOG: Watchdog Ping
 * - SBOXNET_CMD_DEV_RESET: Geräte Reset
 * - SBOXNET_CMD_REG_READ: Register lesen
 * - SBOXNET_CMD_REG_READ_MULTI: mehrere Register lesen
 * - SBOXNET_CMD_DEV_FW_UPD_LOAD: Firmware Laden
 * wenn nichts zugetroffen hat: weiter zu bldr_process_basic_msg()
 * 
 */
#ifdef WITH_SECS
BLDR_APP_SECTION
#endif
static uint8_t bldr_process_std_msg(struct sboxnet_msg_header *pmsg, uint8_t* pagebuf) {
    switch (pmsg->cmd) {
        // Nachricht SBOXNET_CMD_NET_RESET
        case SBOXNET_CMD_NET_RESET: {
            // ein Netz Reset
            if (pmsg->srcaddr == 0) {
                // nur wenn die Quelle der Master == 0 ist, Netz reset verarbeiten
                bldr_process_cmd_net_reset();
            }
            return SBOXNET_ACKRC_SEND_NO_ANSWER;
        }
        // Nachricht SBOXNET_CMD_NET_WATCHDOG
        case SBOXNET_CMD_NET_WATCHDOG: {
            // Watchdog Ping
            return SBOXNET_ACKRC_SEND_NO_ANSWER;
        }
        // Nahcricht SBOXNET_CMD_DEV_RESET: Geräte Reset
        case SBOXNET_CMD_DEV_RESET: {
            // Reset Flag setzen
            setbit_atomic(g_dev_state, DEV_STATE_FLG_RESET_b);
            // wenn Ziel Broadcast Adresse ist, keine Antwort
            if (pmsg->dstaddr == SBOXNET_ADDR_BROADCAST) {
                return SBOXNET_ACKRC_SEND_NO_ANSWER;
            } else {
                // Ansonsten OK
                pmsg->opt.len = 0;
                return SBOXNET_ACKRC_OK;
            }
        }
    }
    
    // ist das Ziel die Broadcast Adresse, keine Antwort
    if (pmsg->dstaddr == SBOXNET_ADDR_BROADCAST) {
        return SBOXNET_ACKRC_SEND_NO_ANSWER;
    }

    switch (pmsg->cmd) {
        // Nachricht SBOXNET_CMD_REG_READ: Register lesen
        case SBOXNET_CMD_REG_READ: {
            // weiter zu bldr_cmd_reg_read()
            return bldr_cmd_reg_read(pmsg);
        }
        // Nachricht SBOXNET_CMD_REG_READ_MULTI: mehrere Register lesen
        case SBOXNET_CMD_REG_READ_MULTI: {
            // weiter zu bldr_cmd_reg_read_multi()
            return bldr_cmd_reg_read_multi(pmsg);
        }
        // Nachricht SBOXNET_CMD_DEV_FW_UPD_LOAD
        case SBOXNET_CMD_DEV_FW_UPD_LOAD: {
            // Firmware laden
            // es muss vorher SBOXNET_CMD_DEV_FW_UPD_START ausgeführt worden sein!
            // Ist Gerät im Firmware Update Modus?
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
            // Firmware schreiben
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
            // weiter zu bldr_process_basic_msg()
            return bldr_process_basic_msg(pmsg);        
    }
    return SBOXNET_ACKRC_CMD_UNKNOWN;
}

/* Nachrichten verarbeiten.
 * uint8_t NOINLINE bldr_process_msg(struct sboxnet_msg_header* pmsg, uint8_t* pagebuf)
 * - pmsg  Zeiger auf Nachricht
 * - pagebuf  Buffer für Flash Seite
 */
#ifdef WITH_SECS
BLDR_APP_SECTION
#endif
static uint8_t NOINLINE bldr_process_msg(struct sboxnet_msg_header* pmsg, uint8_t* pagebuf) {    
    uint8_t rc = 0;
    
    // ist die Quelladresse == 0, dann Watchdog Timer setzen
    if (pmsg->srcaddr == 0) {
        g_v.timer_watchdog = WATCHDOG_VAL; // 128 entspricht 4ms * 128 = 0,5 s
    }

    if (pmsg->cmd & 0x80) {
        return rc;  // Ist Bit 7 im Kommando gesetzt, ist es eine Anwort
    }
    // Standard Nachricht verarbeiten
    rc = bldr_process_std_msg(pmsg, pagebuf);
    if (rc == SBOXNET_ACKRC_SEND_NO_ANSWER) {
        return rc;
    }
    if (rc == SBOXNET_ACKRC_OK || rc == SBOXNET_ACKRC_FWUP_FINISHED) {
        goto send_ok;
    }
    
//send_error:
// Fehler melden
    pmsg->opt.len = 2;
    pmsg->data[0] = pmsg->cmd; // Original Nachricht
    pmsg->data[1] = rc;        // Rückgabewert
    pmsg->cmd = 0x80; // als Antwort markieren

send_ok:
// Kommando OK
    // Zurück an Absender
    pmsg->dstaddr = pmsg->srcaddr;
    pmsg->cmd |= 0x80; // Bit 7 im Kommando setzen
    sboxnet_send_msg(pmsg); // und senden
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

/* Random Funktion
 * xor shift Random Generator.
 * siehe https://de.wikipedia.org/wiki/Xorshift
 */
//BOOTLOADER_SECTION
BLDR_APP_SECTION
static uint16_t bldr_random(void) {
    uint16_t r;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        r = prng_xorshift16(g_v.prandom);
        g_v.prandom = r;
    }
    return r;
}

/* Bootloader System Init.
 */
BOOTLOADER_SECTION
static void bldr_init_system(void) {
    // Pins von PORTA als Eingabe
    PORTA.DIR = 0;
    // Portcfg auf alle Pins
    PORTCFG_MPCMASK = 0xff; // all pins
    // pull down ein
    PORTA.PIN0CTRL = PORT_OPC_PULLDOWN_gc;
    
    // Pins von PORTB als Eingabe
    PORTB.DIR = 0;
    // Portcfg auf alle Pins
    PORTCFG_MPCMASK = 0xff; // all pins
    // pull down ein
    PORTB.PIN0CTRL = PORT_OPC_PULLDOWN_gc;
    
    // Pins von PORTC als Eingabe
    PORTC.DIR = 0;
    // Portcfg auf alle Pins
    PORTCFG_MPCMASK = 0xff; // all pins

    // pull down ein
    PORTC.PIN0CTRL = PORT_OPC_PULLDOWN_gc;
    
    // Pins von PORTD als Eingabe
    PORTD.DIR = 0;
    // Portcfg auf alle Pins
    PORTCFG_MPCMASK = 0xff; // all pins
    // pull down ein
    PORTD.PIN0CTRL = PORT_OPC_PULLDOWN_gc;

    PORTE.DIR = 0;
    PORTCFG_MPCMASK = 0xff; // all pins
    PORTE.PIN0CTRL = PORT_OPC_PULLUP_gc;
    
    // Virtual Port Maps: VP0: PORTA, VP1: PORTB, VP2: PORTC, VP3: PORTD
    PORTCFG_VPCTRLA = PORTCFG_VP02MAP_PORTB_gc|PORTCFG_VP02MAP_PORTA_gc;
    PORTCFG_VPCTRLB = PORTCFG_VP02MAP_PORTD_gc|PORTCFG_VP02MAP_PORTC_gc;

    // Teste ob Chip ein USB Gerät hat?
#ifdef USB_SPEED_bp
    // hat ein USB --> atxmega32a4u
    g_v.is_atxmega32a4u = 1;
#else
    // wenn nicht ein normaler atxmega32a4
    g_v.is_atxmega32a4u = 0;
#endif

    // Status LEDs, einschalten
    port_dirout(PORTE, 0x03); // LEDs Ports als Ausgabe
    port_clr(PORTE, 0x03); // LED einschalten
    g_v.led_gn = 0; // grüne LED, wird links rotiert, & 0x80 ==> LED wird eingeschaltet
    g_v.led_rt = 0; // rote LED,  wird links rotiert, & 0x80 ==> LED wird eingeschaltet
    
    // Init Werte setzen
    g_dev_state = 0; // Gerätestatus GPIO_GPIOR0
    g_dev_errflags = 0; // Gerätefehler GPIO_GPIOR1
    g_dev_errflags2 = 0; // Gerätefehler2 GPIO_GPIOR2

    // Chip von 2MHz auf 32MHz umschalten
    bldr_switch_to_32MHz();

    // Powersave
#ifdef PR_USB_bp
    ;
    // unbenutzte Devices abschalten
	//PR.PRGEN = Bit(PR_USB_bp)|Bit(PR_AES_bp)|Bit(3/*PR_EBI_bp*/)|Bit(PR_RTC_bp); // 
    //PR.PRPE = Bit(PR_TWI_bp)|Bit(PR_USART1_bp)/*|Bit(PR_USART0_bp)*/|Bit(PR_SPI_bp)|Bit(PR_HIRES_bp)|Bit(PR_TC1_bp); // PR_TC0_bp;
#else
    // unbenutzte Devices abschalten
    PR.PRGEN = Bit(PR_AES_bp)|Bit(PR_EBI_bp)|Bit(PR_RTC_bp);
    PR.PRPE = Bit(PR_TWI_bp)|Bit(PR_USART1_bp)/*|Bit(PR_USART0_bp)*/|Bit(PR_SPI_bp)|Bit(PR_HIRES_bp)|Bit(PR_TC1_bp); // PR_TC0_bp;
#endif
    // die Pseudo UID holen
    g_v.dev_puid = e2prom_get_long(&bldr_eeprom.puid);
    // Start Geräteadresse 255, wird über SBOXNET_CMD_DEV_REQ_ADDR angefordert und über SBOXNET_CMD_DEV_SET_ADDR gesetzt.
    g_v.dev_addr = 0xff;
    
    // Variablen für Firmware Update
    g_v.fwup_pageaddr = 0;
    g_v.fwup_pageoffs = 0;
    g_v.fwup_size = 0;
    
    // Seed für Random Generator
    g_v.prandom = (uint16_t)g_v.dev_puid;
    // Init timer mit Zufallszahl
    g_v.timer = (uint8_t)bldr_random();
}

/* Sboxnet Logon.
 * 
 */
#ifdef WITH_SECS
BLDR_APP_SECTION
#endif
static void bldr_sboxnet_logon(struct sboxnet_msg_max* pmsg) {
    uint32_t *ppuid = (uint32_t*)pmsg->data;
    uint16_t *pprodid = (uint16_t*)(pmsg->data + 4);
    uint16_t *pvendid = (uint16_t*)(pmsg->data + 6);
    
    // Product ID und Vendor ID des Gerätes ermitteln aus dem EEPROM
    volatile uint16_t productid = e2prom_get_word(&bldr_eeprom.productid);
    volatile uint16_t vendorid = e2prom_get_word(&bldr_eeprom.vendorid);
    
    // Nachricht empfangen
    volatile int8_t rc = sboxnet_receive_msg(&pmsg->msgh, sizeof(*pmsg));
    if (rc > 0) {
        // empfangen
        // Quelle 0
        if (pmsg->msgh.srcaddr == 0) {
            // setzte Logon Timer auf Zufallswert um die Logons zu entzerren
            if (g_v.timer_watchdog == 0) {
                g_v.timer_logon = 1 + (bldr_random() & 0x1f);
            }
            g_v.timer_watchdog = WATCHDOG_VAL;
        }
        // nach dem SBOXNET_CMD_DEV_REQ_ADDR wird mit einem SBOXNET_CMD_DEV_SET_ADDR an die Broadcast Adresse geantwortet
        // stimmen PUID, Product ID und Vendor ID überein mit der Nachricht? dann ist das ein SBOXNET_CMD_DEV_SET_ADDR für dieses Gerät.
        if (pmsg->msgh.srcaddr == 0 && pmsg->msgh.dstaddr == SBOXNET_ADDR_BROADCAST && pmsg->msgh.cmd == SBOXNET_CMD_DEV_SET_ADDR) {
           uint8_t x = 0;
           if (pmsg->msgh.opt.len == 9 && *ppuid == g_v.dev_puid && *pprodid == productid && *pvendid == vendorid) {
            uint8_t y = 0;
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
    // falls keine Nachricht Adresse anfordern...
    //g_v.timer_watchdog;
    volatile uint8_t x1 = g_v.timer_logon;
    if (g_v.timer_watchdog && g_v.timer_logon == 0) {
        // Ziel ist Adresse 0
        pmsg->msgh.dstaddr = 0;
        pmsg->msgh.seq = 0;
        pmsg->msgh.opt.len = 8;
        pmsg->msgh.cmd = SBOXNET_CMD_DEV_REQ_ADDR; // Adresse anfordern
        if (sboxnet_can_send_msg(&pmsg->msgh)) {
            g_v.timer_logon = 10 + (bldr_random() & 0x1f);
            *ppuid = g_v.dev_puid; // PUID des Gerätes
            *pprodid = productid; // Product ID
            *pvendid = vendorid;  // Vebdor ID
            sboxnet_send_msg(&pmsg->msgh); // und senden
        }
    }
}

/* Bootloader Task.
 * Führt bestimmte Aufgaben aus:
 * - LEDs je nach Status.
 * - Geräte Reset
 * - anfordern einer Adresse
 */
#ifdef WITH_SECS
BLDR_APP_SECTION
#endif
NOINLINE __ATTR_USED
static uint8_t _bldr_task(struct sboxnet_msg_max* pmsg) {
    // LEDs je nach Status setzen
    bldr_leds_task();
    
    // soll ein Geräte Reset gemacht werden?
    if (bit_is_set(g_dev_state, DEV_STATE_FLG_RESET_b)) {
        // zuerst warten bis alles gesendet ist
        while (!sboxnet_all_sent());
        // Reset durchführen
        ioreg_ccp(&RST.CTRL, Bit(RST_SWRST_bp));
        while(1);
    }
    
    // soll Adresse angefordert werden?
    uint8_t t = g_dev_state;
    uint8_t r = DEV_STATE_FLG_REQ_ADDR_b;
    if (bit_is_set(t, r)) {
        // Logon
        bldr_sboxnet_logon(pmsg);
        // 1 zurückgeben
        return 1;
    }
    return 0;
}

/* Bootloader Hauptroutine.
 */
#ifdef WITH_SECS
BLDR_APP_SECTION
#endif
NOINLINE
static void bldr_main(uint8_t boot) {
    // Buffer für Flashbereich
    struct sboxnet_msg_max msg;
    uint8_t pagebuf[SPM_PAGESIZE];
    // Bootsector Vector Tabelle aktivieren und MED Level Interrupts for sboxnet
    ioreg_ccp(&PMIC.CTRL, Bit(PMIC_IVSEL_bp)|Bit(PMIC_MEDLVLEN_bp));

    // markiere Status als "im bootloader"
    setbit_atomic(g_dev_state, DEV_STATE_FLG_BOOTLOADER_b);
    // soll der Boot Process weiterlaufen
    if (boot) {
        // ja, dann NET Reset verarbeiten:
        // - Geräteadresse auf die Broadcastadresse
        // - setzt State Flag DEV_STATE_FLG_REQ_ADDR_b, erwarte Adresse
        bldr_process_cmd_net_reset();
        
        // Sboxnet Init
        sboxnet_init();
    }
    
    // Standard Sleep Modus IDLE
    set_sleep_mode(SLEEP_SMODE_IDLE_gc);
    // aktiviere Interrupts
    sei();
        
    while(1) {
        // führe bldr_task() aus: - LEDs je nach Status, Geräte Reset, und anfordern einer Adresse; != 0 Address angefordert; == 0 keine Addresse angefordert
        volatile uint8_t r1 = bldr_task(&msg);
        if (!r1 /*!bldr_task(&msg)*/) {
            // ist was empfangen worden?
            if (sboxnet_receive_msg(&msg.msgh, sizeof(msg)) > 0) {
                // Nachricht verarbeiteiten
                if (bldr_process_msg(&msg.msgh, pagebuf) == SBOXNET_ACKRC_FWUP_FINISHED) {
                    // war es ein erfogreiches Firmware Update?
                    // warte bis alles gesendet ist
                    while (!sboxnet_all_sent());
                    // benende dann das Firmware Update
                    sbldr_finish_fwup(bit_is_set(g_dev_state, DEV_STATE_FLG_FWUP_APPLICATION_b));
                
                    // Wird nie erreicht...
                }
            }
        }
        
        
        // und schlafen legen, durch einen Interrupt wird wieder aufgewacht
        // sleep_enable(), sleep_cpu(), sleep_disable()
        sleep_mode();
    }
}

/* Bootloader Start
 */
BOOTLOADER_SECTION NOINLINE
void bldr_start(void) {
    // Watchdog triggern
    wdt_reset();
    // Watchdog einschalten
    ioreg_ccp(&WDT.CTRL, Bsv(WDT_ENABLE_bp,0)|Bsv(WDT_CEN_bp,1));
/*
    // clear errinfo in eeprom
    uint8_t *e = &bldr_eeprom.errinfo[0];
    uint8_t i;
    for (i=0;i<55;i++) {
        sbldr_e2prom_write(e[i], 0);
    }
    bldr_e2prom_write(&bldr_eeprom.errinfo[0],1);
*/
    //Bootloader System Init
    bldr_init_system();
    // Watchdog Timer Init
    g_v.timer_watchdog = 0;

    // Test ob Firmware  Update Flag 0 ist
    // Wenn 0x300 (EEPROM) 0 ist dann zur Applikation springen
    volatile uint8_t rc = e2prom_get_byte(&bldr_eeprom.firmware_update);
    if (rc == 0) { // check Firmware Update flag in EEPROM address 0x300
        // Applikation Sektion Vector Tabelle aktivieren und HI,MED,LO Interrupts
        ioreg_ccp(&PMIC.CTRL, Bit(PMIC_HILVLEN_bp)|Bit(PMIC_MEDLVLEN_bp)|Bit(PMIC_LOLVLEN_bp));
        // springe zum Applikations Code bei Adresse 0
        __asm__ __volatile__ (
            "jmp  0\n"    // springe zum Applikations Programm bei Adresse 0
        );
    }
    // absonsten bootloader Main mit boot.
    bldr_main(1);
    
    while(1); // für immer warten
}

/* Bootloader aktiviern aus der Applikation.
 */
#ifdef WITH_SECS
BLDR_APP_SECTION
#endif
NOINLINE __ATTR_USED
static void _bldr_activate(void) {
    // zuerst Interrupts aus
    cli();
    // dann Firmware  Update Flag aus 255 setzen
    sbldr_e2prom_write(&bldr_eeprom.firmware_update, 0xff);
    // Reboot zum bootloader
    bldr_main(0);
    while(1);
}


/* 2 Bytes reservieren in der Bootloader Sektion für die Firmware Update CRC.
 * Verhindert das der Linker diesen Bereich nutzt.
 * 
 */
BLDR_CRC_SECTION NOINLINE __ATTR_NAKED __ATTR_USED
static void bldr_crc_section(void) {
    __asm__ __volatile__ (
        "bldr_crc:  .word 0xffff\n"
    );
}



// boot loader static section ------
// Macros für SPM (byte), SPM (word), nvm Kommando

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
/* uint8_t xsbldr_prog_page(uint16_t pageaddr, uint8_t *srcbuf, uint8_t fromflash)
 * Flash Seite programmieren
 */
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

/* void xsbldr_e2prom_write(uint8_t* addr, uint8_t v)
 * Ein Byte v in das EEPROM schreiben bei EEPROM Adresse addr
 */
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

/* void xsbldr_finish_fwup(uint8_t isapp)
 * Firmware Update abschliessen und Reboot.
 */
BLDR_STATIC_SECTION __ATTR_NO_RETURN __ATTR_USED NOINLINE
static void xsbldr_finish_fwup(uint8_t isapp) {
    cli();
    
    uint8_t verifyerr = 0;
    
    if (!isapp) {
        uint16_t pageaddr = BLDR_START_ADDR;
        uint16_t srcaddr  = 0;
        while (pageaddr < BLDR_START_ADDR + BLDR_SIZE) {
            verifyerr |= sbldr_prog_page(pageaddr, (uint8_t*)srcaddr, 1);
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

/* Vectoren für:
 * - sbldr_e2prom_write -> xsbldr_e2prom_write
 * - sbldr_finish_fwup -> xsbldr_finish_fwup
 * - sbldr_prog_page -> xsbldr_prog_page
 */
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

// und nicht mehr BOOTLOADER
#undef BOOTLOADER


