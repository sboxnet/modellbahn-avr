/***************************************************************************
 *   Copyright (C) 2014-2020
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

#include "common.h"

/*
 * - DCC Cutout is generated only at the end of the packet
 * - At the end of a cutout, shortcut detection may be triggered once or two times.
 * - Current/shortcut detection: MAX_SHORTCUT_CNT shortcuts must have occured in TIMER_SHORT_CUT time
 * - Shortcut detection is disabled in the TIMER_STARTUP time after DCC startup
 * - Shortcut detection is disabled in the first half of the first DCC bit after a cutout
 */

/*
 * Port Mapping
 * AWEXC:
 * ------ (ATXMEGA32A4U Datasheet P 59)
 * PC0 10 OC0ALS
 * PC1 11 OC0AHS
 * PC2 12 OC0BLS
 * PC3 13 OC0BHS
 */

/*
 * TCC0
 * - timer not running
 * - CCA: AWEXC for booster IN1/IN2. PC0, PC1.
 * - CCB: AWEXC for booster EN. PC2 and PC3 wired or.
 * 
 * TCC1
 * - timer 500 kHz / 2us, FRQ Mode, 8bit
 * - DCC generator.
 * - CCA: filled by DMA transfer
 * 
 * TCD0
 * - general timer 500 kHz / 2us, normal mode, 16bit
 * - CCA: timer
 * - CCB: booster shortcut detector
 * - CCC: DCC decoder, cutout timer
 * 
 * TCD1
 * - timer 500 kHz / 2us, normal mode, 8bit
 * - CCA: DCC decoder. input capture, event 0, PC4
 */

#define USE_DCCGEN 1

#define VENDOR_ID    0x9999
#define FIRMWARE_VERSION 0x0200
#define PRODUCT_ID   0x0004
#define DEVICE_DESC  "DCCgen:2"

APP_FIRMWARE_HEADER(PRODUCT_ID, VENDOR_ID, FIRMWARE_VERSION)

#define LED_PORT  PORTD

#define NOTAUS_PORT PORTC
#define NOTAUS_b    5

// timer 10ms
static struct timer g_timer_10ms;

// Booster Werte im EEPROM
// Shortcut Limit
// Shortcut Interval
/*struct booster_eeprom {
    uint16_t   shortcut_limit;
    uint16_t   shortcut_interval;
};*/

// DCCGEN Werte im EEPROM
// locoaddr_scan_max Maximalwert für Locoaddr
struct dccgen_eeprom {
    uint16_t   locoaddr_scan_max;
};

struct Eeprom {
    //struct booster_eeprom booster;
    //uint8_t reserved[128-sizeof(struct booster_eeprom)];
    struct dccgen_eeprom  dccgen;
};
struct Eeprom g_eeprom EEMEM;

// LED Ein
#define LED_DCC_ON_b      6
// LED Notaus
#define LED_DCC_NOTAUS_b  7

// Flag Ein
#define DCCGEN_FLAG_ON_b      0
// Flag NOTAUS
#define DCCGEN_FLAG_NOTAUS_b  1

#define SWITCH_NOTAUS_b  0

#define DCCGEN_DEFAULT_LOCOADDR_SCAN_MAX  100

struct dccgen {
    uint8_t   flags;
    uint8_t   switches;
    uint8_t   switches_t;
    uint16_t  locoaddr_scan_cur;
    uint16_t  locoaddr_scan_max;
    struct {
        unsigned write_locoaddr_scan_max:1;
    }              eeprom_flags;
};
struct dccgen g_dccgen = { 0, };

#define DCC_PORT        PORTC
#define DCC_PORT_BIT    4


#define DCC_1  ((58/2)-1)
#define DCC_0  ((116/2)-1)

#define NUM_DCC_SLOTS 2
#define DCC_PACKET_COMPILED_BUFSIZE (2*(6*9+1+20))

struct dcc_packet_compiled {
    uint8_t size;
    uint8_t buf[DCC_PACKET_COMPILED_BUFSIZE];
};

struct dcc_packet {
    uint8_t size;
    uint8_t data[6];
};

struct dcc_slot {
    uint8_t fill;
    struct dcc_packet_compiled pkg;
};

#define DCC_STATE_NONE  0
#define DCC_STATE_INIT_RESET  1
#define DCC_STATE_INIT_IDLE   2
#define DCC_STATE_ON 3

struct dcc_control {
    uint8_t state;
    uint8_t packet_counter;
    uint8_t free_slot;
    struct dcc_slot slots[NUM_DCC_SLOTS];
};

