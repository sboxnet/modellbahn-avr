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

#include "common.h"

#include <avr/eeprom.h>

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

static uint16_t com_stack_size(void) {
    return (uint8_t*)BLDR_STACK_TOP - &_end;
}


#include "avrutilslib/ringbuffer.c"


#define TIMER_PRESCALER   64
#define TIMER_FREQ_HZ     1000
#define TIMER_PERIOD        (F_CPU / TIMER_PRESCALER / TIMER_FREQ_HZ)  // 500

struct com_ram g_com = { 0, };

uint8_t bitmask(uint8_t bit) {
	static const uint8_t offsmap[8] PROGMEM = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
	return pgm_read_byte(offsmap + bit);
}

__ATTR_WEAK void do_init_system(void) {
}

__ATTR_WEAK uint8_t do_reg_read(uint16_t reg, uint16_t* pdata) {
    return SBOXNET_ACKRC_REG_INVALID;
}

__ATTR_WEAK uint8_t do_reg_write(uint16_t reg, uint16_t data, uint16_t mask) {
    return SBOXNET_ACKRC_REG_INVALID;
}

__ATTR_WEAK uint8_t do_msg(struct sboxnet_msg_header *pmsg) {
    return SBOXNET_ACKRC_CMD_UNKNOWN;
}

__ATTR_WEAK void do_setup(void) {
}

__ATTR_WEAK void do_main(void) {
}

__ATTR_WEAK void do_before_bldr_activate(void) {
}

// mtester weaks
/*
__ATTR_WEAK void mtester_do_init_system(void) {
}

__ATTR_WEAK uint8_t mtester_do_reg_read(uint16_t reg, uint16_t* pdata) {
	return SBOXNET_ACKRC_REG_INVALID;
}

__ATTR_WEAK uint8_t mtester_do_reg_write(uint16_t reg, uint16_t data, uint16_t mask) {
	return SBOXNET_ACKRC_REG_INVALID;
}

__ATTR_WEAK uint8_t mtester_do_msg(struct sboxnet_msg_header *pmsg) {
	return SBOXNET_ACKRC_CMD_UNKNOWN;
}

__ATTR_WEAK void mtester_do_setup(void) {
}

__ATTR_WEAK void mtester_do_main(void) {
}

__ATTR_WEAK void mtester_do_before_bldr_activate(void) {
}
*/

// gbm weaks
__ATTR_WEAK void gbm_do_init_system(void) {
}

__ATTR_WEAK uint8_t gbm_do_reg_read(uint16_t reg, uint16_t* pdata) {
	return SBOXNET_ACKRC_REG_INVALID;
}

__ATTR_WEAK uint8_t gbm_do_reg_write(uint16_t reg, uint16_t data, uint16_t mask) {
	return SBOXNET_ACKRC_REG_INVALID;
}

__ATTR_WEAK uint8_t gbm_do_msg(struct sboxnet_msg_header *pmsg) {
	return SBOXNET_ACKRC_CMD_UNKNOWN;
}

__ATTR_WEAK void gbm_do_setup(void) {
}

__ATTR_WEAK void gbm_do_main(void) {
}

__ATTR_WEAK void gbm_do_before_bldr_activate(void) {
}

/*
// ws weaks
__ATTR_WEAK void ws_do_init_system(void) {
}

__ATTR_WEAK uint8_t ws_do_reg_read(uint16_t reg, uint16_t* pdata) {
	return SBOXNET_ACKRC_REG_INVALID;
}

__ATTR_WEAK uint8_t ws_do_reg_write(uint16_t reg, uint16_t data, uint16_t mask) {
	return SBOXNET_ACKRC_REG_INVALID;
}

__ATTR_WEAK uint8_t ws_do_msg(struct sboxnet_msg_header *pmsg) {
	return SBOXNET_ACKRC_CMD_UNKNOWN;
}

__ATTR_WEAK void ws_do_setup(void) {
}

__ATTR_WEAK void ws_do_main(void) {
}

__ATTR_WEAK void ws_do_before_bldr_activate(void) {
}
*/

