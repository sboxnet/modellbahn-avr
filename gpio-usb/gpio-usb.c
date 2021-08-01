 /***************************************************************************
 *   Copyright (C) 2013-2014
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

#define SYSCLK_FREQ_MHZ 8
#define SYSCLK_DIVISOR  1
#define F_CPU   (SYSCLK_FREQ_MHZ * 1000000 / SYSCLK_DIVISOR)

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/cpufunc.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "avrutilslib/defines.h"

#define USE_LOGGER_USART 0
#include "avrutilslib/usb/usb.c"


/******
 *  0000 0000       clear error flags               () -> ()
    0000 0001       get error flags                 () -> (errflags)
    0000 0010       get software version            () -> (version)
    0000 0011       get software ident string       () -> (n-byte string terminated by 0)
    0000 0100       get flags                       () -> (flags)
    0000 0101       get receiver buffer size        () -> (bufsize)
    0000 1000       enter avr prog mode             () -> (RC_OK)
    0000 1001       exit avr prog mode              () -> (RC_OK)
    0000 1010       read io register                (addr) -> (byte)
    0000 1011       put io register                 (addr,byte,mask) -> ()
    0000 1110       software reset                  () -> ()
    
    0001 0000       avr: set prog mode              (flags,sclk-period) -> (new_flags)
                                                    FLG_B_AVRPROGMODE:  if set -> programming mode on
                                                    FLG_B_AVRWAITx:     programming wait mode
                                                    sclk-period:        serial clock period = (sclk-period + 1) * 10us                                                  
    0001 0001       avr: raw command                (b1,b2,b3,b4) -> (r2,r3,r4)
    0001 0010       avr: read flash                 (addrh,addrl,nh,nl) -> (n*(wordl,wordh),RC_OK)
    0001 0011       avr: read eeprom                (addrh,addrl,nh,nl) -> (n*byte, RC_OK)
    0001 0100       avr: write flash page           (addrh,addrl,n,(2^n)*(wordl,wordh)) -> (((2^n)-1)*RC_NEXT,RC_OK)
    0001 0101       avr: write eeprom page          (addrh,addrl,n,(2^n)*byte) -> (((2^n)-1)*RC_NEXT,RC_OK)
    0001 0110       avr: write eeprom byte          (addrh,addrl,byte) -> (RC_OK)
    0001 0111       avr: load extended address      (addrx) -> (RC_OK)
    0001 1000       avr: chip erase                 () -> (RC_OK)
    0001 1001       avr: program enable             () -> (RC_OK|RC_NOSYNC)
    0001 1010       avr: read signature             () -> (s1,s2,s3,RC_OK)
    0001 1011       avr: read lock                  () -> (r,RC_OK)
    0001 1100       avr: write lock                 (b) -> (RC_OK)
    0001 1101       avr: read fuse                  (n) -> (r,RC_OK)
    0001 1110       avr: write fuse                 (n,b) -> (RC_OK)
    0001 1111       avr: read cal.byte              (n) -> (r,RC_OK)
**********/

enum FLAGS {
    FLG_AVRMODE     = 0x0001,
    FLG_AVRPROGMODE = 0x0002,
    FLG_AVRWAIT0    = 0x0004,
    FLG_AVRWAIT1    = 0x0008,
};
enum AvrWait {
    AVRWAIT_MASK    = 0x0c,
    AVRWAIT_POLL    = 0x00,
    AVRWAIT_8MS     = 0x04,
    AVRWAIT_16MS    = 0x08,
    AVRWAIT_64MS    = 0x0c,
};
enum ERRORFLAGS {
    ERRFLG_AVRCMD       = (1<<7),
    ERRFLG_FRAMEERR     = (1<<4),
    ERRFLG_DATAOV       = (1<<3),
    ERRFLG_PARITYERR    = (1<<2),
    ERRFLG_UNKNOWNCMD   = (1<<0),
};
enum CMDS {
    //CMD_CLR_ERROR_FLAGS = 0x00,
    //CMD_GET_ERROR_FLAGS = 0x01,
    //CMD_GET_VERSION     = 0x02,
    //CMD_GET_VERSION_STR = 0x03,
    //CMD_GET_FLAGS       = 0x04,
    //CMD_GET_RECVBUFSIZE = 0x05,
    CMD_ENTER_AVRMODE   = 0x08,
    CMD_LEAVE_AVRMODE   = 0x09,
    //CMD_READ_IO_REG     = 0x0a,
    //CMD_PUT_IO_REG      = 0x0b,
    //CMD_SOFT_RESET      = 0x0e,
    
    CMD_AVR_PROGMODE    = 0x10,
    CMD_AVR_RAWCMD      = 0x11,
    CMD_AVR_READ_PRGMEM = 0x12,
    CMD_AVR_READ_EEPROM = 0x13,
    CMD_AVR_WRITE_PRGMEM_PAGE   = 0x14,
    CMD_AVR_WRITE_EEPROM_PAGE   = 0x15,
    CMD_AVR_WRITE_EEPROM_BYTE   = 0x16,
    CMD_AVR_LOAD_EXT_ADDR   = 0x17,
    CMD_AVR_CHIP_ERASE      = 0x18,
    CMD_AVR_PROGRAMENABLE   = 0x19,
    CMD_AVR_READ_SIGNATURE  = 0x1a,
    CMD_AVR_READ_LOCKBITS   = 0x1b,
    CMD_AVR_WRITE_LOCKBITS  = 0x1c,
    CMD_AVR_READ_FUSEBITS   = 0x1d,
    CMD_AVR_WRITE_FUSEBITS  = 0x1e,
    CMD_AVR_READ_CALIBR     = 0x1f,
    CMD_AVR_GET_STATUS      = 0x20,
       
    CMD_PDI_ENABLE         = 0x40,
    CMD_PDI_DISABLE        = 0x41,
    CMD_PDI_LDS            = 0x42,
    CMD_PDI_STS            = 0x43,
    CMD_PDI_LD             = 0x44,
    CMD_PDI_ST             = 0x45,
    CMD_PDI_LDCS           = 0x46,
    CMD_PDI_STCS           = 0x47,
    CMD_PDI_REPEAT         = 0x48,
    CMD_PDI_KEY            = 0x49,
    CMD_PDI_GETSTATUS      = 0x4a,
    CMD_ENTER_PDI          = 0x4b,
    CMD_LEAVE_PDI          = 0x4c,
    
    CMD_NVMC_ENABLE           = 0x50,
    CMD_NVMC_DISABLE          = 0x51,
    CMD_NVMC_FLASH_READ       = 0x52,
    CMD_NVMC_FLASH_WRITE_PAGE = 0x53,
    CMD_NVMC_EEPROM_READ      = 0x54,
    CMD_NVMC_EEPROM_WRITE     = 0x55,
    CMD_NVMC_FUSE_READ        = 0x56,
    CMD_NVMC_FUSE_WRITE       = 0x57,
    CMD_NVMC_SIGNROW_READ     = 0x58,
    CMD_NVMC_NVM_READ         = 0x59,
    CMD_NVMC_GET_DEVID        = 0x60,
    CMD_NVMC_ERASE_CHIP       = 0x61,
};


#define AVRSPI_PORT  PORTB
#define AVRSPI_DDR   DDRB
#define AVRSPI_PIN   PINB
#define AVRSPIB_SCLK PB1
#define AVRSPIB_MOSI PB2
#define AVRSPIB_MISO PB3
#define AVRSPIB_RESET PB0
#define AVRSPIB_POWER PB4

#define STATUS_LED_PORT PORTB
#define STATUS_LED_DDR  DDRB
#define STATUS_LED_BIT PB5

#define FLG_B_AVRMODE     0
#define FLG_B_AVRPROGMODE 1
#define FLG_B_AVRWAIT0    2
#define FLG_B_AVRWAIT1    3
#define AVRFLG_MASK     (Bit(FLG_B_AVRPROGMODE)|Bit(FLG_B_AVRWAIT0)|Bit(FLG_B_AVRWAIT1))

#define AVR_ERRF_NOSYNC        0x01
#define AVR_ERRF_POLL_TIMEOUT  0x02