struct dcc_control g_dcc;

struct dcc_packet g_dcc_packet_reset = {
    .size = 2,
    .data = { 0, 0 },
};
struct dcc_packet g_dcc_packet_idle = {
    .size = 2,
    .data = { 0xff, 0 },
};


void dcc_init_dma(void) {
    DMA.CTRL = DMA_ENABLE_bm|DMA_DBUFMODE_CH01_gc|DMA_PRIMODE_RR0123_gc;
    DMA.INTFLAGS = 0xff;
    
    struct dcc_slot* slot = &g_dcc.slots[0];
    DMA_CH_t* ch = ((DMA_CH_t*)(&DMA.CH0));
    for (uint8_t i = 0; i < NUM_DCC_SLOTS; i++, slot++, ch++) {
        ch->CTRLA = 0;
        ch->CTRLA = DMA_CH_RESET_bm;
        
        while (ch->CTRLA & DMA_CH_RESET_bm);
        
        ch->REPCNT = 0; // endless
        ch->CTRLA = Bsv(DMA_CH_ENABLE_bp, 0)|Bsv(DMA_CH_REPEAT_bp, 1)|Bsv(DMA_CH_SINGLE_bp, 1)|DMA_CH_BURSTLEN_1BYTE_gc; // single shot: transfer only 1 byte at transfer trigger
        ch->CTRLB = DMA_CH_ERRIF_bm|DMA_CH_TRNIF_bm|DMA_CH_ERRINTLVL_OFF_gc|DMA_CH_TRNINTLVL_LO_gc;
        ch->ADDRCTRL = DMA_CH_SRCRELOAD_BLOCK_gc|DMA_CH_SRCDIR_INC_gc|DMA_CH_DESTRELOAD_NONE_gc|DMA_CH_DESTDIR_FIXED_gc;
        ch->TRIGSRC = DMA_CH_TRIGSRC_TCC1_CCA_gc;
        ch->TRFCNT = 1;
        
        uint32_t srcaddr = (uintptr_t)slot->pkg.buf;
        ch->SRCADDR0 = (uint8_t)srcaddr;
        ch->SRCADDR1 = (uint8_t)(srcaddr >> 8);
        ch->SRCADDR2 = (uint8_t)(srcaddr >> 16);
        
        uint32_t dstaddr = (uintptr_t)&TCC1.CCAL;
        ch->DESTADDR0 = (uint8_t)dstaddr;
        ch->DESTADDR1 = (uint8_t)(dstaddr >> 8);
        ch->DESTADDR2 = (uint8_t)(dstaddr >> 16);
    }
}

void dcc_init(struct dcc_control* pcntr) {
    pcntr->state = DCC_STATE_NONE;
    pcntr->packet_counter = 0;
    pcntr->free_slot = 0;
    
    // DCC timer
    TCC1.CTRLB = Bsv(TC1_CCAEN_bp,1)|TC_WGMODE_FRQ_gc;
    
    TCC1.CTRLA = TC_CLKSEL_OFF_gc;
    TCC1.CTRLC = Bit(TC1_CMPA_bp); // set compare output CMPA to 1 (Recessive)
    TCC1.CTRLD = 0;
    TCC1.CTRLE = TC1_BYTEM_bm;
    TCC1.INTCTRLA = 0;
    TCC1.INTCTRLB = 0;
    TCC1.INTFLAGS = 0xff;
    TCC1.PER = 0;
    TCC1.CNT = 0;
    TCC1.CCA = DCC_1;
    
    port_dirout(DCC_PORT, Bit(DCC_PORT_BIT));
}

void dcc_compile_packet(struct dcc_slot* pdst, struct dcc_packet* psrc) {
#define dcc_put_bit(_pdst, _bit) ({ \
    uint8_t t = (_bit) ? DCC_1 : DCC_0; \
    *(_pdst)++ = t; \
    *(_pdst)++ = t; \
})
    uint8_t* d = pdst->pkg.buf;
    uint8_t size = psrc->size;
    // preamble
    for (uint8_t i = 16; i > 0; i--) {
        dcc_put_bit(d, 1);
    }
    uint8_t xor = 0;
    for (uint8_t i = 0; i < size; i++) {
        dcc_put_bit(d, 0);
        uint8_t b = psrc->data[i];
        xor ^= b;
        for (uint8_t k = 0x80; k != 0; k >>= 1) {
            dcc_put_bit(d, (k & b));
        }
    }
    dcc_put_bit(d, 0);
    for (uint8_t k = 0x80; k != 0; k >>= 1) {
        dcc_put_bit(d, (k & xor));
    }
    dcc_put_bit(d, 1); // Packet End Bit
    pdst->pkg.size = d - pdst->pkg.buf;
    pdst->fill = 0;
}