/*
// dcc weaks
__ATTR_WEAK void dg_do_init_system(void) {
}

__ATTR_WEAK uint8_t dg_do_reg_read(uint16_t reg, uint16_t* pdata) {
	return SBOXNET_ACKRC_REG_INVALID;
}

__ATTR_WEAK uint8_t dg_do_reg_write(uint16_t reg, uint16_t data, uint16_t mask) {
	return SBOXNET_ACKRC_REG_INVALID;
}

__ATTR_WEAK uint8_t dg_do_msg(struct sboxnet_msg_header *pmsg) {
	return SBOXNET_ACKRC_CMD_UNKNOWN;
}

__ATTR_WEAK void dg_do_setup(void) {
}

__ATTR_WEAK void dg_do_main(void) {
}

__ATTR_WEAK void dg_do_before_bldr_activate(void) {
}
*/

/*
// booster weaks
__ATTR_WEAK void bo_do_init_system(void) {
}

__ATTR_WEAK uint8_t bo_do_reg_read(uint16_t reg, uint16_t* pdata) {
	return SBOXNET_ACKRC_REG_INVALID;
}

__ATTR_WEAK uint8_t bo_do_reg_write(uint16_t reg, uint16_t data, uint16_t mask) {
	return SBOXNET_ACKRC_REG_INVALID;
}

__ATTR_WEAK uint8_t bo_do_msg(struct sboxnet_msg_header *pmsg) {
	return SBOXNET_ACKRC_CMD_UNKNOWN;
}

__ATTR_WEAK void bo_do_setup(void) {
}

__ATTR_WEAK void bo_do_main(void) {
}

__ATTR_WEAK void bo_do_before_bldr_activate(void) {
}
*/
/*
void bo_do_init_system(void);
uint8_t bo_do_msg(struct sboxnet_msg_header *pmsg);
uint8_t bo_do_reg_read(uint16_t reg, uint16_t* pdata);
uint8_t bo_do_reg_write(uint16_t reg, uint16_t data, uint16_t mask);
void bo_do_setup(void);
void bo_do_main(void);
void bo_do_before_bldr_activate(void);
*/
// end weaks

struct Eeprom_t eeprom EEMEM;

static uint8_t com_process_cmd_reset(void) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        g_v.dev_addr = SBOXNET_ADDR_BROADCAST;
        setbit(g_dev_state, DEV_STATE_FLG_REQ_ADDR_b);
    }
    return SBOXNET_ACKRC_SEND_NO_ANSWER;
}

static uint8_t com_reg_read(uint16_t reg, uint16_t *pdata) {
    switch (reg) {
        case R_DEBUG_STACK_SIZE:    *pdata = com_stack_size(); return 0;
        case R_DEBUG_STACK_FREE:    *pdata = com_stack_free(); return 0;
    }
        
    uint8_t rc = bldr_reg_read(reg, pdata);
    if (rc == 0)
        return rc;
    
    switch (reg) {
        case R_FIRMWARE_VERSION:    *pdata = g_com.firmware_version; return 0;
        case R_CAPABILITIES:        *pdata = g_com.capabilities; return 0;
        case R_CAP_CLASS:           *pdata = g_com.cap_class; return 0;
    }
    return com_sched_do_reg_read(reg, pdata);
}

static uint8_t com_reg_write(uint16_t reg, uint16_t data, uint16_t mask) {
    return com_sched_do_reg_write(reg, data, mask);
}