#define AVRWAIT_MASK        0x0c
#define AVRWAIT_POLL        0x00
#define AVRWAIT_8MS         0x04
#define AVRWAIT_16MS        0x08
#define AVRWAIT_64MS        0x0c

uint8_t g_avr_flags = 0;
uint8_t g_avr_error = 0;
uint8_t g_avr_clk_wait = 0;
volatile uint8_t g_avr_timer = 0;

volatile uint8_t g_pdi_timer = 0;
volatile uint8_t g_nvmc_timer = 0;
uint8_t g_pdi_last_rc = 0;

#define PDI_FLAG_PDI_MODE     0
#define PDI_FLAG_PDI_ENABLED  1
#define PDI_FLAG_NVMC_ENABLED 2
uint8_t g_pdi_flags = 0;

#define AVR_TIMER_PRESCALER 8
#define AVR_TIMER_FREQ_HZ   500
#define AVR_TIMER_DIFF  (F_CPU / (AVR_TIMER_PRESCALER * AVR_TIMER_FREQ_HZ) - 1)  // 2000-1
#define AVR_TIMER_MS(_x)    ((_x)/2)

ISR(TIMER1_COMPA_vect) { // every 2ms
    OCR1A += AVR_TIMER_DIFF;
    if (g_avr_timer) {
        g_avr_timer--;
    }
    if (g_pdi_timer) {
        g_pdi_timer--;
    }
    if (g_nvmc_timer) {
        g_nvmc_timer--;
    }
}


void avr_clkwait(void) {
    uint8_t wait = g_avr_clk_wait;
    do {
        _delay_us(5);
    } while (wait--);
}

uint8_t avr_sendbyte(uint8_t b) {
    if (bit_is_clear(g_avr_flags, FLG_B_AVRPROGMODE))
        return 0;
    
    uint8_t r = 0;
    clrbit(AVRSPI_PORT, AVRSPIB_SCLK);
    for (uint8_t i = 0; i < 8; i++) {
        if (b & 0x80) {
            setbit(AVRSPI_PORT, AVRSPIB_MOSI);
        } else {
            clrbit(AVRSPI_PORT, AVRSPIB_MOSI);
        }
        b <<= 1;
        
        avr_clkwait();
        
        r <<= 1;
        if (bit_is_set(AVRSPI_PIN, AVRSPIB_MISO)) {
            r |= 0x01;
        }
        
        setbit(AVRSPI_PORT, AVRSPIB_SCLK);
        avr_clkwait();
        clrbit(AVRSPI_PORT, AVRSPIB_SCLK);
    }
    return r;
}

void cmd_avr_init(void) {
    AVRSPI_DDR &= ~(Bit(AVRSPIB_SCLK)|Bit(AVRSPIB_MISO)|Bit(AVRSPIB_MOSI)|Bit(AVRSPIB_RESET)|Bit(AVRSPIB_POWER)); // all SPI pins to input
    AVRSPI_PORT &= ~(Bit(AVRSPIB_SCLK)|Bit(AVRSPIB_MISO)|Bit(AVRSPIB_MOSI)|Bit(AVRSPIB_RESET));
    setbit(AVRSPI_PORT, AVRSPIB_POWER);
    AVRSPI_DDR |= (Bit(AVRSPIB_SCLK)|Bit(AVRSPIB_MOSI)|Bit(AVRSPIB_RESET)|Bit(AVRSPIB_POWER));
    
    setbit(g_avr_flags, FLG_B_AVRMODE);
    clrbit(g_avr_flags, FLG_B_AVRPROGMODE);
    clrbit(g_avr_flags, FLG_B_AVRWAIT0);
    clrbit(g_avr_flags, FLG_B_AVRWAIT1);
    
    usb_ep_send_zlp();
}

void cmd_avr_exit(void) {
    AVRSPI_DDR &= ~(Bit(AVRSPIB_SCLK)|Bit(AVRSPIB_MISO)|Bit(AVRSPIB_MOSI)|Bit(AVRSPIB_RESET)|Bit(AVRSPIB_POWER)); // all SPI pins to input
    AVRSPI_PORT |= (Bit(AVRSPIB_SCLK)|Bit(AVRSPIB_MISO)|Bit(AVRSPIB_MOSI)|Bit(AVRSPIB_RESET)|Bit(AVRSPIB_POWER)); // pullup's enable
    
    g_avr_flags &= ~(Bit(FLG_B_AVRMODE)|Bit(FLG_B_AVRPROGMODE)|Bit(FLG_B_AVRWAIT0)|Bit(FLG_B_AVRWAIT1));
    
    usb_ep_send_zlp();
}

void cmd_avr_progmode(uint8_t flags, uint8_t sclk_period, uint16_t wLength) {
    if (bit_is_set(g_avr_flags, FLG_B_AVRMODE)) {
        if (bit_is_set(flags, FLG_B_AVRPROGMODE)) {
            g_avr_clk_wait = sclk_period;
            g_avr_flags = (g_avr_flags & ~AVRFLG_MASK) | (flags & AVRFLG_MASK);
            AVRSPI_PORT &= ~(Bit(AVRSPIB_SCLK)|Bit(AVRSPIB_MOSI)|Bit(AVRSPIB_RESET));
            setbit(AVRSPI_PORT, AVRSPIB_POWER);
            _delay_us(10);
            clrbit(AVRSPI_PORT, AVRSPIB_POWER); // power on
            goto do_answer;
            
        } else {
            setbit(AVRSPI_PORT, AVRSPIB_POWER); // power off
            AVRSPI_PORT &= ~(Bit(AVRSPIB_SCLK)|Bit(AVRSPIB_MOSI)|Bit(AVRSPIB_RESET));
        }
    }
    g_avr_flags &= ~(Bit(FLG_B_AVRPROGMODE)|Bit(FLG_B_AVRWAIT0)|Bit(FLG_B_AVRWAIT1));
    g_avr_clk_wait = 0;
    
do_answer:
    usb_ep_send_control_data(&g_avr_flags, 1, USB_MEM_SRC_RAM, wLength);
}

void cmd_avr_rawcmd(uint16_t w1, uint16_t w2, uint16_t wLength) {
    uint8_t ret[3];
    avr_sendbyte(w1 >> 8);
    ret[0] = avr_sendbyte(w1);
    ret[1] = avr_sendbyte(w2 >> 8);
    ret[2] = avr_sendbyte(w2);
    usb_ep_send_control_data(ret, 3, USB_MEM_SRC_RAM, wLength);
}

uint8_t avr_sendcmd(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4) {
    avr_sendbyte(b1);
    uint8_t r1 = avr_sendbyte(b2);
    uint8_t r2 = avr_sendbyte(b3);
    uint8_t r3 = avr_sendbyte(b4);
    if (b1 != r1 || b2 != r2) {
        g_avr_error |= AVR_ERRF_NOSYNC;
    }
    return r3;
}

void cmd_avr_readflash(uint16_t addr, uint16_t numbytes) {
    while (numbytes > 0) {
        uint8_t buf[2];
        buf[0] = avr_sendcmd(0x20, addr >> 8, addr, 0); // low byte of word
        buf[1] = avr_sendcmd(0x28, addr >> 8, addr, 0); // high byte of word
        if (usb_ep_send_control_data_chunk(buf, 2, USB_MEM_SRC_RAM) != USB_RC_OK) {
            return;
        }
        addr++;
        numbytes -= 2;
    }
    usb_ep_send_control_data_finish();
}

void cmd_avr_readeeprom(uint16_t addr, uint16_t numbytes) {
    while (numbytes > 0) {
        uint8_t b = avr_sendcmd(0xa0, addr >> 8, addr, 0);
        if (usb_ep_send_control_data_chunk(&b, 1, USB_MEM_SRC_RAM) != USB_RC_OK) {
            return;
        }
        addr++;
        numbytes--;
    }
    usb_ep_send_control_data_finish();
}