void dcc_start_generator(struct dcc_control* pcntr) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        TCC1.CTRLA = TC_CLKSEL_OFF_gc;
        TCC1.CTRLC = Bit(TC1_CMPA_bp); // set compare output CMPA to 1 (Recessive)
        pcntr->state = DCC_STATE_INIT_RESET;
        pcntr->packet_counter = 0;
        pcntr->free_slot = 0;
        dcc_compile_packet(&pcntr->slots[0], &g_dcc_packet_reset); 
        dcc_compile_packet(&pcntr->slots[1], &g_dcc_packet_reset);
        
        dcc_init_dma();

        DMA.CH0.TRFCNT = pcntr->slots[0].pkg.size;
        DMA.CH1.TRFCNT = pcntr->slots[1].pkg.size;

        setbit(DMA.CH0.CTRLA, DMA_CH_ENABLE_bp); // start with channel 0

        TCC1.CNT = 0;
        TCC1.CCA = DCC_1;
        TCC1.CTRLA = TC_CLKSEL_DIV64_gc; // start timer, first dma transfer is started on first compare match.
    }
}

void dcc_stop_generator(struct dcc_control* pcntr) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        pcntr->state = DCC_STATE_NONE;
        
        DMA.CH0.CTRLB = DMA_CH_ERRIF_bm|DMA_CH_TRNIF_bm|DMA_CH_ERRINTLVL_OFF_gc|DMA_CH_TRNINTLVL_OFF_gc; // clear interrupt flags and disable transfer end interrupt
        DMA.CH1.CTRLB = DMA_CH_ERRIF_bm|DMA_CH_TRNIF_bm|DMA_CH_ERRINTLVL_OFF_gc|DMA_CH_TRNINTLVL_OFF_gc; // clear interrupt flags and disable transfer end interrupt
        DMA.CH0.CTRLA = 0;
        DMA.CH1.CTRLA = 0;

        TCC1.CTRLC = Bit(TC1_CMPA_bp); // set compare output CMPA to 1 (Recessive)
        TCC1.CTRLA = TC_CLKSEL_OFF_gc; // stop timer
    }
}

ISR(DMA_CH0_vect) {
    DMA.INTFLAGS = DMA_CH0ERRIF_bm|DMA_CH0TRNIF_bm;
    g_dcc.slots[0].fill = 1;
    g_dcc.free_slot = 0;
    g_dcc.packet_counter++;
}

ISR(DMA_CH1_vect) {
    DMA.INTFLAGS = DMA_CH1ERRIF_bm|DMA_CH1TRNIF_bm;
    g_dcc.slots[1].fill = 1;
    g_dcc.free_slot = 1;
    g_dcc.packet_counter++;
}


#define LOCO_FLG_SPEED_14   0
#define LOCO_FLG_SPEED_28   1
#define LOCO_FLG_SPEED_128  2

#define LOCO_SPEED_DIR_FW   0x80
#define LOCO_SPEED_DIR_REV  0x00

#define LOCO_DCC_TIMER 2  // 20ms

#define LOCO_SPTVAL  20

struct loco {
    struct loco* next;
    uint16_t     addr;
    struct locoflags {
        unsigned  spd:2;
        unsigned  fn0:1;
        unsigned  fn1:1;
        unsigned  fn2:1;
        unsigned  pom:2;
    }            flags;
    uint8_t      speed;
    int8_t       sptimer;
    int8_t       sptval;
    uint8_t      fnkey[2];
    struct pom {
     uint16_t cv;
     uint8_t  data;
    }            pom;
};

#define MAX_LOCOS  100

struct loco_list {
    struct loco* pfree;
    struct loco* pused;
    struct loco* pcur;
    uint8_t      numlocos;
    struct loco  loco[MAX_LOCOS];
    volatile uint8_t timer;
};

struct loco_list g_locos;

#define LOCO_COMPILE_SPEED  0
#define LOCO_COMPILE_FN0_4  1
#define LOCO_COMPILE_FN5_8  2
#define LOCO_COMPILE_FN9_12 3
#define LOCO_COMPILE_FN_IDX(_i) (LOCO_COMPILE_FN0_4 + (_i))
#define LOCO_COMPILE_POM    4

