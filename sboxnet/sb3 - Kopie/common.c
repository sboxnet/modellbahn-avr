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
 * ****************************/

// allgemeine defs holen
#include "common.h"
// für EEPROM Zugriffe
#include <avr/eeprom.h>

// section .init2: Stack auf (INTERNAL_SRAM_END-sizeof(struct bldr_ram) - 2) setzen
__attribute__ ((section(".init2"))) __ATTR_NAKED
void com_init2(void) {
    SP = BLDR_STACK_TOP;
}

/* Reserve 2 bytes in the application section for the firmware update CRC.
 * Prevents the linker to use this space.
 * THIS IS NOT THE LOCATION OF THE APP CRC ! IT IS AT THE END OF THE APP CODE. THE ONLY USE OF THIS IS TO GENERATE A LINKER ERROR IF THE CODE IS TOO LARGE.
 */
__attribute__ ((section(".appcrc"))) __ATTR_NAKED __ATTR_NO_UNUSED_WARN
static void com_appcrc(void) {
    __asm__ __volatile__ (
        "app_crc:  .word 0xffff\n"
    );

}

// symbols defined by linker.
// if only bootloader is linked, this symbol points to INTERNAL_SRAM_START.
// if application is linked, this symbol points to the end of BSS.
extern uint8_t _end;


#define STACK_MAGIC   0
/* Wieviel ist vom Stack frei?
 * von _end bis ein 0 Pointer auf stack liegt.
 */
static uint16_t com_stack_free(void) {
    uint8_t* p = &_end;
    while (p < (uint8_t*)BLDR_STACK_TOP) {
        if (*p != STACK_MAGIC) {
            break;
        }
        p++;
    }
    return (uint16_t)(p - &_end);
}

/* Größe des Stacks in Bytes.
 */
static uint16_t com_stack_size(void) {
    return (uint8_t*)BLDR_STACK_TOP - &_end;
}

// Ringbuffer einbinden
#include "avrutilslib/ringbuffer.c"

//Timer D1
#define TIMER_PRESCALER   64 // timer prescaler = 64
#define TIMER_FREQ_HZ     1000 // timer freq
#define TIMER_PERIOD        (F_CPU / TIMER_PRESCALER / TIMER_FREQ_HZ)  // 500

/* common ram , init with all zeros
 * Bereich für Variablen
 */
struct com_ram g_com = { 0, };

/* Bitmaske eines Bits machen
 */