void avr_poll_and_return(void) {
    switch (g_avr_flags & AVRWAIT_MASK) {
        case AVRWAIT_POLL: {
            // poll
            g_avr_timer = AVR_TIMER_MS(24);
            do {
                uint8_t r = avr_sendcmd(0xf0, 0, 0, 0); // poll command
                if ((r & 0x01) == 0) {
                    return; // ready
                }
            } while (*(volatile uint8_t*)&g_avr_timer);
            g_avr_error |= AVR_ERRF_POLL_TIMEOUT;
            return;
        }
        case AVRWAIT_8MS: {
            g_avr_timer = AVR_TIMER_MS(10);
            break;
        }
        case AVRWAIT_16MS: {
            g_avr_timer = AVR_TIMER_MS(18);
            break;
        }
        case AVRWAIT_64MS:
        default: {
            g_avr_timer = AVR_TIMER_MS(66);
            break;
        }
    }
    while (*(volatile uint8_t*)&g_avr_timer) {
        sleep_cpu();
    }
}

void cmd_avr_writeflashpage(uint16_t addr, uint16_t numbytes) {
    uint16_t pageoff = 0;
    while (numbytes > 1) {
        uint8_t buf[2];
        uint16_t readb;
        if (usb_ep_recv_control_data_chunk(buf, &readb, 2) != USB_RC_OK) {
            return;
        }
        avr_sendcmd(0x40, pageoff >> 8, pageoff, buf[0]); // low byte
        avr_sendcmd(0x48, pageoff >> 8, pageoff, buf[1]); // high byte
        numbytes -= 2;
        pageoff++;
    }
    // flash it
    avr_sendcmd(0x4c, addr >> 8, addr, 0);
    avr_poll_and_return();
    usb_ep_recv_control_data_finish();
}

void cmd_avr_writeeeprompage(uint16_t addr, uint16_t numbytes) {
    uint16_t pageoff = 0;
    while (numbytes > 0) {
        uint8_t buf[1];
        uint16_t readb;
        if (usb_ep_recv_control_data_chunk(buf, &readb, 1) != USB_RC_OK) {
            return;
        }
        avr_sendcmd(0xc1, pageoff >> 8, pageoff, buf[0]);
        numbytes--;
        pageoff++;
    }
    // flash it
    avr_sendcmd(0xc2, addr >> 8, addr, 0);
    avr_poll_and_return();
    usb_ep_recv_control_data_finish();
}

/********************* 

void cmd_avr_writeeeprombyte(uint16_t addr, uint8_t val) {
    avr_sendcmd(0xc0, addr >> 8, addr, val);
    avr_poll_and_return();
    usb_ep_send_zlp();
}

void cmd_avr_loadextaddr(uint8_t extaddr) {
    avr_sendcmd(0x4d, 0, extaddr, 0);
    usb_ep_send_zlp();
}

void cmd_avr_readsig(uint16_t wLength) {
    uint8_t ret[3];
    ret[0] = avr_sendcmd(0x30, 0, 0, 0);
    ret[1] = avr_sendcmd(0x30, 0, 1, 0);
    ret[2] = avr_sendcmd(0x30, 0, 2, 0);
    usb_ep_send_control_data(ret, 3, USB_MEM_SRC_RAM, wLength);
}

void cmd_avr_readlock(uint16_t wLength) {
    uint8_t ret[1];
    ret[0] = avr_sendcmd(0x58, 0, 0, 0);
    usb_ep_send_control_data(ret, 1, USB_MEM_SRC_RAM, wLength);
}

void cmd_avr_writelock(uint8_t lock) {
    avr_sendcmd(0xac, 0xe0, 0, lock);
    avr_poll_and_return();
    usb_ep_send_zlp();    
}

void cmd_avr_readfuse(uint8_t fusenr, uint16_t wLength) {
    uint8_t ret[1];
    switch (fusenr) {
        case 1: // high
            ret[0] = avr_sendcmd(0x58, 0x08, 0, 0);
            break;
        case 2: // extra
            ret[0] = avr_sendcmd(0x50, 0x08, 0, 0);
            break;
        default:
            ret[0] = avr_sendcmd(0x50, 0, 0, 0);
            break;
    }
    usb_ep_send_control_data(ret, 1, USB_MEM_SRC_RAM, wLength);
}

void cmd_avr_writefuse(uint8_t fusenr, uint8_t val) {
    switch (fusenr) {
        case 1: // high
            avr_sendcmd(0xac, 0xa8, 0, val);
            break;
        case 2: // extra
            avr_sendcmd(0xac, 0xa4, 0, val);
            break;
        default:
            avr_sendcmd(0xac, 0xa0, 0, val);
            break;
    }
    avr_poll_and_return();
    usb_ep_send_zlp();    
}

void cmd_avr_readcal(uint8_t nr, uint16_t wLength) {
    uint8_t ret[1];
    ret[0] = avr_sendcmd(0x38, 0, nr, 0);
    usb_ep_send_control_data(ret, 1, USB_MEM_SRC_RAM, wLength);
}

void cmd_avr_erase(void) {
    avr_sendcmd(0xac, 0x80, 0, 0);
    avr_poll_and_return();
    usb_ep_send_zlp();    
}

************************/

/********
 * 
 * ATXMEGA PDI interface
 *
 * 
 */

#define XNVM_FLASH_BASE                 0x0800000 //!< Adress where the flash starts.
#define XNVM_EEPROM_BASE                0x08C0000 //!< Address where eeprom starts.
#define XNVM_FUSE_BASE                  0x08F0020 //!< Address where fuses start.
#define XNVM_DATA_BASE                  0x1000000 //!< Address where data region starts.
#define XNVM_APPL_BASE            XNVM_FLASH_BASE //!< Addres where application section starts.
#define XNVM_PROD_SIGN_BASE             0x008E0200 //!< Address where calibration row starts.
#define XNVM_USER_SIGN_BASE             0x008E0400 //!< Address where signature bytes start.

#define XNVMC_BASE 0x01C0                     //!< NVM Controller register base address.
#define XNVMC_REG_CMD    (XNVMC_BASE+0x0A)    //!< NVM Controller Command Register offset.
#define XNVMC_REG_STATUS (XNVMC_BASE+0x0F)    //!< NVM Controller Status Register offset.
#define XNVMC_REG_CTRLA  (XNVMC_BASE+0x0B)    //!< NVM Controller Control Register A offset.

#define XNVM_MCU_OFFSET  0x90
#define XNVM_MCU_DEVID0 (XNVM_MCU_OFFSET+0)

#define XNVMC_CTRLA_CMDEX_BIT  0              //!< CMDEX bit offset.
#define XPDI_STATUS_NVMEN_BIT  1              //!< NVMEN bit offset.
#define XNVMC_NVM_BUSY_BIT     7              //!< NVMBUSY bit offset.

#define XPDI_REG_STATUS  0         //!< PDI status register address.
#define XPDI_REG_RESET   1         //!< PDI reset register address.
#define XPDI_RESET_SIGNATURE   0x59         //!< PDI reset Signature.

#define NVM_PAGE_ORDER    9                       //!< NVM Page Order of 2.
#define NVM_PAGE_SIZE   (1 << NVM_PAGE_ORDER)     //!< NVM Page Size.
#define NVM_EEPROM_PAGE_SIZE 32                   //!< EEPROM Page Size.
#define NVM_LOCKBIT_ADDR  7                       //!< Lockbit address.
#define NVM_MCU_CONTROL   0x90                    //!< MCU Control base address.

#define DUMMY_BYTE 0x55                           //!< Dummy byte for Dummy writing.

#define XPDI_CMD_LDS    0x00 //!< LDS instruction.
#define XPDI_CMD_STS    0x40 //!< STS instruction.
#define XPDI_CMD_LD     0x20 //!< LD instruction.
#define XPDI_CMD_ST     0x60 //!< ST instruction.
#define XPDI_CMD_LDCS   0x80 //!< LDCS instruction.
#define XPDI_CMD_STCS   0xC0 //!< STCS instruction.
#define XPDI_CMD_REPEAT 0xA0 //!< REPEAT instruction.
#define XPDI_CMD_KEY    0xE0 //!< KEY instruction.

#define XPDI_SIZEA_1    0x00
#define XPDI_SIZEA_2    0x04
#define XPDI_SIZEA_3    0x08
#define XPDI_SIZEA_4    0x0c
#define XPDI_SIZEB_1    0x00
#define XPDI_SIZEB_2    0x01
#define XPDI_SIZEB_3    0x02
#define XPDI_SIZEB_4    0x03
#define XPDI_PTR_STAR   0x00
#define XPDI_PTR_STARINC 0x04
#define XPDI_PTR_IMM    0x08