#define PUT_COMPILE_BUF(_x)  ({ pkg->data[pkg->size++] = (_x); })

void loco_compile_addr(uint16_t addr, struct dcc_packet* pkg) {
    pkg->size = 0;
    if (addr < 100) { // short address
        PUT_COMPILE_BUF((uint8_t)addr);
    } else {
        PUT_COMPILE_BUF((uint8_t)((addr >> 8) | 0xc0));
        PUT_COMPILE_BUF((uint8_t)addr);
    }    
}

void loco_compile(struct loco* ploco, uint8_t what, struct dcc_slot* pslot) {
    struct dcc_packet pkgbuf;
    struct dcc_packet* pkg = &pkgbuf;
    
    loco_compile_addr(ploco->addr, pkg);

    if (what == LOCO_COMPILE_SPEED) {
        uint8_t cmd = 0x40;
        if (ploco->speed & LOCO_SPEED_DIR_FW) {
            cmd |= 0x20;
        }
        uint8_t speed = ploco->speed & 0x7f;
        if (ploco->flags.spd == LOCO_FLG_SPEED_14) {
            if (speed >= 2) {
                speed = ((speed - 2) * 14 / 125) + 2;
                if (speed > 15) {
                    speed = 15;
                }
            }
            cmd |= speed;
            if (ploco->fnkey[0] & 0x01) {
                cmd |= 0x10;
            }            
            PUT_COMPILE_BUF(cmd);
            
        } else if (ploco->flags.spd == LOCO_FLG_SPEED_28) {
            if (speed == 0) {
                speed = 0;
            } else if (speed == 1) {
                speed = 2;
            } else {
                speed = ((speed - 2) * 28 / 125) + 4;
                if (speed > 31) {
                    speed = 31;
                }
            }
            cmd |= (speed >> 1);
            if (speed & 0x01) {
                cmd |= 0x10;
            }
            PUT_COMPILE_BUF(cmd);
            
        } else {
            PUT_COMPILE_BUF(0x3f); // advanced instruction: 128 speed step control
            PUT_COMPILE_BUF(ploco->speed);
        }
        
    } else if (what == LOCO_COMPILE_FN0_4) {
        uint8_t cmd = 0x80;
        cmd |= (ploco->fnkey[0] >> 1) & 0x0f;
        if (ploco->fnkey[0] & 0x01) {
            cmd |= 0x10;
        }
        PUT_COMPILE_BUF(cmd);
        
    } else if (what == LOCO_COMPILE_FN5_8) {
        uint8_t cmd = 0xb0;
        cmd |= (ploco->fnkey[0] >> 5) & 0x07;
        if (ploco->fnkey[1] & 0x01) {
            cmd |= 0x08;
        }
        PUT_COMPILE_BUF(cmd);
        
    } else if (what == LOCO_COMPILE_FN9_12) {
        uint8_t cmd = 0xa0;
        cmd |= (ploco->fnkey[1] >> 1) & 0x0f;
        PUT_COMPILE_BUF(cmd);        
    
    } else if (what == LOCO_COMPILE_POM) {
         uint8_t cmd = 0xec | ((ploco->pom.cv >> 8) & 0x03);
         PUT_COMPILE_BUF(cmd);
         PUT_COMPILE_BUF(ploco->pom.cv & 0x0ff);
         PUT_COMPILE_BUF(ploco->pom.data);
    }
    
    dcc_compile_packet(pslot, pkg);
}

struct loco* loco_find(uint16_t addr) {
    struct loco* p = g_locos.pused;
    while (p) {
        if (p->addr == addr) {
            return p;
        }
        p = p->next;
    }
    return 0;
}

void loco_scan(struct dcc_slot* freeslot) {
    g_dccgen.locoaddr_scan_cur++;
    if (g_dccgen.locoaddr_scan_cur > g_dccgen.locoaddr_scan_max) {
        g_dccgen.locoaddr_scan_cur = 1;
    }
    if (!loco_find(g_dccgen.locoaddr_scan_cur)) {
        struct dcc_packet pkgbuf;
        struct dcc_packet* pkg = &pkgbuf;
        loco_compile_addr(g_dccgen.locoaddr_scan_cur, pkg);
        PUT_COMPILE_BUF(0xa0); // function group two instruction  FN 9-12 packet
        dcc_compile_packet(freeslot, pkg);
    } else {
        dcc_compile_packet(freeslot, &g_dcc_packet_idle);
    }
}