static uint8_t com_cmd_reg_read(struct sboxnet_msg_header *pmsg) {
    uint16_t *pdata = (uint16_t*)pmsg->data;
    uint16_t regaddr = *pdata;
    uint8_t num = pmsg->data[2];
    if (pmsg->opt.len != 3 || num > SBOXNET_MSG_DATA_LEN/2 ) {
        return SBOXNET_ACKRC_INVALID_ARG;
    }
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

static uint8_t com_cmd_reg_read_multi(struct sboxnet_msg_header *pmsg) {
    uint16_t *pdata = (uint16_t*)pmsg->data;
    if (pmsg->opt.len > SBOXNET_MSG_DATA_LEN || (pmsg->opt.len & 0x01)) {
        return SBOXNET_ACKRC_INVALID_ARG;
    }
    uint8_t n = pmsg->opt.len / 2;
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

static uint8_t com_process_std_msg(struct sboxnet_msg_header *pmsg) {
    
    switch (pmsg->cmd) {
        case SBOXNET_CMD_NET_RESET: {
            if (pmsg->srcaddr == 0) {
                return com_process_cmd_reset();
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
            return com_cmd_reg_read(pmsg);
        }
        case SBOXNET_CMD_REG_READ_MULTI: {
            return com_cmd_reg_read_multi(pmsg);
        }
        case SBOXNET_CMD_REG_WRITE: {
            uint16_t *pdata = (uint16_t*)pmsg->data;
            uint16_t regaddr = pdata[0];
            uint16_t regval  = pdata[1];
            uint16_t mask    = (pmsg->opt.len == 6 ? pdata[2] : 0xffff);
            if (pmsg->opt.len != 4 && pmsg->opt.len != 6) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            pmsg->opt.len = 0;
            return com_reg_write(regaddr, regval, mask);
        }
        case SBOXNET_CMD_REG_WRITE_BIT: {
            uint16_t* pdata = (uint16_t*)pmsg->data;
            uint16_t regaddr = pdata[0];
            uint8_t val = pmsg->data[2];
            if (pmsg->opt.len != 3) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            uint16_t data = 0;
            uint16_t mask = 0x01;
            mask <<= (val & 0x0f);
            if (val & 0x80) {
                data = mask;
            }
            pmsg->opt.len = 0;
            return com_reg_write(regaddr, data, mask);
        }
        case SBOXNET_CMD_DEV_SET_DESC: {
            uint8_t n = pmsg->opt.len;
            if (n == 0 || n > DEV_DESC_MAX_SIZE + 1) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            uint8_t id = pmsg->data[0];
            if (id == 0 || id >= DEV_NUM_DESCS) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            n--;
            if (n < DEV_DESC_MAX_SIZE) {
                pmsg->data[1+n] = 0;
                n++;
            }
            eeprom_update_block(&pmsg->data[1], bldr_eeprom.dev_desc[id], n);
            pmsg->opt.len = 0;
            return SBOXNET_ACKRC_OK;
        }
        default:
            return bldr_process_basic_msg(pmsg);        
    }
    return SBOXNET_ACKRC_CMD_UNKNOWN;
}


static void com_process_msg(struct sboxnet_msg_header *pmsg) {
    uint8_t rc = 0;
    
    if (pmsg->srcaddr == 0) {
        g_v.timer_watchdog = WATCHDOG_VAL;
    }
   
    rc = com_sched_do_msg(pmsg);
    if (rc == SBOXNET_ACKRC_OK) {
        goto send_ok;
    } else if (rc == SBOXNET_ACKRC_SEND_NO_ANSWER) {
        return;
    } else if (rc != SBOXNET_ACKRC_CMD_UNKNOWN) {
        goto send_error;
    }

    if (pmsg->cmd & 0x80) {
        return;  // discard Ack Answers
    }
    
    rc = com_process_std_msg(pmsg);
    if (rc == SBOXNET_ACKRC_SEND_NO_ANSWER) {
        return;
    }
    if (rc == SBOXNET_ACKRC_OK) {
        goto send_ok;
    }
send_error:
    pmsg->opt.len = 2;
    pmsg->data[0] = pmsg->cmd;
    pmsg->data[1] = rc;
    pmsg->cmd = 0x80;

send_ok:
    // send anwser
    pmsg->dstaddr = pmsg->srcaddr;
    pmsg->cmd |= 0x80;
    sboxnet_send_msg(pmsg);
}

static void com_receive_and_process_msg(struct sboxnet_msg_max* pmsg) {    
    int8_t rc = sboxnet_receive_msg(&pmsg->msgh, sizeof(*pmsg));
    if (rc > 0) {
        com_process_msg(&pmsg->msgh);

    } else if (rc == SBOXNET_ERR_RECVMSG_CRC) {
        setbit(g_dev_errflags, DEV_ERR_FLG_SBOXNETCRC_b); // crc error
    }
}


void com_sched_init_system(void) {
	volatile uint16_t t = g_v.module;
	switch(t) {
		case MODULE_TESTER:
		{
			mtester_do_init_system();
			return;
		}
		case MODULE_GBM:
		{
			gbm_do_init_system();
			return;
		}
		case MODULE_WS:
		{
			ws_do_init_system();
			return;
		}
		case MODULE_DCC:
		{
			dg_do_init_system();
			return;
		}
		case MODULE_BOOSTER:
		{
			bo_do_init_system();
			return;
		}
		default:
			return;
	}
}

uint8_t com_sched_do_msg(struct sboxnet_msg_header *pmsg) {
	switch(g_v.module) {
		case MODULE_TESTER:
			return mtester_do_msg(pmsg);

		case MODULE_GBM:
			return gbm_do_msg(pmsg);
			
		case MODULE_WS:
			return ws_do_msg(pmsg);
			
		case MODULE_DCC:
			return dg_do_msg(pmsg);

		case MODULE_BOOSTER:
			return bo_do_msg(pmsg);

		default:
			return SBOXNET_ACKRC_CMD_UNKNOWN;
	}
}

void com_sched_do_setup(void) {
	switch(g_v.module) {
		case MODULE_TESTER:
			mtester_do_setup();
			return;

		case MODULE_GBM:
			gbm_do_setup();
			return;
		
		case MODULE_WS:
			ws_do_setup();
			return;
		
		case MODULE_DCC:
			dg_do_setup();
			return;

		case MODULE_BOOSTER:
			bo_do_setup();
			return;

		default:
			return;
	}
}

void com_sched_do_main(void) {
	switch(g_v.module) {
		case MODULE_TESTER:
			mtester_do_main();
			return;

		case MODULE_GBM:
			gbm_do_main();
			return;
			
		case MODULE_WS:
			ws_do_main();
			return;
			
		case MODULE_DCC:
			dg_do_main();
			return;
			
		case MODULE_BOOSTER:
			bo_do_main();
			return;

		default:
			return;
	}
}

void com_sched_do_before_bldr_activate(void) {
	// stop timers
	TCC0.INTCTRLA = 0;
	TCC0.INTCTRLB = 0;
	TCC0.CTRLA = 0; // stop timer
	TCC0.INTFLAGS = 0xff; // clear interrupt flags

	TCC1.INTCTRLA = 0;
	TCC1.INTCTRLB = 0;
	TCC1.CTRLA = 0; // stop timer
	TCC1.INTFLAGS = 0xff; // clear interrupt flags

	TCD0.INTCTRLA = 0;
	TCD0.INTCTRLB = 0;
	TCD0.CTRLA = 0; // stop timer
	TCD0.INTFLAGS = 0xff; // clear interrupt flags

	TCD1.INTCTRLA = 0;
	TCD1.INTCTRLB = 0;
	TCD1.CTRLA = 0; // stop timer
	TCD1.INTFLAGS = 0xff; // clear interrupt flags
	
	TCE0.INTCTRLA = 0;
	TCE0.INTCTRLB = 0;
	TCE0.CTRLA = 0;
	TCE0.INTFLAGS = 0xff;

	switch(g_v.module) {
		case MODULE_TESTER:
			mtester_do_main();
			return;

		case MODULE_GBM:
			gbm_do_main();
			return;
		
		case MODULE_WS:
			ws_do_main();
			return;
		
		case MODULE_DCC:
			dg_do_main();
			return;
		
		case MODULE_BOOSTER:
			bo_do_main();
			return;

		default:
			return;
	}
}

uint8_t com_sched_do_reg_read(uint16_t reg, uint16_t* pdata) {
	switch(g_v.module) {
		case MODULE_TESTER:
			return mtester_do_reg_read(reg, pdata);

		case MODULE_GBM:
			return gbm_do_reg_read(reg, pdata);
		
		case MODULE_WS:
			return ws_do_reg_read(reg, pdata);
		
		case MODULE_DCC:
			return dg_do_reg_read(reg, pdata);
		
		case MODULE_BOOSTER:
		return bo_do_reg_read(reg, pdata);

		default:
			return SBOXNET_ACKRC_REG_INVALID;
	}
}

uint8_t com_sched_do_reg_write(uint16_t reg, uint16_t data, uint16_t mask) {
	switch(g_v.module) {
		case MODULE_TESTER:
			return mtester_do_reg_write(reg, data, mask);

		case MODULE_GBM:
			return gbm_do_reg_write(reg, data, mask);
		
		case MODULE_WS:
			return ws_do_reg_write(reg, data, mask);
		
		case MODULE_DCC:
			return dg_do_reg_write(reg, data, mask);
		
		case MODULE_BOOSTER:
			return bo_do_reg_write(reg, data, mask);

		default:
		return SBOXNET_ACKRC_REG_INVALID;
	}

}

static void com_init(void) {
	//bldr_init_ports();
	bldr_init_modules();
	//bldr_init_devvars();
	//bldr_switch_to_32MHz();
	//bldr_init_pr();
	//bldr_init_sboxnettimer();
	bldr_init_vars();
}

static void com_init_system(void) {
	com_init();
    com_sched_init_system();
    
    // timer
    TCD1.CTRLB = TC_WGMODE_NORMAL_gc;
    TCD1.CTRLD = 0;
    TCD1.CTRLE = 0;
    TCD1.INTCTRLA = 0;
    TCD1.INTCTRLB = TC_CCAINTLVL_LO_gc;
    TCD1.INTFLAGS = 0xff;
    TCD1.PER = 0xffff;
    //TCD1.CCA = TCD1.CNT + TIMER_PERIOD;
    TCD1.CTRLA = TC_CLKSEL_DIV64_gc;

    // write product and vendor id to eeprom
    eeprom_update_word(&bldr_eeprom.productid, g_com.productid);
    eeprom_update_word(&bldr_eeprom.vendorid, g_com.vendorid);
    
    // write device desc to eeprom
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
}

// every 1ms
ISR(TCD1_CCA_vect) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		volatile uint16_t tcnt = TCD1.CNT;
		volatile uint16_t tcca = TCD1.CCA;
		volatile uint16_t tcca2 = tcca + TIMER_PERIOD;
		TCD1.CCA = tcca2;
        //TCD1.CCA += TIMER_PERIOD;
		volatile uint8_t x = 1;
    }
    g_com.timer++;
    struct timer *t;
    if ((g_com.timer & 0x0f) == 0) {
        t = g_com.timer_16ms;
        while (t) {
            if (t->value > 0) {
                t->value--;
            }
            t = t->next;
        }
    }
    t = g_com.timer_1ms;
    while (t) {
        if (t->value > 0) {
            t->value--;
        }
        t = t->next;
    }
}

__ATTR_OS_MAIN int main(void) {
    cli();
    com_init_system();

    com_process_cmd_reset();
    sboxnet_init();
   
    sei();

    com_sched_do_setup();
    
    while(1) {
        struct sboxnet_msg_max msg;
        
        if (!bldr_task(&msg)) {
            com_receive_and_process_msg(&msg);

            if (g_dev_state & DEV_STATE_FLG_FWUP_MASK) {
                while (!sboxnet_all_sent()); // wait until sboxnet transmitter is empty
                
                cli();
                
                com_sched_do_before_bldr_activate();
                
                TCC0.INTCTRLA = 0;
                TCC0.INTCTRLB = 0;
                TCC0.CTRLA = 0; // stop timer
                TCC0.INTFLAGS = 0xff; // clear interrupt flags
                TCC1.INTCTRLA = 0;
                TCC1.INTCTRLB = 0;
                TCC1.CTRLA = 0; // stop timer
                TCC1.INTFLAGS = 0xff; // clear interrupt flags

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
            } else {
                com_sched_do_main();
            }
        } else {
            1; //sleep_cpu();
        }
    }
    
    return 0;
}

#include "mtester.c"
#include "weichen-servo.c"
//#include "gbmelder.c"
#include "dccgen.c"

#include "booster.c"