#define XNVM_CMD_NOP                         0x00 //!< No Operation.
#define XNVM_CMD_CHIP_ERASE                  0x40 //!< Chip Erase.
#define XNVM_CMD_READ_NVM_PDI                0x43 //!< Read NVM PDI.
#define XNVM_CMD_LOAD_FLASH_PAGE_BUFFER      0x23 //!< Load Flash Page Buffer.
#define XNVM_CMD_ERASE_FLASH_PAGE_BUFFER     0x26 //!< Erase Flash Page Buffer.
#define XNVM_CMD_ERASE_FLASH_PAGE            0x2B //!< Erase Flash Page.
#define XNVM_CMD_WRITE_FLASH_PAGE            0x2E //!< Flash Page Write.
#define XNVM_CMD_ERASE_AND_WRITE_FLASH_PAGE  0x2F //!< Erase & Write Flash Page.
#define XNVM_CMD_CALC_CRC_ON_FLASH           0x78 //!< Flash CRC.

#define XNVM_CMD_ERASE_APP_SECTION           0x20 //!< Erase Application Section.
#define XNVM_CMD_ERASE_APP_PAGE              0x22 //!< Erase Application Section.
#define XNVM_CMD_WRITE_APP_SECTION           0x24 //!< Write Application Section.
#define XNVM_CMD_ERASE_AND_WRITE_APP_SECTION 0x25 //!< Erase & Write Application Section Page.
#define XNVM_CMD_CALC_CRC_APP_SECTION        0x38 //!< Application Section CRC.

#define XNVM_CMD_ERASE_BOOT_SECTION          0x68 //!< Erase Boot Section.
#define XNVM_CMD_ERASE_BOOT_PAGE             0x2A //!< Erase Boot Loader Section Page.
#define XNVM_CMD_WRITE_BOOT_PAGE             0x2C //!< Write Boot Loader Section Page.
#define XNVM_CMD_ERASE_AND_WRITE_BOOT_PAGE   0x2D //!< Erase & Write Boot Loader Section Page.
#define XNVM_CMD_CALC_CRC_BOOT_SECTION       0x39 //!< Boot Loader Section CRC.

#define XNVM_CMD_READ_USER_SIGN              0x03 //!< Read User Signature Row.
#define XNVM_CMD_ERASE_USER_SIGN             0x18 //!< Erase User Signature Row.
#define XNVM_CMD_WRITE_USER_SIGN             0x1A //!< Write User Signature Row.
#define XNVM_CMD_READ_CALIB_ROW              0x02 //!< Read Calibration Row.

#define XNVM_CMD_READ_FUSE                   0x07 //!< Read Fuse.
#define XNVM_CMD_WRITE_FUSE                  0x4C //!< Write Fuse.
#define XNVM_CMD_WRITE_LOCK_BITS             0x08 //!< Write Lock Bits.

#define XNVM_CMD_LOAD_EEPROM_PAGE_BUFFER     0x33 //!< Load EEPROM Page Buffer.
#define XNVM_CMD_ERASE_EEPROM_PAGE_BUFFER    0x36 //!< Erase EEPROM Page Buffer.

#define XNVM_CMD_ERASE_EEPROM                0x30 //!< Erase EEPROM.
#define XNVM_CMD_ERASE_EEPROM_PAGE           0x32 //!< Erase EEPROM Page.
#define XNVM_CMD_WRITE_EEPROM_PAGE           0x34 //!< Write EEPROM Page.
#define XNVM_CMD_ERASE_AND_WRITE_EEPROM      0x35 //!< Erase & Write EEPROM Page.
#define XNVM_CMD_READ_EEPROM                 0x06 //!< Read EEPROM.

#define NVM_KEY_BYTE0 0xFF
#define NVM_KEY_BYTE1 0x88
#define NVM_KEY_BYTE2 0xD8
#define NVM_KEY_BYTE3 0xCD
#define NVM_KEY_BYTE4 0x45
#define NVM_KEY_BYTE5 0xAB
#define NVM_KEY_BYTE6 0x89
#define NVM_KEY_BYTE7 0x12


enum EraseMode {
    EraseMode_CHIP = 0,
    EraseMode_APP_SECTION = 1,
    EraseMode_BOOT_SECTION = 2,
    EraseMode_EEPROM = 3,
};


void pdi_off(void) {
    UCSR1A = 0;
    UCSR1B = 0; // TXEN, RXEN off
    UCSR1C = 0;
    UCSR1D = 0;
    clrbit(DDRD, PD2); // RxD Input
    clrbit(PORTD, PD2); // RxD pull up off
    setbit(DDRD, PD3); // TxD/PDI_DATA output
    clrbit(PORTD, PD3); // TxD/PDI_DATA low
    setbit(DDRD, PD5); // XCK output
    setbit(PORTD, PD5); // XCK high
}

void pdi_enter(void) {
    pdi_off();
}

void pdi_leave(void) {
    UCSR1A = 0;
    UCSR1B = 0;
    UCSR1C = 0;
    UCSR1D = 0;
    clrbit(DDRD, PD5); // all pins: input, pullup on
    setbit(PORTD, PD5);
    clrbit(DDRD, PD2);
    setbit(PORTD, PD2);
    clrbit(DDRD, PD3);
    setbit(PORTD, PD3);
}

void pdi_start(void) {
    pdi_off();
    
    setbit(PORTD, PD3); // TxD/PDI_DATA high

    _delay_us(50);
    
    // enable synchronous USART -> outputs clock at XCK
    UBRR1 = 39; // 100kBaud @ 8Mhz
    UCSR1A = 0;
    UCSR1B = Bsv(RXCIE1,0)|Bsv(TXCIE1,0)|Bsv(UDRIE1,0)|Bsv(RXEN1,0)
            |Bsv(TXEN1,0)|Bsv(UCSZ12,0);
    UCSR1C = Bsv(UMSEL11,0)|Bsv(UMSEL10,1)|Bsv(UPM11,1)|Bsv(UPM10,0) // Sync USART, Even Parity
            |Bsv(USBS1, 1)|Bsv(UCSZ11,1)|Bsv(UCSZ10,1)|Bsv(UCPOL1,1);  // 2 stop bits, 8 Data Bits, Sample on Rising Edge
    UCSR1D = 0;
    
    setbit(UCSR1B, RXEN1); // also enables clock on XCK output
    
    _delay_us(200);
}


uint8_t pdi_write(uint8_t* buf, uint16_t count0) {
    uint8_t x;
    uint8_t rc = 0;
        
    // empty receiver
    while (bit_is_set(UCSR1A, RXC1)) {
        x = UDR1;
    }
    
    setbit(UCSR1B, TXEN1);
    
    while (1) {
        
        while (bit_is_clear(UCSR1A, UDRE1));
        setbit(UCSR1A, TXC1); // clear TXC1 !
        
        uint8_t b;
        if (buf) {
            b = *buf++;
        } else {
            uint16_t readb;
            if (usb_ep_recv_control_data_chunk(&b, &readb, 1) != USB_RC_OK) {
                rc = 21;
                break;
            }
            if (readb != 1) {
                rc = 21;
                break;
            }
        }
        UDR1 = b;
        
        while (bit_is_clear(UCSR1A, TXC1));
        setbit(UCSR1A, TXC1); // clear TXC1 !
        
        g_pdi_timer = AVR_TIMER_MS(10);
        while (bit_is_clear(UCSR1A, RXC1)) {
            if (g_pdi_timer == 0) {
                rc = 10;
                break;
            }
        }
        if (rc == 0) {
            uint8_t flags = UCSR1A;
            x = UDR1;
            if (x != b || (flags & (Bit(FE1)|Bit(DOR1)|Bit(UPE1)))) {
                rc = 4;
            }
        }
        if (count0 == 0 || rc) {
            break;
        }
        count0--;
    }
    // disable transmitter
    clrbit(UCSR1B, TXEN1);
    clrbit(DDRD, PD3); // TxD input high impedance
    clrbit(PORTD, PD3);
    
    return rc;
}