uint8_t bitmask(uint8_t bit) {
	static const uint8_t offsmap[8] PROGMEM = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
	return pgm_read_byte(offsmap + bit);
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// alle Funktionen die weak gebunden sind, können mit "normalen" Funktionen überscrhieben werden
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

/* do_init_system: system init
 * weak bound: Geräte System Init
 */
__ATTR_WEAK void do_init_system(void) {
}

/* do_reg_read: Gerät read a register (reg: 16bit register, pdata: pointer to data)
 * weak bound: einen Wert aus einem Register lesen: Register --> common.h
 */
__ATTR_WEAK uint8_t do_reg_read(uint16_t reg, uint16_t* pdata) {
    return SBOXNET_ACKRC_REG_INVALID;
}

/* do_reg_write: write a register (reg: 16 bit register, data: 16 bit data, mask: 16 bit maske)
 * Schreibt (data & mask) in das Register!
 * weak bound: einen Wert in ein Register schreiben: Register --> common.h
 */
__ATTR_WEAK uint8_t do_reg_write(uint16_t reg, uint16_t data, uint16_t mask) {
    return SBOXNET_ACKRC_REG_INVALID;
}

/* do_msg: process a message pmsg: pointer to a struct sboxnet_msg_header
 * Geräte Nachricht verarbeiten, Nachrichten --> sboxnet-struct.h (SBOXNET_CMD _*)
 * weak bound: Verarbeitet (Geräte-)Nachrichten
 */
__ATTR_WEAK uint8_t do_msg(struct sboxnet_msg_header *pmsg) {
    return SBOXNET_ACKRC_CMD_UNKNOWN;
}

/* do_setup: init the device user defined
 * Geräte Init.
 * weak bound: Geräteeigenes Init
 */
__ATTR_WEAK void do_setup(void) {
}

/* do_main: Hauptroutine. soll nur kurz dauern.
 * weak bound: Geräte Hauptroutine
 */
__ATTR_WEAK void do_main(void) {
}

/* do_before_bldr_activate: Statements die vor der Aktivierung des Bootloaders ausgeführt werden sollen
 * weak bound: Statements die vor der Aktivierung des Bootloaders ausgeführt werden sollen
 */
__ATTR_WEAK void do_before_bldr_activate(void) {
}

// Standard Action die beim reset Kommando ausgeführt wird
// Alle devices sollen eine neue addresse anfordern.
static uint8_t com_process_cmd_reset(void) {
    // Atomic: Interrupts sollen nicht unterbrechen!
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        // Geräteadresse ist die Broadcast Adresse
        g_v.dev_addr = SBOXNET_ADDR_BROADCAST;
        // eine neue Adresse soll angefordert werden
        g_dev_state |= Bit(DEV_STATE_FLG_REQ_ADDR_b);
		//setbit(g_dev_state, DEV_STATE_FLG_REQ_ADDR_b);
    }
    // keine Antwort senden
    return SBOXNET_ACKRC_SEND_NO_ANSWER;
}

/* Intern Standard Register lesen
 * uint8_t com_reg_read(uint16_t reg, uint16_t *pdata)
 *    - reg    Register als 16 Bit Wert
 *    - pdata  Zeiger auf 16 Bit Werte
 * 
 * Reaktion auf:
 *  R_DEBUG_STACK_SIZE > stack size
 *  R_DEBUG_STACK_FREE > stack free
 *  R_FIRMWARE_VERSION > firmware version
 *  R_CAPABILITIES > Fähigkeiten des Devices
 *  R_CAP_CLASS > Fähigkeiten Klasse (GBM, Weichenservo, Booster, DccGen, ...) siehe CAP_* Makros
 */
static uint8_t com_reg_read(uint16_t reg, uint16_t *pdata) {
    switch (reg) {
        case R_DEBUG_STACK_SIZE:    *pdata = com_stack_size(); return 0;
        case R_DEBUG_STACK_FREE:    *pdata = com_stack_free(); return 0;
    }
    // bootloader register lesen
    uint8_t rc = bldr_reg_read(reg, pdata);
    if (rc == 0)
        return rc;
    
    switch (reg) {
        case R_FIRMWARE_VERSION:    *pdata = g_com.firmware_version; return 0;
        case R_CAPABILITIES:        *pdata = g_com.capabilities; return 0;
        case R_CAP_CLASS:           *pdata = g_com.cap_class; return 0;
    }
    // Register Lesen vom Gerät
    return do_reg_read(reg, pdata);
}

/* com_reg_write: Intern Standard Register schreiben
 * uint8_t com_reg_write(uint16_t reg, uint16_t data, uint16_t mask)
 *     - reg   Register als 16 Bit Wert
 *     - data  16 Bit Daten
 *     - mask  16 Bit Maske: beschrieben wird (data & mask)
 */
static uint8_t com_reg_write(uint16_t reg, uint16_t data, uint16_t mask) {
    // ruft nur Geräte do_reg_write auf
    return do_reg_write(reg, data, mask);
}

/* Kommando: com_cmd_reg_read: ein oder mehrere (len8) Register lesen
 * gelesen wird ab dem ersten Register, dann fortlaufend
 * uint8_t com_cmd_reg_read(struct sboxnet_msg_header *pmsg)
 *   - pmsg  Zeiger auf die Nachricht
 * 
 * read register: len:3 reg16 len8   --> len:2*len8 len8*databyte16
 */