void loco_task(void) {
    struct dcc_slot* freeslot = &g_dcc.slots[g_dcc.free_slot];
    
    if (!freeslot->fill) {
        return;
    }
    switch (g_dcc.state) {
        case DCC_STATE_NONE:
            return;
            
        case DCC_STATE_INIT_RESET: {
            dcc_compile_packet(freeslot, &g_dcc_packet_reset);
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                if (g_dcc.packet_counter > 100) {
                    g_dcc.packet_counter = 0;
                    g_dcc.state = DCC_STATE_INIT_IDLE;
                }
            }
            break;
        }
        case DCC_STATE_INIT_IDLE: {
            dcc_compile_packet(freeslot, &g_dcc_packet_idle);
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                if (g_dcc.packet_counter > 30) {
                    g_dcc.packet_counter = 0;
                    g_dcc.state = DCC_STATE_ON;
                }
            }
            break;
        }
        case DCC_STATE_ON: {
            // find next loco
            if (g_locos.pcur == 0) {
                if (g_locos.timer == 0) {
                    g_locos.pcur = g_locos.pused;
                    g_locos.timer = LOCO_DCC_TIMER;
                }
                loco_scan(freeslot);
                break;
            }
            struct loco* ploco = g_locos.pcur;
            struct loco* pfound = ploco;
            do {
                if (ploco->sptimer > 0) {
                    ploco->sptimer--;
                }
                if (ploco->sptimer < pfound->sptimer) {
                    pfound = ploco;
                }
                ploco = ploco->next;
                if (ploco == 0) {
                    ploco = g_locos.pused;
                }
            } while (ploco != g_locos.pcur);

            uint8_t compile = LOCO_COMPILE_SPEED;
            if (pfound->flags.pom) {
                pfound->flags.pom--;
                compile = LOCO_COMPILE_POM;
            } else if (pfound->flags.fn0) {
                pfound->flags.fn0 = 0;
                compile = LOCO_COMPILE_FN_IDX(0);
            } else if (pfound->flags.fn1) {
                pfound->flags.fn1 = 0;
                compile = LOCO_COMPILE_FN_IDX(1);
            } else if (pfound->flags.fn2) {
                pfound->flags.fn2 = 0;
                compile = LOCO_COMPILE_FN_IDX(2);                    
            } else {
                if (pfound->sptval < LOCO_SPTVAL) {
                    pfound->sptval++;
                }
                if (pfound->sptval > (LOCO_SPTVAL/2)) {
                    pfound->flags.fn0 = 1;
                    pfound->flags.fn1 = 1;
                    pfound->flags.fn2 = 1;
                }
                pfound->sptimer = pfound->sptval;
                
                g_locos.pcur = g_locos.pcur->next;
            }
            loco_compile(pfound, compile, freeslot);
            break;
        }
    }
    if (g_dcc.free_slot == 0) {
        DMA.CH0.TRFCNT = g_dcc.slots[0].pkg.size;
    } else {
        DMA.CH1.TRFCNT = g_dcc.slots[1].pkg.size;
    }
}

void loco_list_init(void) {
    memset(&g_locos, 0, sizeof(g_locos));
    
    struct loco_list* p = &g_locos;
    p->pfree = p->loco;
    for (uint8_t i = 0; i < MAX_LOCOS-1; i++) {
        p->loco[i].next = p->loco + i + 1;
    }
}

void loco_list_reset(void) {
    g_locos.pcur = g_locos.pused;
    g_locos.timer = 0;
}

uint8_t loco_is_addr_valid(uint16_t addr) {
    return (addr > 0) && (addr < 10000);
}

uint8_t loco_add(uint16_t addr, uint8_t flags, struct loco** pploco) {
    if (!loco_is_addr_valid(addr)) {
        return SBOXNET_ACKRC_LOCO_INVADDR;
    }
    // is a slot available?
    if (g_locos.pfree == 0) {
        return SBOXNET_ACKRC_LOCO_NOSLOT;
    }
    
    // is address free?
    struct loco* p = g_locos.pused;
    while (p) {
        if (p->addr == addr) {
            return SBOXNET_ACKRC_LOCO_ADDRINUSE;
        }
        p = p->next;
    }
    
    // create new loco
    struct loco* ploco = g_locos.pfree;
    g_locos.pfree = ploco->next;
 
    memset(ploco, 0, sizeof(struct loco));
    ploco->addr = addr;
    uint8_t spd = flags & 0x03;
    if (spd > 2) {
        spd = 2;
    }
    ploco->flags.spd = spd;
    ploco->sptimer = LOCO_SPTVAL;
    ploco->sptval = LOCO_SPTVAL;
 
    ploco->next = g_locos.pused;
    g_locos.pused = ploco;
    g_locos.numlocos++;
    
    if (pploco) {
        *pploco = ploco;
    }
    
    return 0;
}