uint8_t pdi_read(uint8_t* data) {
    g_pdi_timer = AVR_TIMER_MS(10);
    while (bit_is_clear(UCSR1A, RXC1)) {
        if (g_pdi_timer == 0) {
            return 11;
        }
    }
    
    uint8_t flags = UCSR1A;
    uint8_t b = UDR1;
    
    if (flags & (Bit(FE1)|Bit(DOR1)|Bit(UPE1))) {
        return 5;
    }
    *data = b;
    return 0;
}

uint8_t pdi_cmd(uint8_t cmdb[], uint8_t len, uint8_t pout[], uint8_t* plen) {
    if (len == 0)
        return 1;
    uint8_t cmd = cmdb[0];
    uint8_t rc = 0;
    if (plen)
        *plen = 0;
    switch (cmd & 0xe0) {
        case XPDI_CMD_LDS: { // LDS
            uint8_t sza = ((cmd & 0x0c) >> 2) + 1;
            uint8_t szb = (cmd & 0x03) + 1;
            if (sza + 1 != len)
                return 3;
            rc = pdi_write(cmdb, len-1);
            if (rc) {
                return rc;
            }
            for (uint8_t i = 0; i < szb; i++) {
                rc = pdi_read(pout + i);
                if (rc) {
                    return rc;
                }
            }
            if (plen)
                *plen = szb;
            break;
        }
        case XPDI_CMD_STS: { // STS
            uint8_t sza = ((cmd & 0x0c) >> 2) + 1;
            uint8_t szb = (cmd & 0x03) + 1;
            if (sza + szb + 1 != len)
                return 3;
            rc = pdi_write(cmdb, len-1);
            if (rc) {
                return rc;
            }
            break;
        }
        case XPDI_CMD_LD: { // LD
            uint8_t szb = (cmd & 0x03) + 1;
            if (1 != len)
                return 3;
            rc = pdi_write(cmdb, len-1);
            if (rc) {
                return rc;
            }
            for (uint8_t i = 0; i < szb; i++) {
                rc = pdi_read(pout + i);
                if (rc) {
                    return rc;
                }
            }
            if (plen)
                *plen = szb;
            break;
        }    
        case XPDI_CMD_ST: { // ST
            uint8_t szb = (cmd & 0x03) + 1;
            if (szb + 1 != len)
                return 3;
            rc = pdi_write(cmdb, len-1);
            if (rc) {
                return rc;
            }
            break;
        }
        case XPDI_CMD_LDCS: { // LDCS
            if (1 != len)
                return 3;
            rc = pdi_write(cmdb, len-1);
            if (rc) {
                return rc;
            }
            rc = pdi_read(pout);
            if (rc) {
                return rc;
            }
            *plen = 1;
            break;
        }
        case XPDI_CMD_STCS: { // STCS
            if (2 != len) {
                return 3;
            }
            rc = pdi_write(cmdb, len-1);
            if (rc) {
                return rc;
            }
            break;
        }
        case XPDI_CMD_REPEAT: { // REPEAT
            uint8_t szb = (cmd & 0x03) + 1;
            if (szb + 1 != len)
                return 3;
            rc = pdi_write(cmdb, len-1);
            if (rc) {
                return rc;
            }
            break;
        }
        case XPDI_CMD_KEY: { // KEY
            if (9 != len)
                return 3;
            rc = pdi_write(cmdb, 9-1);
            if (rc) {
                return rc;
            }
            break;
        }
        default: return 2;
    }
    return 0;
}

uint8_t pdi_cmd_lds_byte(uint32_t addr, uint8_t* b) {
    uint8_t buf[6];
    buf[0] = XPDI_CMD_LDS | XPDI_SIZEA_4 | XPDI_SIZEB_1;
    buf[1] = addr & 0xff;
    buf[2] = addr >> 8;
    buf[3] = addr >> 16;
    buf[4] = addr >> 24;
    return pdi_cmd(buf, 5, b, NULL);
}

uint8_t pdi_cmd_sts_byte(uint32_t addr, uint8_t b) {
    uint8_t buf[6];
    buf[0] = XPDI_CMD_STS | XPDI_SIZEA_4 | XPDI_SIZEB_1;
    buf[1] = addr & 0xff;
    buf[2] = addr >> 8;
    buf[3] = addr >> 16;
    buf[4] = addr >> 24;
    buf[5] = b;
    return pdi_cmd(buf, 6, NULL, NULL);
}

uint8_t pdi_cmd_st_ptr(uint32_t addr) {
    uint8_t buf[5];
    buf[0] = XPDI_CMD_ST | XPDI_PTR_IMM | XPDI_SIZEB_4;
    buf[1] = addr & 0xff;
    buf[2] = addr >> 8;
    buf[3] = addr >> 16;
    buf[4] = addr >> 24;
    return pdi_cmd(buf, 5, NULL, NULL);   
}

uint8_t pdi_cmd_stcs(uint8_t addr, uint8_t b) {
    uint8_t buf[2];
    buf[0] = XPDI_CMD_STCS | (addr & 0x0f);
    buf[1] = b;
    return pdi_cmd(buf, 2, NULL, NULL);   
}

uint8_t pdi_cmd_ldcs(uint8_t addr, uint8_t* b) {
    uint8_t buf[2];
    buf[0] = XPDI_CMD_LDCS | (addr & 0x0f);
    return pdi_cmd(buf, 1, b, NULL);
}

uint8_t pdi_cmd_key_nvm(void) {
    uint8_t buf[9];
    buf[0] = XPDI_CMD_KEY;
    buf[1] = NVM_KEY_BYTE0;
    buf[2] = NVM_KEY_BYTE1;
    buf[3] = NVM_KEY_BYTE2;
    buf[4] = NVM_KEY_BYTE3;
    buf[5] = NVM_KEY_BYTE4;
    buf[6] = NVM_KEY_BYTE5;
    buf[7] = NVM_KEY_BYTE6;
    buf[8] = NVM_KEY_BYTE7;
    return pdi_cmd(buf, 9, NULL, NULL);
}

uint8_t pdi_cmd_repeat(uint16_t count) {
    uint8_t buf[3];
    buf[0] = XPDI_CMD_REPEAT | XPDI_SIZEB_2;
    buf[1] = count & 0xff;
    buf[2] = count >> 8;
    return pdi_cmd(buf, 3, NULL, NULL);
}

uint8_t pdi_cmd_load(uint32_t addr, uint8_t* pdata, uint16_t count0) {
    uint8_t rc;
    uint8_t buf[2];
    
    rc = pdi_cmd_st_ptr(addr);
    if (rc)
        return rc;
    
    rc = pdi_cmd_repeat(count0);
    if (rc)
        return rc;
    
    buf[0] = XPDI_CMD_LD | XPDI_PTR_STARINC | XPDI_SIZEB_1;
    rc = pdi_write(buf, 0);
    if (rc)
        return rc;
    
    while (1) {
        if (pdata) {
            rc = pdi_read(pdata);
            pdata++;
        } else {
            rc = pdi_read(buf);
            if (!rc) {
                if (usb_ep_send_control_data_chunk(buf, 1, USB_MEM_SRC_RAM) != USB_RC_OK)
                    rc = 20;
            }
        }
        if (rc)
            return rc;
        if (count0 == 0)
            break;
        count0--;
    }
    return 0;
}

uint8_t pdi_cmd_store(uint32_t addr, uint8_t* pdata, uint16_t count0) {
    uint8_t rc;
    uint8_t buf[2];
    
    rc = pdi_cmd_st_ptr(addr);
    if (rc)
        return rc;
    
    rc = pdi_cmd_repeat(count0);
    if (rc)
        return rc;
    
    buf[0] = XPDI_CMD_ST | XPDI_PTR_STARINC | XPDI_SIZEB_1;
    rc = pdi_write(buf, 0);
    if (rc)
        return rc;
    
    return pdi_write(pdata, count0);
}