static uint8_t com_cmd_reg_read(struct sboxnet_msg_header *pmsg) {
    // Zeiger auf Nachricht Daten
    uint16_t *pdata = (uint16_t*)pmsg->data;
    // Register, index 0
    uint16_t regaddr = *pdata;
    // Anzahl der Register
    uint8_t num = pmsg->data[2];
    // ist die Anzahl der Register gültig?
    if (pmsg->opt.len != 3 || num > SBOXNET_MSG_DATA_LEN/2 ) {
        return SBOXNET_ACKRC_INVALID_ARG;
    }
    // Antwort vorbereiten
    pmsg->opt.len = 2*num;
    while (num) {
        uint8_t rc = com_reg_read(regaddr++, pdata++);
        if (rc != 0) {
            return rc;
        }
        num--;
    }
    return SBOXNET_ACKRC_OK;
}


/* Kommando: com_cmd_reg_read_multi: mehrere Register lesen
 * Gelesen werden die angegeben Register.
 * uint8_t com_cmd_reg_read_multi(struct sboxnet_msg_header *pmsg)
 *   - pmsg  Zeiger auf die Nachricht
 * 
 * read register multi: len:n*2 n*reg16   --> len:n*2 n*data16
 */
static uint8_t com_cmd_reg_read_multi(struct sboxnet_msg_header *pmsg) {
    // Zeiger auf Daten
    uint16_t *pdata = (uint16_t*)pmsg->data;
    // ist die Länge der Nachricht OK?
    if (pmsg->opt.len > SBOXNET_MSG_DATA_LEN || (pmsg->opt.len & 0x01)) {
        return SBOXNET_ACKRC_INVALID_ARG;
    }
    // Länge der Nachricht
    uint8_t n = pmsg->opt.len / 2;
    // Pro Register, lesen
    while (n > 0) {
        uint8_t rc = com_reg_read(*pdata, pdata);
        if (rc != 0) {
            return rc;
        }
        pdata++;
        n--;
    }
    return SBOXNET_ACKRC_OK;
}

/* com_process_std_msg: Standard Nachricht verarbeiten
 *  SBOXNET_CMD_NET_RESET    Net Reset: alle Geräte sollen neue Adresse anfordern
 *  SBOXNET_CMD_NET_WATCHDOG keine Aktion
 *  SBOXNET_CMD_DEV_RESET    Device Reset: Gerät soll neue Adresse anfordern
 *  SBOXNET_ADDR_BROADCAST   keine Aktion
 *  SBOXNET_CMD_REG_READ     Register read
 *  SBOXNET_CMD_REG_READ_MULTI mehrere Register lesen
 *  SBOXNET_CMD_REG_WRITE    Register schreiben
 *  SBOXNET_CMD_REG_WRITE_BIT Bit im Register schreiben
 *  SBOXNET_CMD_DEV_SET_DESC Gerätebeschreibung setzen
 *  alle anderen werden weitergeleitet zu bldr_process_basic_msg
 */