uint8_t loco_del(uint16_t addr) {
    struct loco* p = g_locos.pused;
    struct loco* pold = 0;
    while (p) {
        if (p->addr == addr) {
            if (g_locos.pcur == p) {
                g_locos.pcur = p->next;
            }
            if (pold) {
                pold->next = p->next;
            } else {
                g_locos.pused = p->next;
            }
            g_locos.numlocos--;
            // add to free list
            p->next = g_locos.pfree;
            g_locos.pfree = p;
            return 0;
        }
        pold = p;
        p = p->next;
    }
    return SBOXNET_ACKRC_LOCO_NOTFOUND;
}

uint8_t loco_set_func(uint16_t addr, uint8_t fn0, uint8_t fn1) {
    struct loco* p = loco_find(addr);
    if (!p) {
        return SBOXNET_ACKRC_LOCO_NOTFOUND;
    }
    uint8_t xor0 = fn0 ^ p->fnkey[0];
    uint8_t xor1 = fn1 ^ p->fnkey[1];
    if (xor0 || (xor1 & 0x1f)) {
        if (xor0 & 0x1f) {
            p->flags.fn0 = 1;
        }
        if ((xor0 & 0xe0) || (xor1 & 0x1)) {
            p->flags.fn1 = 1;
        }
        if (xor1 & 0x1e) {
            p->flags.fn2 = 1;
        }
        p->fnkey[0] = fn0;
        p->fnkey[1] = fn1;
        if (p->sptimer >= -2) {
            p->sptimer = -1;
        }
    }
    return 0;
}

uint8_t loco_set_speed(uint16_t addr, uint8_t speed) {
    struct loco* p = loco_find(addr);
    if (!p) {
        return SBOXNET_ACKRC_LOCO_NOTFOUND;
    }
    if (p->speed != speed) {
        p->speed = speed;
        p->sptimer = -2;
        p->sptval = 0;
    }
    return 0;
}

void loco_timing(void) {
    if (g_locos.timer) {
        g_locos.timer--;
    }
}

uint8_t loco_pom(uint16_t addr, uint16_t cv, uint8_t data) {
    if (!loco_is_addr_valid(addr)) {
        return SBOXNET_ACKRC_LOCO_INVADDR;
    }
    struct loco* p = loco_find(addr);
    if (!p) {
        return SBOXNET_ACKRC_LOCO_NOTFOUND;
    }
    
    uint8_t mode = (cv >> 10) & 0x03;
    if (mode != 3) { // only write
        return SBOXNET_ACKRC_INVALID_ARG;
    }
    cv = cv & 0x3f;
    // do not allow to change the address !
    if (cv == 0 || cv == 17-1 || cv == 18-1) {
        return SBOXNET_ACKRC_INVALID_ARG;
    }

    if (p->flags.pom) {
        return SBOXNET_ACKRC_LOCO_BUSY;
    }
    
    p->flags.pom = 2;
    p->pom.cv = cv;
    p->pom.data = data;
    
    p->sptimer = -1;
    
    return 0;
}

void dccgen_init(void) {
    g_dccgen.flags = 0;
    g_dccgen.locoaddr_scan_cur = 1;
    g_dccgen.locoaddr_scan_max = e2prom_get_word(&g_eeprom.dccgen.locoaddr_scan_max);
    if (g_dccgen.locoaddr_scan_max == 0xffff) {
        g_dccgen.locoaddr_scan_max = DCCGEN_DEFAULT_LOCOADDR_SCAN_MAX;
    }
    g_dccgen.eeprom_flags.write_locoaddr_scan_max = 0;

    dcc_init(&g_dcc);
    loco_list_init();
}

static void dcc_read_switches(void) {
    uint8_t sw;

    sw = 0;
    if (bit_is_set(port_in(NOTAUS_PORT), NOTAUS_b)) {
       setbit(sw, SWITCH_NOTAUS_b);
    }
    
    debounce_keys(&g_dccgen.switches, &g_dccgen.switches_t, sw);
}