uint8_t nvmc_init(void) {
    uint8_t rc;
    uint8_t b;
    
    rc = pdi_cmd_stcs(XPDI_REG_RESET, XPDI_RESET_SIGNATURE);
    if (rc)
        goto error;
    
    _delay_us(10);
    rc = pdi_cmd_ldcs(XPDI_REG_RESET, &b);
    if (rc)
        goto error;
    if (bit_is_clear(b, 0)) {
        rc = 99;
        goto error;
    }
        
    rc = pdi_cmd_key_nvm();
    if (rc)
        goto error;

    _delay_ms(1);
    
    g_nvmc_timer = AVR_TIMER_MS(200);
    while (1) {
        rc = pdi_cmd_ldcs(XPDI_REG_STATUS, &b);
        if (rc)
            goto error;
        if (bit_is_set(b, XPDI_STATUS_NVMEN_BIT)) {
            break;
        }
        if (g_nvmc_timer == 0) {
            rc = 12;
            goto error;
        }
    }
    
    return 0;

error:
    pdi_off();
    return rc;
}

uint8_t nvmc_exit(void) {
    uint8_t rc;
    
    pdi_cmd_stcs(XPDI_REG_STATUS, 0); // unset NVMEM bit, disabled NVM programming interface
    rc = pdi_cmd_stcs(XPDI_REG_RESET, 0);  // pull device out of reset
    
    return rc;
}

uint8_t nvmc_set_cmd(uint8_t cmd) {
    return pdi_cmd_sts_byte(XNVM_DATA_BASE + XNVMC_REG_CMD, cmd);
}
uint8_t nvmc_trigger_cmdex(void) {
    return pdi_cmd_sts_byte(XNVM_DATA_BASE + XNVMC_REG_CTRLA, Bit(XNVMC_CTRLA_CMDEX_BIT));
}
uint8_t nvmc_nvmbusy_wait(void) {
    uint8_t rc;
    uint8_t status;
    g_nvmc_timer = AVR_TIMER_MS(20);
    while (1) {
        _delay_us(100);
        rc = pdi_cmd_lds_byte(XNVM_DATA_BASE + XNVMC_REG_STATUS, &status);
        if (rc)
            return rc;
        if (bit_is_clear(status, XNVMC_NVM_BUSY_BIT))
            break;
        if (g_nvmc_timer == 0)
            return 14;
    }
    return 0;
}



uint8_t nvmc_read_nvm(uint32_t addr, uint16_t count0) {
    uint8_t rc = nvmc_set_cmd(XNVM_CMD_READ_NVM_PDI);
    if (rc == 0) {
        rc = pdi_cmd_load(addr, NULL, count0);
    }
    return rc;
}

uint8_t nvmc_erase_chip(uint8_t mode) {
    uint8_t rc;
    uint8_t cmd;
    switch (mode) {
        case EraseMode_APP_SECTION:
            cmd = XNVM_CMD_CHIP_ERASE;
            break;
        case EraseMode_BOOT_SECTION:
            cmd = XNVM_CMD_ERASE_BOOT_SECTION;
            break;
        case EraseMode_EEPROM:
            cmd = XNVM_CMD_ERASE_EEPROM;
            break;
        case EraseMode_CHIP:
            cmd = XNVM_CMD_CHIP_ERASE;
            break;
        default:
            return 15;
    }
    rc = nvmc_set_cmd(cmd);
    if (rc)
        return rc;
    rc = nvmc_trigger_cmdex();
    if (rc)
        return rc;
    if (mode == EraseMode_CHIP) {
        g_nvmc_timer = AVR_TIMER_MS(100);
        while (1) {
            uint8_t b;
            _delay_us(500);
            rc = pdi_cmd_ldcs(XPDI_REG_STATUS, &b);
            if (rc)
                return rc;
            if (bit_is_set(b, XPDI_STATUS_NVMEN_BIT)) {
                break;
            }
            if (g_nvmc_timer == 0) {
                return 13;
            }
        }
    } else {
        return nvmc_nvmbusy_wait();
    }
    return 0;
}

#define CHECKRC(_x) rc = _x; if (rc) { return rc; }

uint8_t nvmc_flash_write_page(uint32_t addr, uint16_t count0) {
    uint8_t rc = 0;
    
    addr = XNVM_FLASH_BASE + addr*2;
    
    // erase flash page buffer
    CHECKRC(nvmc_set_cmd(XNVM_CMD_ERASE_FLASH_PAGE_BUFFER));
    CHECKRC(nvmc_trigger_cmdex());
    CHECKRC(nvmc_nvmbusy_wait());

    // load flash page buffer, count0+1 must be the page Size
    CHECKRC(nvmc_set_cmd(XNVM_CMD_LOAD_FLASH_PAGE_BUFFER));
    CHECKRC(pdi_cmd_store(addr, NULL, count0));
    
    // erase and program flash page
    CHECKRC(nvmc_set_cmd(XNVM_CMD_ERASE_AND_WRITE_FLASH_PAGE));
    CHECKRC(pdi_cmd_sts_byte(addr, DUMMY_BYTE));
    CHECKRC(nvmc_nvmbusy_wait());
    
    return rc;
}

uint8_t nvmc_flash_read(uint32_t addr, uint16_t count0) {
    return nvmc_read_nvm(addr*2 + XNVM_FLASH_BASE, count0);
}

uint8_t nvmc_eeprom_write(uint32_t addr, uint16_t count0) {
    uint8_t rc = 0;
    
    addr = XNVM_EEPROM_BASE + addr;
    
    // erase eeprom page Buffer
    CHECKRC(nvmc_set_cmd(XNVM_CMD_ERASE_EEPROM_PAGE_BUFFER));
    CHECKRC(nvmc_trigger_cmdex());
    CHECKRC(nvmc_nvmbusy_wait());
    
    // load eeprom page buffer
    CHECKRC(nvmc_set_cmd(XNVM_CMD_LOAD_EEPROM_PAGE_BUFFER));
    CHECKRC(pdi_cmd_store(addr, NULL, count0));
    
    // erase and program eeprom Page
    CHECKRC(nvmc_set_cmd(XNVM_CMD_ERASE_AND_WRITE_EEPROM));
    CHECKRC(pdi_cmd_sts_byte(addr, DUMMY_BYTE));
    CHECKRC(nvmc_nvmbusy_wait());
    
    return rc;
}

uint8_t nvmc_eeprom_read(uint32_t addr, uint16_t count0) {
    return nvmc_read_nvm(addr + XNVM_EEPROM_BASE, count0);
}

uint8_t nvmc_fuse_write(uint16_t addr, uint8_t b) {
    uint8_t rc = 0;
    
    CHECKRC(nvmc_set_cmd(XNVM_CMD_WRITE_FUSE));
    CHECKRC(pdi_cmd_sts_byte(XNVM_FUSE_BASE + addr, b));
    CHECKRC(nvmc_nvmbusy_wait());
    
    return rc;
}

uint8_t nvmc_fuse_read(uint16_t addr) {
    return nvmc_read_nvm(addr + XNVM_FUSE_BASE, 0);
}

uint8_t nvmc_signaturerow_read(uint16_t addr, uint16_t count0) {
    return nvmc_read_nvm(addr + XNVM_PROD_SIGN_BASE, count0);
}    

uint8_t nvmc_read_dev_sign(uint16_t count0) {
    return pdi_cmd_load(XNVM_DATA_BASE + XNVM_MCU_DEVID0, NULL, count0); // devid0, devid1, devid2, revid
}