static uint8_t com_process_std_msg(struct sboxnet_msg_header *pmsg) {
    // Nachrichten behandeln
    switch (pmsg->cmd) {
        // SBOXNET_CMD_NET_RESET
        case SBOXNET_CMD_NET_RESET: {
            if (pmsg->srcaddr == 0) {
                // neue Adresse anfordern
                return com_process_cmd_reset();
            }
            return SBOXNET_ACKRC_SEND_NO_ANSWER;
        }
        
        // SBOXNET_CMD_NET_WATCHDOG
        case SBOXNET_CMD_NET_WATCHDOG: {
            return SBOXNET_ACKRC_SEND_NO_ANSWER;
        }
        
        // SBOXNET_CMD_DEV_RESET
        case SBOXNET_CMD_DEV_RESET: {
            // Geräte Reset
            setbit_atomic(g_dev_state, DEV_STATE_FLG_RESET_b);
            
            if (pmsg->dstaddr == SBOXNET_ADDR_BROADCAST) {
                return SBOXNET_ACKRC_SEND_NO_ANSWER;
            } else {
                pmsg->opt.len = 0;
                return SBOXNET_ACKRC_OK;
            }
        }
    }
    
    // falls Ziel Broadcat Adresse ist, keine Antwort
    if (pmsg->dstaddr == SBOXNET_ADDR_BROADCAST) {
        return SBOXNET_ACKRC_SEND_NO_ANSWER;
    }
    
    switch (pmsg->cmd) {
        // SBOXNET_CMD_REG_READ
        case SBOXNET_CMD_REG_READ: {
            // register lesen
            return com_cmd_reg_read(pmsg);
        }
        
        // SBOXNET_CMD_REG_READ_MULTI
        case SBOXNET_CMD_REG_READ_MULTI: {
            // mehre Register lesen
            return com_cmd_reg_read_multi(pmsg);
        }
        
        // SBOXNET_CMD_REG_WRITE
        case SBOXNET_CMD_REG_WRITE: {
            // Register schreiben

            // Zeiger auf Daten
            uint16_t *pdata = (uint16_t*)pmsg->data;
            // Register
            uint16_t regaddr = pdata[0];
            // und die Daten / Wert
            uint16_t regval  = pdata[1];
            // maske
            uint16_t mask    = (pmsg->opt.len == 6 ? pdata[2] : 0xffff);
            if (pmsg->opt.len != 4 && pmsg->opt.len != 6) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            pmsg->opt.len = 0;
            // und register schreiben
            return com_reg_write(regaddr, regval, mask);
        }
        
        // SBOXNET_CMD_REG_WRITE_BIT
        case SBOXNET_CMD_REG_WRITE_BIT: {
            // Bit in einem Register schreiben
            
            // Zeiger auf Daten
            uint16_t* pdata = (uint16_t*)pmsg->data;
            // Register
            uint16_t regaddr = pdata[0];
            // Bit
            uint8_t val = pmsg->data[2];// value (0x80) und bit (0x0f) nummer
            if (pmsg->opt.len != 3) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            uint16_t data = 0;
            uint16_t mask = 0x01;
            mask <<= (val & 0x0f);
            // ist Bit 7 im Wert gesetzt -> data = mask entspricht bitval = 1
            if (val & 0x80) {
                data = mask;
            }
            pmsg->opt.len = 0;
            // Bit schreiben
            return com_reg_write(regaddr, data, mask);
        }
        
        // SBOXNET_CMD_DEV_SET_DESC
        case SBOXNET_CMD_DEV_SET_DESC: {
            // Gerätebeshcribung setzen
            
            // Länge der Beschreibung
            uint8_t n = pmsg->opt.len;
            // Länge Muss sein [1..(DEV_DESC_MAX_SIZE]
            if (n == 0 || n > DEV_DESC_MAX_SIZE) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            // ID der Beschreibung
            uint8_t id = pmsg->data[0];
            //ist ID gültig?
            if (id == 0 || id >= DEV_NUM_DESCS) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            n--;
            if (n < DEV_DESC_MAX_SIZE) {
                pmsg->data[1+n] = 0;
                n++;
            }
            // schreiben ins EEPROM
            eeprom_update_block(&pmsg->data[1], bldr_eeprom.dev_desc[id], n);
            pmsg->opt.len = 0;
            return SBOXNET_ACKRC_OK;
        }
        default:
            // sonst bldr_process_basic_msg
            return bldr_process_basic_msg(pmsg);        
    }
    return SBOXNET_ACKRC_CMD_UNKNOWN;
}

/* com_process_msg: Standard-Nachricht verarbeiten
 * void com_process_msg(struct sboxnet_msg_header *pmsg)
 *   - pmsg  Zeiger auf Nachricht
 * 
 */