void do_init_system(void) {
    uint8_t portd_bits = 0
                        |Bit(LED_DCC_NOTAUS_b)|Bit(LED_DCC_ON_b)

                        ;
    port_out(PORTD) = portd_bits;
    PORTCFG_MPCMASK = portd_bits;
    PORTD.PIN0CTRL = PORT_OPC_TOTEM_gc;
    port_dirout(PORTD, portd_bits);

    PORTCFG_MPCMASK = Bit(NOTAUS_b);
    PORTC.PIN0CTRL = PORT_OPC_PULLUP_gc;
    
    // configure sleep mode: idle sleep mode, sleep mode allowed
    SLEEP.CTRL = SLEEP_SMODE_IDLE_gc|Bit(SLEEP_SEN_bp);
    // power reduction
    PR.PRPA = Bit(PR_DAC_bp)
#ifndef USE_BOOSTER
                |Bit(PR_ADC_bp)
#endif
                |Bit(PR_AC_bp);
    PR.PRPB = Bit(PR_DAC_bp)
#ifndef USE_BOOSTER
                |Bit(PR_ADC_bp)
#endif
                |Bit(PR_AC_bp);
    PR.PRPC = Bit(PR_TWI_bp)|Bit(PR_USART1_bp)|Bit(PR_USART0_bp)|Bit(PR_SPI_bp)|Bit(PR_HIRES_bp); // PR_TC1_bp PR_TC0_bp;
    PR.PRPD = Bit(PR_TWI_bp)|Bit(PR_USART1_bp)|Bit(PR_USART0_bp)|Bit(PR_SPI_bp)|Bit(PR_HIRES_bp); // PR_TC1_bp PR_TC0_bp;
    
    g_com.productid = PRODUCT_ID;
    g_com.vendorid = VENDOR_ID;
    g_com.firmware_version = FIRMWARE_VERSION;
    g_com.capabilities = 0
        |CAP_DCC_GENERATOR
        ;
    g_com.cap_class = 0;
    g_com.dev_desc_P = PSTR(DEVICE_DESC);

    timer_register(&g_timer_10ms, TIMER_RESOLUTION_1MS);
    
    dccgen_init();

}

uint8_t do_msg(struct sboxnet_msg_header *pmsg) {
    uint8_t rc = 0;
    if (pmsg->dstaddr == SBOXNET_ADDR_BROADCAST) {
        return SBOXNET_ACKRC_CMD_UNKNOWN;
    }

    switch (pmsg->cmd) {
        case SBOXNET_CMD_LOCO_POWER: {
            if (pmsg->opt.len != 1) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            volatile uint8_t flags = pmsg->data[0];
            if (flags & 0x01) { // on
                if (bit_is_set(g_dccgen.flags, DCCGEN_FLAG_NOTAUS_b)
                        /*||
                        bit_is_set(g_dev_state, DEV_STATE_FLG_WATCHDOG_b)*/) {
                    return SBOXNET_ACKRC_LOCO_NOTAUS;
                }
                if (bit_is_clear(g_dccgen.flags, DCCGEN_FLAG_ON_b) && g_dcc.state == DCC_STATE_NONE) {
                    if (flags & 0x02) {
                        loco_list_reset();
                    } else {
                        loco_list_init();
                    }
                    dcc_start_generator(&g_dcc);
                }
                setbit(g_dccgen.flags, DCCGEN_FLAG_ON_b);
            } else { // off
                dcc_stop_generator(&g_dcc);
                g_locos.pcur = 0;
                if (!(flags & 0x02)) {
                    loco_list_init();
                }
                clrbit(g_dccgen.flags, DCCGEN_FLAG_ON_b);
                clrbit(g_dccgen.flags, DCCGEN_FLAG_NOTAUS_b);
            }
            pmsg->opt.len = 0;
            return SBOXNET_ACKRC_OK;
        }
        case SBOXNET_CMD_LOCO_DRIVE: {
            uint8_t msglen = pmsg->opt.len;
            if (msglen != 3 && msglen != 5) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            uint16_t addr = ((uint16_t)pmsg->data[1] << 8) | pmsg->data[0];
            rc = loco_set_speed(addr, pmsg->data[2]);
            if (rc) {
                return rc;
            }
            if (msglen == 5) {
                rc = loco_set_func(addr, pmsg->data[3], pmsg->data[4]);
                if (rc) {
                    return rc;
                }
            }
            pmsg->opt.len = 0;
            return SBOXNET_ACKRC_OK;
        }
        
        case SBOXNET_CMD_LOCO_FUNC: {
            if (pmsg->opt.len != 4) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            uint16_t addr = ((uint16_t)pmsg->data[1] << 8) | pmsg->data[0];
            rc = loco_set_func(addr, pmsg->data[2], pmsg->data[3]);
            pmsg->opt.len = 0;
            return rc;
        }
        
        case SBOXNET_CMD_LOCO_ADD: {
            if (pmsg->opt.len != 3) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            uint16_t addr = ((uint16_t)pmsg->data[1] << 8) | pmsg->data[0];
            uint8_t flags = pmsg->data[2];
            pmsg->opt.len = 0;
            return loco_add(addr, flags, 0);
        }
        
        case SBOXNET_CMD_LOCO_DEL: {
            if (pmsg->opt.len != 2) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            uint16_t addr = ((uint16_t)pmsg->data[1] << 8) | pmsg->data[0];
            pmsg->opt.len = 0;
            return loco_del(addr);
        }
        
        case SBOXNET_CMD_LOCO_POM: {
            if (pmsg->opt.len != 5) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            uint16_t addr = ((uint16_t)pmsg->data[1] << 8) | pmsg->data[0];
            uint16_t cv = ((uint16_t)pmsg->data[3] << 8) | pmsg->data[2];
            uint8_t data = pmsg->data[4];
            pmsg->opt.len = 0;
            return loco_pom(addr, cv, data);
        }
    }
    return SBOXNET_ACKRC_CMD_UNKNOWN;
}