void pdi_exec(struct usb_device_request* req) {
    uint8_t buf[12];
    uint8_t bout[6];
    uint8_t outlen;
    uint16_t wValue = req->wValue;
    uint16_t wIndex = req->wIndex;
    uint16_t wLength = req->wLength;

    g_pdi_last_rc = 1;
    
    switch(req->bRequest) {
        case CMD_PDI_LDS: {
            uint8_t len = minvalof(wLength,4);
            len = maxvalof(len, 1);
            buf[0] = 0x0c | 0x0c | ((len-1) & 0x03);
            buf[1] = wIndex & 0xff;
            buf[2] = (wIndex >> 8) & 0xff;
            buf[3] = wValue & 0xff;
            buf[4] = (wValue >> 8) & 0xff;
            g_pdi_last_rc = pdi_cmd(buf, 5, bout, &outlen);
            usb_ep_send_control_data(bout, outlen, USB_MEM_SRC_RAM, len);
            break;
        }
        case CMD_PDI_STS: {
            uint16_t recvb = 0;
            uint8_t len = minvalof(wLength,4);
            len = maxvalof(len, 1);
            buf[0] = 0x40 | 0x0c | ((len-1) & 0x03);
            buf[1] = wIndex & 0xff;
            buf[2] = (wIndex >> 8) & 0xff;
            buf[3] = wValue & 0xff;
            buf[4] = (wValue >> 8) & 0xff;
            usb_ep_recv_control_data(buf+5, &recvb, len);
            g_pdi_last_rc = pdi_cmd(buf, 5+recvb, bout, &outlen);
            break;
        }
        case CMD_PDI_LD: {
            uint8_t len = minvalof(wLength,4);
            len = maxvalof(len, 1);
            buf[0] = 0x20 | (wValue & 0x0c) | ((len-1) & 0x03);
            pdi_cmd(buf, 1, bout, &outlen);
            usb_ep_send_control_data(bout, outlen, USB_MEM_SRC_RAM, len);
            break;
        }
        case CMD_PDI_ST: {
            uint16_t recvb = 0;
            uint8_t len = minvalof(wLength,4);
            len = maxvalof(len, 1);
            buf[0] = 0x60 | (wValue & 0x0c) | ((len-1) & 0x03);
            usb_ep_recv_control_data(buf+1, &recvb, len);
            g_pdi_last_rc = pdi_cmd(buf, 1+recvb, bout, &outlen);
            break;
        }
        case CMD_PDI_LDCS: {
            buf[0] = 0x80 | (wValue & 0x0f);
            g_pdi_last_rc = pdi_cmd(buf, 1, bout, &outlen);
            usb_ep_send_control_data(bout, outlen, USB_MEM_SRC_RAM, wLength);
            break;
        }
        case CMD_PDI_STCS: {
            buf[0] = 0xc0 | (wValue & 0x0f);
            buf[1] = wIndex & 0x0ff;
            g_pdi_last_rc = pdi_cmd(buf, 2, bout, &outlen);
            usb_ep_send_zlp();
            break;
        }
        case CMD_PDI_REPEAT: {
            uint16_t recvb = 0;
            uint8_t len = minvalof(wLength,4);
            len = maxvalof(len, 1);
            buf[0] = 0xa0 | ((len-1) & 0x03);
            usb_ep_recv_control_data(buf+1, &recvb, len);
            g_pdi_last_rc = pdi_cmd(buf, 1+recvb, bout, &outlen);
            break;
        }
        case CMD_PDI_KEY: {
            uint16_t recvb = 0;
            buf[0] = 0xe0;
            usb_ep_recv_control_data(buf+1, &recvb, minvalof(wLength,8));
            g_pdi_last_rc = pdi_cmd(buf, 1+recvb, bout, &outlen);
            break;
        }
        
        case CMD_NVMC_NVM_READ: {
            if (wLength)
                g_pdi_last_rc = nvmc_read_nvm(((uint32_t)wValue << 16)|wIndex, wLength-1); 
            usb_ep_send_control_data_finish();
            break;
        }
        case CMD_NVMC_FLASH_READ: {
            if (wLength)
                g_pdi_last_rc = nvmc_flash_read(((uint32_t)wValue << 16)|wIndex, wLength-1); 
            usb_ep_send_control_data_finish();
            break;
        }
        case CMD_NVMC_EEPROM_READ: {
            if (wLength)
                g_pdi_last_rc = nvmc_eeprom_read(((uint32_t)wValue << 16)|wIndex, wLength-1); 
            usb_ep_send_control_data_finish();
            break;
        }
        case CMD_NVMC_FUSE_READ: {
            if (wLength)
                g_pdi_last_rc = nvmc_fuse_read(wValue);
            usb_ep_send_control_data_finish();
            break;
        }
        case CMD_NVMC_SIGNROW_READ: {
            if (wLength)
                g_pdi_last_rc = nvmc_signaturerow_read(wValue, wLength-1);
            usb_ep_send_control_data_finish();
            break;
        }
        case CMD_NVMC_FLASH_WRITE_PAGE: {
            if (wLength) {
                g_pdi_last_rc = nvmc_flash_write_page(((uint32_t)wValue << 16)|wIndex, wLength-1);
            }
            usb_ep_recv_control_data_finish();
            break;
        }
        case CMD_NVMC_EEPROM_WRITE: {
            if (wLength) {
                g_pdi_last_rc = nvmc_eeprom_write(((uint32_t)wValue << 16)|wIndex, wLength-1);
            }
            usb_ep_recv_control_data_finish();
            break;
        }
        case CMD_NVMC_FUSE_WRITE: {
            g_pdi_last_rc = nvmc_fuse_write(wValue, wIndex);
            usb_ep_send_zlp();
            break;
        }
        case CMD_NVMC_GET_DEVID: {
            if (wLength) {
                g_pdi_last_rc = nvmc_read_dev_sign(minvalof(wLength-1, 3));
            }
            usb_ep_send_control_data_finish();
            break;
        }
        case CMD_NVMC_ERASE_CHIP: {
            g_pdi_last_rc = nvmc_erase_chip(wValue);
            usb_ep_send_zlp();
            break;
        }
    }
}

/***************************************/



//#define VENDOR_ID  0xf123
#define VENDOR_ID 0x03eb
//#define PRODUCT_ID 0xa001
#define PRODUCT_ID 0x2FFA
#define BCD_DEVICE 0x0100

const struct usb_device_desc g_device_desc_P PROGMEM = {
    .bLength            = sizeof(struct usb_device_desc),
    .bDescriptorType    = USB_DESC_DEVICE,
    .bcdUSB             = USB_SPEC_1_1,
    .bDeviceClass       = 0xff,
    .bDeviceSubClass    = 0xff,
    .bDeviceProtocol    = 0xff,
    .bMaxPacketSize0    = 64,
    .idVendor           = VENDOR_ID,
    .idProduct          = PRODUCT_ID,
    .bcdDevice          = BCD_DEVICE,
    .iManufacturer      = 1,
    .iProduct           = 2,
    .iSerialNumber      = 3,
    .bNumConfigurations = 1
};
const struct config_1 {
    struct usb_configuration_desc config;
    struct usb_interface_desc     interface;
} g_config_1_P PROGMEM = {
    .config = {
        .bLength            = sizeof(struct usb_configuration_desc),
        .bDescriptorType    = USB_DESC_CONFIGURATION,
        .wTotalLength       = sizeof(struct config_1),
        .bNumInterfaces     = 1,
        .bConfigurationValue = 1,
        .iConfiguration     = 0,
        .bmAttributes       = USB_CONFIG_ATTRIBUTES(0),
        .bMaxPower          = 25
    },
    .interface = {
        .bLength            = sizeof(struct usb_interface_desc),
        .bDescriptorType    = USB_DESC_INTERFACE,
        .bInterfaceNumber   = 0,
        .bAlternateSetting  = 0,
        .bNumEndpoints      = 0,
        .bInterfaceClass    = 0xff,
        .bInterfaceSubClass = 0xff,
        .bInterfaceProtocoll = 0xff,
        .iInterface         = 0
    }
};


USB_DEF_STR_CHARS_P(g_string0_P, {0x0409}, 2);
USB_DEF_STR_P(g_string1_P, L"Thomas Maier <balagi@justmail.de>");
USB_DEF_STR_P(g_string2_P, L"gpio-usb");
USB_DEF_STR_P(g_string3_P, L"V 1.2");

#define NUM_USB_STRINGS 4
PGM_VOID_P const g_strings_P[NUM_USB_STRINGS] PROGMEM = {
    &g_string0_P,
    &g_string1_P,
    &g_string2_P,
    &g_string3_P
};


void usb_event_set_configuration(void) {
}