static void com_process_msg(struct sboxnet_msg_header *pmsg) {
    char rc = 0;
    // ist die Quelladresse 0, dann watch dog timer setzen
    
    // ist Quelladresse 0 -> Watchdog Ping
    if (pmsg->srcaddr == 0) {
        g_v.timer_watchdog = WATCHDOG_VAL;
    }
    
    // Usereinsprung
    rc = do_msg(pmsg);
     if (rc == SBOXNET_ACKRC_OK) {
        goto send_ok;
    } else if (rc == SBOXNET_ACKRC_SEND_NO_ANSWER) {
        return;
    } else if (rc != SBOXNET_ACKRC_CMD_UNKNOWN) {
        goto send_error;
    }
    // ist Bit 7 des Kommandos gestetzt, dann keine Antwort
    if (pmsg->cmd & 0x80) {
        return;  // discard Ack Answers
    }
    
    // Standard Nachricht
    rc = com_process_std_msg(pmsg);
    if (rc == SBOXNET_ACKRC_SEND_NO_ANSWER) {
        return;
    }
    if (rc == SBOXNET_ACKRC_OK) {
		goto send_ok;
    }
send_error:
    pmsg->opt.len = 2;
    pmsg->data[0] = pmsg->cmd; // ursprüngliches Kommando
    pmsg->data[1] = rc; // Rückgabewert
    pmsg->cmd = 0x80; // mark Kommando als Antwort
send_ok:
    // Antwort senden an Absender
    pmsg->dstaddr = pmsg->srcaddr;
    pmsg->cmd |= 0x80; // als Antwort auf Kommando markieren
    sboxnet_send_msg(pmsg);
}

/* com_receive_and_process_msg: Nachricht emfangen und verarbeiten
 * void com_receive_and_process_msg(struct sboxnet_msg_max* pmsg)
 *   - pmsg  Nachrichtenbuffer
 */
static void com_receive_and_process_msg(struct sboxnet_msg_max* pmsg) {    
    int8_t rc = sboxnet_receive_msg(&pmsg->msgh, sizeof(*pmsg));
    if (rc > 0) {
        // Kommando war erfolgreich
        com_process_msg(&pmsg->msgh);
    
        // ansonsten war das Ergebnis ein CRC Fehler?
    } else if (rc == SBOXNET_ERR_RECVMSG_CRC) {
        // wenn ja, das Errorflag setzen CRC Fehler
        setbit(g_dev_errflags, DEV_ERR_FLG_SBOXNETCRC_b); // crc error
    }
}

/* Standard Geräte Initialiserung
 */
static void com_init_system(void) {
    // zuerst die User Geräte Initialiserung
    do_init_system();
    
    // standard timer
    // timer D1
    TCD1.CTRLB = TC_WGMODE_NORMAL_gc; // Normal Mode
    TCD1.CTRLD = 0; // Event Action off
    TCD1.CTRLE = 0; // normal mode
    TCD1.INTCTRLA = 0;  // err and ov disabled
    TCD1.INTCTRLB = TC_CCBINTLVL_LO_gc; // CCB interupt level auf LOW
    TCD1.INTFLAGS = 0xff; // clear interupt TCD0 flags
    TCD1.PER = 0xffff; // D1 Periode
    TCD1.CCB = TCD1.CNT + TIMER_PERIOD; // 1000 Hz
    TCD1.CTRLA = TC_CLKSEL_DIV64_gc; // TCD0 clock divider /64

    // schreibe Product und Vendor ID in das EEPROM
    eeprom_update_word(&bldr_eeprom.productid, g_com.productid);
    eeprom_update_word(&bldr_eeprom.vendorid, g_com.vendorid);
    
    // schreibe Gerätebeschreibung in das EEPROM
    if (g_com.dev_desc_P) {
        PGM_P p = g_com.dev_desc_P;
        for (uint8_t n = 0; n < DEV_DESC_MAX_SIZE; n++) {
            uint8_t c = pgm_read_byte(p++);
            eeprom_update_byte(&bldr_eeprom.dev_desc[0][n], c);
            if (c == 0) {
                break;
            }
        }
    }
    uint8_t x = 0;
}