uint8_t do_reg_read(uint16_t reg, uint16_t* pdata) {
    switch(reg) {
        case R_DCCGEN_FLAGS:     *pdata = g_dccgen.flags; return 0;
        case R_DCCGEN_NUM_LOCOS: *pdata = g_locos.numlocos; return 0;
        case R_DCCGEN_LOCOADDR_SCAN_MAX: *pdata = g_dccgen.locoaddr_scan_max; return 0;
        case R_DCCGEN_LOCOADDR_SCAN_CUR: *pdata = g_dccgen.locoaddr_scan_cur; return 0;
    }    
    return SBOXNET_ACKRC_REG_INVALID;
};

uint8_t do_reg_write(uint16_t reg, uint16_t data, uint16_t mask) {
    switch (reg) {
        case R_DCCGEN_LOCOADDR_SCAN_MAX: {
            g_dccgen.locoaddr_scan_max = data;
            g_dccgen.eeprom_flags.write_locoaddr_scan_max = 1;
            return 0;
        }
    }
    return SBOXNET_ACKRC_REG_INVALID;
}


void do_setup(void) {
}

static void led_set(uint8_t bit, uint8_t on) {
    if (on) {
        port_clrbit(LED_PORT, bit);
    } else {
        port_setbit(LED_PORT, bit);
    }
}
    
void do_main(void) {
    if (timer_timedout(&g_timer_10ms)) {
        timer_set(&g_timer_10ms, 10);
        
        loco_timing();
        dcc_read_switches();
    }
    
    loco_task();
    
    if (bit_is_set(g_dccgen.switches, SWITCH_NOTAUS_b)) {
        setbit(g_dccgen.flags, DCCGEN_FLAG_NOTAUS_b);
    }
    if (/*bit_is_set(g_dev_state, DEV_STATE_FLG_WATCHDOG_b) ||*/ bit_is_set(g_dccgen.flags, DCCGEN_FLAG_NOTAUS_b)) {
        if (bit_is_set(g_dccgen.flags, DCCGEN_FLAG_ON_b)) {
            clrbit(g_dccgen.flags, DCCGEN_FLAG_ON_b);
            dcc_stop_generator(&g_dcc);
        }
    }
    
    led_set(LED_DCC_ON_b,     bit_is_set(g_dccgen.flags, DCCGEN_FLAG_ON_b));
    led_set(LED_DCC_NOTAUS_b, bit_is_set(g_dccgen.flags, DCCGEN_FLAG_NOTAUS_b));

    if (g_dccgen.eeprom_flags.write_locoaddr_scan_max && eeprom_is_ready()) {
        eeprom_update_word(&g_eeprom.dccgen.locoaddr_scan_max, g_dccgen.locoaddr_scan_max);
        g_dccgen.eeprom_flags.write_locoaddr_scan_max = 0;
    }
    
    sleep_cpu();
}

void do_before_bldr_activate(void) {

    dcc_stop_generator(&g_dcc);
    
    PORTC.INT0MASK = 0;
    PORTC.INT1MASK = 0;
    PORTC.INTFLAGS = 0xff; // clear interrupt flags;
    PORTC.INTCTRL = 0;
}