uint8_t usb_event_ep0_request(struct usb_device_request* req) {
    if ( (req->bmRequestType & 0x7f) != (USB_REQTYPE_TYP_VENDOR|USB_REQTYPE_REC_INTERFACE) )
        return 1;
    
    if (usb_req_isOUT(req)) {
        switch(req->bRequest) {
            case CMD_ENTER_AVRMODE: {
                usb_ep_clearSETUP();
                cmd_avr_init();
                return 0;
            }
            case CMD_LEAVE_AVRMODE: {
                usb_ep_clearSETUP();
                cmd_avr_exit();
                return 0;
            }
            case CMD_AVR_WRITE_PRGMEM_PAGE: {
                usb_ep_clearSETUP();
                cmd_avr_writeflashpage(req->wValue, req->wLength);
                return 0;
            }
            case CMD_AVR_WRITE_EEPROM_PAGE: {
                usb_ep_clearSETUP();
                cmd_avr_writeeeprompage(req->wValue, req->wLength);
                return 0;
            }
            case CMD_ENTER_PDI: {
                usb_ep_clearSETUP();
                if (1) {
                    pdi_enter();
                    setbit(g_pdi_flags, PDI_FLAG_PDI_MODE);
                    g_pdi_last_rc = 0;
                } else {
                    g_pdi_last_rc = 200;
                }
                usb_ep_send_zlp();
                return 0;
            }
            case CMD_LEAVE_PDI: {
                usb_ep_clearSETUP();
                g_pdi_flags = 0;
                pdi_leave();
                usb_ep_send_zlp();
                return 0;
            }
            case CMD_PDI_ENABLE: {
                usb_ep_clearSETUP();
                if (bit_is_set(g_pdi_flags, PDI_FLAG_PDI_MODE)) {
                    if (bit_is_clear(g_pdi_flags, PDI_FLAG_PDI_ENABLED)) {
                        pdi_start();
                        setbit(g_pdi_flags, PDI_FLAG_PDI_ENABLED);
                    }
                    g_pdi_last_rc = 0;
                } else {
                    g_pdi_last_rc = 100;
                }
                usb_ep_send_zlp();
                return 0;
            }
            case CMD_NVMC_ENABLE: {
                usb_ep_clearSETUP();
                if (bit_is_set(g_pdi_flags, PDI_FLAG_PDI_MODE)) {
                    g_pdi_last_rc = 0;
                    if (bit_is_clear(g_pdi_flags, PDI_FLAG_NVMC_ENABLED)) {
                        if (bit_is_clear(g_pdi_flags, PDI_FLAG_PDI_ENABLED)) {
                            pdi_start();
                            setbit(g_pdi_flags, PDI_FLAG_PDI_ENABLED);
                        }
                        g_pdi_last_rc = nvmc_init();
                        if (g_pdi_last_rc == 0)
                            setbit(g_pdi_flags, PDI_FLAG_NVMC_ENABLED);
                    }
                } else {
                    g_pdi_last_rc = 100;
                }
                usb_ep_send_zlp();
                return 0;
            }
            case CMD_NVMC_DISABLE:
            case CMD_PDI_DISABLE: {
                usb_ep_clearSETUP();
                if (bit_is_set(g_pdi_flags, PDI_FLAG_PDI_MODE)) {
                    nvmc_exit();
                    pdi_off();
                    clrbit(g_pdi_flags, PDI_FLAG_NVMC_ENABLED);
                    clrbit(g_pdi_flags, PDI_FLAG_PDI_ENABLED);
                    g_pdi_last_rc = 0;
                } else {
                    g_pdi_last_rc = 100;
                }
                usb_ep_send_zlp();
                return 0;
            }
            case CMD_PDI_STS:
            case CMD_PDI_ST:
            case CMD_PDI_STCS:
            case CMD_PDI_REPEAT:
            case CMD_PDI_KEY:
            case CMD_NVMC_FLASH_WRITE_PAGE:
            case CMD_NVMC_EEPROM_WRITE:
            case CMD_NVMC_FUSE_WRITE:
            case CMD_NVMC_ERASE_CHIP:
            {
                usb_ep_clearSETUP();
                if (bit_is_set(g_pdi_flags, PDI_FLAG_PDI_MODE)) {
                    if (bit_is_set(g_pdi_flags, PDI_FLAG_PDI_ENABLED)) {
                        pdi_exec(req);
                    } else {
                        g_pdi_last_rc = 101;
                        usb_ep_send_zlp();
                    }
                } else {
                    g_pdi_last_rc = 100;
                    usb_ep_send_zlp();
                }
                return 0;
            }
        }
    } else { // isIN request
        switch(req->bRequest) {
            case CMD_AVR_PROGMODE: {
                usb_ep_clearSETUP();
                cmd_avr_progmode(req->wValue >> 8, req->wValue, req->wLength);
                return 0;
            }
            case CMD_AVR_RAWCMD: {
                usb_ep_clearSETUP();
                cmd_avr_rawcmd(req->wValue, req->wIndex, req->wLength);
                return 0;
            }
            case CMD_AVR_READ_PRGMEM: {
                usb_ep_clearSETUP();
                cmd_avr_readflash(req->wValue, req->wLength);
                return 0;
            }
           case CMD_AVR_READ_EEPROM: {
                usb_ep_clearSETUP();
                cmd_avr_readeeprom(req->wValue, req->wLength);
                return 0;
            }
            case CMD_AVR_GET_STATUS: {
                usb_ep_clearSETUP();
                uint8_t ret[2];
                ret[0] = g_avr_flags;
                ret[1] = g_avr_error;
                g_avr_error = 0;
                usb_ep_send_control_data(ret, 2, USB_MEM_SRC_RAM, req->wLength);
                return 0;
            }
            case CMD_PDI_GETSTATUS: {
                usb_ep_clearSETUP();
                uint8_t ret[1];
                ret[0] = g_pdi_last_rc;
                g_pdi_last_rc = 0;
                usb_ep_send_control_data(ret, 1, USB_MEM_SRC_RAM, req->wLength);
                return 0;
            }
            case CMD_PDI_LDS:
            case CMD_PDI_LD:
            case CMD_PDI_LDCS:
            case CMD_NVMC_FLASH_READ:
            case CMD_NVMC_EEPROM_READ:
            case CMD_NVMC_FUSE_READ:
            case CMD_NVMC_SIGNROW_READ:
            case CMD_NVMC_NVM_READ:
            case CMD_NVMC_GET_DEVID: 
            {
                usb_ep_clearSETUP();
                if (bit_is_set(g_pdi_flags, PDI_FLAG_PDI_MODE)) {
                    if (bit_is_set(g_pdi_flags, PDI_FLAG_PDI_ENABLED)) {
                        pdi_exec(req);
                    } else {
                        g_pdi_last_rc = 101;
                        usb_ep_send_control_data_finish();
                    }
                } else {
                    g_pdi_last_rc = 100;
                    usb_ep_send_control_data_finish();
                }
                return 0;
            }        
        }
    }
    return 1;
}


static uint8_t g_led_counter = 0;

ISR(TIMER1_OVF_vect) { // every 65ms
    g_led_counter++;
    if (g_led_counter >= 10) {
        STATUS_LED_PORT ^= Bit(STATUS_LED_BIT);
        g_led_counter = 0;
    }
}


static NOINLINE void init_system(void) {
    wdt_reset();
    wdt_disable();
    
    PORTB = 0b11111111;
    PORTC = 0b11111111;
    PORTD = 0b11111111;
    DDRB = 0;
    DDRC = 0;
    DDRD = 0;
    
    // system clock prescaler to 1/1 --> 8Mhz
    CLKPR = Bsv(CLKPCE,1);
    CLKPR = 0x00;
    
    setbit(STATUS_LED_DDR, STATUS_LED_BIT); // Pin for status led to: output
    clrbit(STATUS_LED_PORT, STATUS_LED_BIT); 

    // timer 1: for generic timing
    OCR1A = TCNT1 + AVR_TIMER_DIFF;
    TCCR1C = 0;
    TCCR1A = Bsv(WGM11,0)|Bsv(WGM10,0);     // Normal Mode
    TCCR1B = Bsv(WGM13,0)|Bsv(WGM12,0)|Bsv(CS12,0)|Bsv(CS11,1)|Bsv(CS10,0); // start timer with prescaler 1/8
    TIFR1 = 0xff; // clear interrupt flags
    TIMSK1 = Bsv(OCIE1A,1)|Bsv(TOIE1,1); // enable Timer1 TOV interrupt
}

int main(void) {
    cli();
    init_system();

    usb_init(&g_device_desc_P,
             &g_config_1_P, sizeof(g_config_1_P),
             g_strings_P, NUM_USB_STRINGS,
             USB_MEM_SRC_PROGMEM);

    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();
    sei();
    
    while(1) {
        usb_ep0_task();
    }
    
    return 0;
}