/* Timer D1 CCB Interrupt vector
 * every 1ms
 */
ISR(TCD1_CCB_vect) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        // CCB neu setzen. addieren TIMER_PERIOD
        TCD1.CCBBUF = TCD1.CNT + TIMER_PERIOD;
    }
    
    // common timer erhöhen
    g_com.timer++;
    
    struct timer *t;
    // verkettete Timer 
    // Ist der common timer & 0xf == 0, dann auf 16ms timmer setzen
    if ((g_com.timer & 0x0f) == 0) {
        t = g_com.timer_16ms;
        while (t) {
            if (t->value > 0) {
                t->value--;
            }
            t = t->next;
        }
    }
    // 1ms timer
    t = g_com.timer_1ms;
    while (t) {
        if (t->value > 0) {
            t->value--;
        }
        t = t->next;
    }
}

/* main: Geräte Hauptroutine
 * int main(void)
 * 
 */
__ATTR_OS_MAIN int main(void) {
    // erst mal interupts aus
    cli();
    // System Initiatliserung
    com_init_system();
    
    // Kommando reset verarbeiten
    com_process_cmd_reset();
    // Kommunikation Sboxnet Initialiseren
    sboxnet_init();
   
    // Standard sleep Mode IDLE
    set_sleep_mode(SLEEP_SMODE_IDLE_gc);
    // Interrupts ein
    sei();
    
    // Gerät Setup
    do_setup();

// Clk output at PC7
/*
PORTC_DIRSET=PIN7_bm;
PORTCFG_CLKEVOUT=PORTCFG_CLKOUT_PD7_gc;
*/
    // Hauptschleife
    while(1) {
        // Buffer für Nachrichten
        struct sboxnet_msg_max msg;
        
        // wird keine Adresse angefordert? 0==keine Addresse angefordert: !=0 Adresse angefordert
        volatile uint8_t r2 = bldr_task(&msg);
        if (/*bldr_task(&msg)*/ r2 == 0) {
            // dann verarbeiten
            com_receive_and_process_msg(&msg);
            // ist das Gerät im FW Update Modus?
            /*if (g_dev_state & DEV_STATE_FLG_FWUP_MASK) {
                
                while (!sboxnet_all_sent()); // wait until sboxnet transmitter is empty
                
                // Interupts sperren
                cli();
                // Aktion vor Bootloaderaktiverung
                do_before_bldr_activate();
                
                // alle Timer Interrups stoppen
                // C0 und C1
                TCC0.INTCTRLA = 0;
                TCC0.INTCTRLB = 0;
                TCC0.CTRLA = 0; // stop timer
                TCC0.INTFLAGS = 0xff; // clear interrupt flags
                TCC1.INTCTRLA = 0;
                TCC1.INTCTRLB = 0;
                TCC1.CTRLA = 0; // stop timer
                TCC1.INTFLAGS = 0xff; // clear interrupt flags
                // D0 und D1
                TCD0.INTCTRLA = 0;
                TCD0.INTCTRLB = 0;
                TCD0.CTRLA = 0; // stop timer
                TCD0.INTFLAGS = 0xff; // clear interrupt flags
                TCD1.INTCTRLA = 0;
                TCD1.INTCTRLB = 0;
                TCD1.CTRLA = 0; // stop timer
                TCD1.INTFLAGS = 0xff; // clear interrupt flags
                
                // start firmware update
                bldr_activate();
                // never reached
            } else */ {
                // ansonsten User Main ausführen
                do_main();
            }
        } else {
            // keine Adresse angefordert
            // schlafen legen
            sleep_mode();
        }
    }
    
    return 0;
}
