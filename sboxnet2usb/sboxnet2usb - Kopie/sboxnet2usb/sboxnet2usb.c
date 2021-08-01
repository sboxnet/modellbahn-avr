 /***************************************************************************
 *   Copyright (C) 2013-2015
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

#define SYSCLK_FREQ_MHZ 16
#define SYSCLK_DIVISOR  1
#define F_CPU   (SYSCLK_FREQ_MHZ * 1000000 / SYSCLK_DIVISOR)

#include "avrutilslib/defines.h"

#if !defined(__AVR_ATmega32U2__)
# error "please compile for device: atmega32u2"
#endif

/*  ATmega32U2
 * 
 * 32k Flash
 * 1k  SRAM
 * 1k  EEPROM
 * 
 */

#define FIRMWARE_VERSION 0x0101

//#define GET_BOOT_LOADER 1

#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/cpufunc.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include <avr/power.h> // for clock_prescale_set

uint8_t g_sboxnet_addr;
struct sboxnet_device g_sboxnet;

#define TC1_PRESCALER  (Bsv(CS02,0)|Bsv(CS01,1)|Bsv(CS00,0)) // 1/8
#define TC1_PERIOD_MS  33 // 32.77ms

#define SBOXNET_RECV_BUF_SIZE (250)
#define SBOXNET_TMIT_BUF_SIZE (250)

#define SBOXNET_DEBUG_ENABLED 1
#define SBOXNET_DEVICE_ADDR    g_sboxnet_addr
#define SBOXNET_VAR            (&g_sboxnet)
//#define SBOXNET_SECTION        BOOTLOADER_SECTION
#define SBOXNET_RxD_PORT PORTD
#define SBOXNET_RxD_BIT  PD2
#define SBOXNET_TxD_PORT PORTD
#define SBOXNET_TxD_BIT  PD3
#define SBOXNET_TIMER_TIMSK   TIMSK1
#define SBOXNET_TIMER_TIFR    TIFR1
#define SBOXNET_TIMER_OCIE    OCIE1B
#define SBOXNET_TIMER_OCF     OCF1B
#define SBOXNET_TIMER_TCNT    TCNT1
#define SBOXNET_TIMER_OCR     OCR1B
#define SBOXNET_TIMER_BIT_TICKS   8         // at 16MHz / 8
#define SBOXNET_TIMER_PERIOD_MASK 0xffff
#define SBOXNET_TIMER_RANDOM_MASK 0x3f
#define SBOXNET_HW_SENSOR_EICR   EICRA
#define SBOXNET_HW_SENSOR_ISC1   ISC21
#define SBOXNET_HW_SENSOR_ISC0   ISC20
#define SBOXNET_HW_SENSOR_EIMSK  EIMSK
#define SBOXNET_HW_SENSOR_IE_BIT INT2
#define SBOXNET_HW_SENSOR_EIFR   EIFR
#define SBOXNET_HW_SENSOR_IF_BIT INTF2
#define SBOXNET_USE_HW_SENSOR_PINCH  0
#define SBOXNET_USE_HW_SENSOR_EXTINT 1
#define SBOXNET_USART_ID       1
#define SBOXNET_USART_DEF_INTR 1
#define SBOXNET_PRNG_SEED      OSCCAL
#define SBOXNET_USE_SNIFFER    1
// with usart 1 (SBOXNET_USART_ID=1)
#include "sboxnet/sboxnet.c"

SBOXNET_SECTION ISR(TIMER1_COMPB_vect) {
    sboxnet_timer_bit();
}

enum CMDS {
    CMD_SBOXNET_SET_MODE   = 0x30,
    CMD_SBOXNET_GET_STATUS = 0x31,
    CMD_SBOXNET_RECEIVE   = 0x32,
    CMD_SBOXNET_TRANSMIT  = 0x33,
    CMD_SBOXNET_GET_DBGINFO  = 0x3a,
    CMD_SBOXNET_GET_DBGSTATE = 0x3b,
    CMD_SBOXNET_GET_DBGRECVBUF = 0x3c,
    CMD_SBOXNET_GET_DBGTMITBUF = 0x3d,
    
    CMD_DBG_STACK = 0x40,
    CMD_SET_SERIALNUMBER = 0x51,

#ifdef GET_BOOT_LOADER
    CMD_GET_BOOTLOADER  = 0xff,
#endif
};


#define SBOXNET_FLG_ENABLED       0x01
#define SBOXNET_FLG_SNIFFER       0x02
#define SBOXNET_FLG_BOOTLOADER    0x80
uint8_t g_sboxnet_flags = 0;

#define SBOXNET_STATUS_RX_MSG      0x01
#define SBOXNET_STATUS_RX_CANREAD  0x02
#define SBOXNET_STATUS_RX_CRC      0x04
#define SBOXNET_STATUS_TX_CANSEND  0x10
#define SBOXNET_STATUS_TX_ERR      0x20
#define SBOXNET_STATUS_TX_OVF      0x40
#define SBOXNET_STATUS_TX_XERR     0x80
uint8_t g_sboxnet_status = 0;

uint8_t g_timer = 0;
uint8_t g_timer_led_msg_read = 0;
uint8_t g_timer_led_msg_write = 0;

#define LED_ACTIVE    5
#define LED_MSG_READ  6
#define LED_MSG_WRITE 7

static void trigger_led_msg_read(void) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        g_timer_led_msg_read = 100/TC1_PERIOD_MS;
        setbit(PORTB, LED_MSG_READ);
    }
}
static void trigger_led_msg_write(void) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        g_timer_led_msg_write = 100/TC1_PERIOD_MS;
        setbit(PORTB, LED_MSG_WRITE);
    }
}


// dummy variable, need only addresses! symbols defined by linker.
extern uint8_t _end;
extern uint8_t __stack;

#define STACK_MAGIC 0xa5

static uint16_t stack_free(void) {
    uint8_t* p = &_end;
    while (p < &__stack) {
        if (*p != STACK_MAGIC) {
            break;
        }
        p++;
    }
    return (uint16_t)(p - &_end);
}

static uint16_t stack_size(void) {
    return (uint16_t)(&__stack - &_end);
}

__attribute__ ((section(".init8"))) __ATTR_NAKED
void stack_init8(void) {
    uint8_t* p = &_end;
    while (p < &__stack) {
        *p++ = STACK_MAGIC;
    }
}

#define USE_LOGGER_USART 0
#include "avrutilslib/usb/usb.c"

#define SERIALNUMBER_SIZE 32
struct Eeprom {
    struct serial {
        struct usb_string_desc desc;
        wchar_t                str[SERIALNUMBER_SIZE];
    } serialnumber;
};
struct Eeprom eeprom EEMEM;

#define VENDOR_ID  0xf123
#define PRODUCT_ID 0xa002

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
    .bcdDevice          = FIRMWARE_VERSION,
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
USB_DEF_STR_P(g_string2_P, L"sboxnet2usb");

#define NUM_USB_STRINGS 3
PGM_VOID_P const g_strings_P[NUM_USB_STRINGS] PROGMEM = {
    &g_string0_P,
    &g_string1_P,
    &g_string2_P,
};

uint8_t get_string(struct usb_device_request* req, uint8_t idx) {
    if (idx == 3) {
        usb_ep_clearSETUP();
        usb_ep_send_control_data(&eeprom.serialnumber.desc, usb_mem_read_byte(&eeprom.serialnumber.desc.bLength, USB_MEM_SRC_EEPROM),
                                 USB_MEM_SRC_EEPROM, req->wLength);
        return 0;
    }
    return 1;
}

void usb_event_set_configuration(void) {
}

static uint8_t get_status(void) {
    uint16_t st = g_sboxnet_status;
    g_sboxnet_status = 0;
    if (sboxnet_can_read()) {
        st |= SBOXNET_STATUS_RX_CANREAD;
    }
    if (sboxnet_can_send()) {
        st |= SBOXNET_STATUS_TX_CANSEND;
    }
    return st;
}

uint8_t usb_event_ep0_request(struct usb_device_request* req) {
    if ( (req->bmRequestType & 0x7f) != (USB_REQTYPE_TYP_VENDOR|USB_REQTYPE_REC_INTERFACE) )
        return 1;

    switch(req->bRequest) {        
        case CMD_SBOXNET_SET_MODE: {
            usb_ep_clearSETUP();
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                if (req->wValue & SBOXNET_FLG_BOOTLOADER) {
                    wdt_reset();
                    // first, enable watchdog. the watchdog timeout makes a reset of the device.
                    // then after the reset, in init_system() MCUSR is checked if the cause of the reset was a timeout.
                    // if yes, jump to bootloader.
                    WDTCSR = Bit(WDCE)|Bit(WDE);
                    WDTCSR = Bit(WDP2)|Bit(WDE);  // 0.5s timeout

                } else if (req->wValue & SBOXNET_FLG_ENABLED) {
                    g_sboxnet_addr = req->wIndex;
                    if (!(g_sboxnet_flags & SBOXNET_FLG_ENABLED)) {
                        sboxnet_init();
                        g_sboxnet_flags |= SBOXNET_FLG_ENABLED;
                    }
                    sboxnet_set_sniffer_mode(req->wValue & SBOXNET_FLG_SNIFFER);
                } else {
                    if (g_sboxnet_flags & SBOXNET_FLG_ENABLED) {
                        sboxnet_deinit();
                        g_sboxnet_flags &= ~SBOXNET_FLG_ENABLED;
                    }
                }
            }
            usb_ep_send_zlp();
            return 0;
        }
        
        case CMD_SBOXNET_GET_STATUS: {            
            usb_ep_clearSETUP();
            uint8_t st = get_status();
            usb_ep_send_control_data(&st, 1, USB_MEM_SRC_RAM, req->wLength);
            return 0;
        }
        
        case CMD_SBOXNET_RECEIVE: {
            usb_ep_clearSETUP();
 
            uint16_t wlen = req->wLength;
            uint8_t usbrc = 0;
            struct {
                uint8_t status;
            } buf;
            if (wlen >= sizeof(buf)) {
                wlen -= sizeof(buf);
                buf.status = get_status();
                usbrc = usb_ep_send_control_data_chunk(&buf, sizeof(buf), USB_MEM_SRC_RAM);
            }
            
            if (usbrc == USB_RC_OK) {
                while (wlen > 1) {
                    struct {
                        int8_t msglen;
                        struct sboxnet_msg_max msg;
                    } buf;
                    uint8_t maxs = sizeof(buf.msg);
                    if (wlen < 257) {
                        maxs = (uint8_t)(wlen - 1);
                    }
                    buf.msglen = sboxnet_receive_msg(&buf.msg.msgh, maxs);
                    if (buf.msglen > 0) {
                        wlen -= (buf.msglen + 1);
                        trigger_led_msg_read();
                        usbrc = usb_ep_send_control_data_chunk(&buf, buf.msglen + 1, USB_MEM_SRC_RAM);
                    } else {
                        usbrc = usb_ep_send_control_data_chunk(&buf.msglen, 1, USB_MEM_SRC_RAM);
                        break;
                    }
                    if (usbrc != USB_RC_OK) {
                        break;
                    }
                }
            }
            if (usbrc == USB_RC_OK) {
                usb_ep_send_control_data_finish();
            }
            return 0;
        }
        
        case CMD_SBOXNET_TRANSMIT: {            
            usb_ep_clearSETUP();
            struct sboxnet_msg_max msg;
            uint16_t readb;
            usb_ep_recv_control_data(&msg, &readb, minvalof(req->wLength,sizeof(msg)));
            if (readb >= SBOXNET_MSG_HEADER_LEN) {
                if (sboxnet_can_send_msg(&msg.msgh)) {
                    trigger_led_msg_write();
                    if (sboxnet_send_msg(&msg.msgh) != 0)
                        g_sboxnet_status |= SBOXNET_STATUS_TX_XERR;
                } else {
                    g_sboxnet_status |= SBOXNET_STATUS_TX_OVF;
                }
            } else {
                g_sboxnet_status |= SBOXNET_STATUS_TX_ERR;
            }
            return 0;
        }
        
        case CMD_SBOXNET_GET_DBGINFO: {
            usb_ep_clearSETUP();
            usb_ep_send_control_data(&g_sboxnet.debug, sizeof(struct sboxnet_debug_info), USB_MEM_SRC_RAM, req->wLength);
            return 0;
        }

        case CMD_SBOXNET_GET_DBGSTATE: {
            usb_ep_clearSETUP();
            usb_ep_send_control_data(&g_sboxnet, 9, USB_MEM_SRC_RAM, req->wLength);
            return 0;
        }
        
        case CMD_SBOXNET_GET_DBGRECVBUF: {
            usb_ep_clearSETUP();
            usb_ep_send_control_data(&g_sboxnet.recv_buf, sizeof(g_sboxnet.recv_buf), USB_MEM_SRC_RAM, req->wLength);
            return 0;
        }
        case CMD_SBOXNET_GET_DBGTMITBUF: {
            usb_ep_clearSETUP();
            usb_ep_send_control_data(&g_sboxnet.tmit_buf, sizeof(g_sboxnet.tmit_buf), USB_MEM_SRC_RAM, req->wLength);
            return 0;
        }
        
        case CMD_DBG_STACK: {
            usb_ep_clearSETUP();
            struct {
                uint16_t size;
                uint16_t free;
            } buf;
            buf.size = stack_size();
            buf.free = stack_free();
            usb_ep_send_control_data(&buf, sizeof(buf), USB_MEM_SRC_RAM, req->wLength);
            return 0;   
        }
        
        case CMD_SET_SERIALNUMBER: {
            usb_ep_clearSETUP();
            struct {
                struct usb_string_desc desc;
                wchar_t str[SERIALNUMBER_SIZE];
            } buf;
            memset(&buf, 0xff, sizeof(buf));
            uint16_t readb;
            usb_ep_recv_control_data(&buf.str, &readb, minvalof(req->wLength,sizeof(buf.str)));
            buf.desc.bLength = sizeof(struct usb_string_desc)+(uint8_t)readb;
            buf.desc.bDescriptorType = USB_DESC_STRING;
            eeprom_update_block(&buf, &eeprom.serialnumber, sizeof(struct usb_string_desc)+readb);
            return 0;
        }

        
#ifdef GET_BOOT_LOADER
        case CMD_GET_BOOTLOADER: {
            usb_ep_clearSETUP();
            usb_ep_send_control_data((const void *)((uint16_t)FLASHEND+1 - 4096), 4096, USB_MEM_SRC_PROGMEM, req->wLength);      
            return 0;
        }
#endif
        
        default: break;
    }
    return 1;
}

uint8_t t1 = 0;
uint8_t t2 = 0;
ISR(TIMER1_OVF_vect) { // every 32.77ms
    
    g_timer++;
	// startup test: blink leds
	static uint8_t rb = 1;
	switch (g_timer & 0xf) {// every 0.5s
		case 0: // every 0.5s
		{	
			rb <<= 1;
			switch(rb) {
				case 0:
				{
					PORTB ^= Bit(LED_ACTIVE);
					break;
				}
				case 0x02:
				{
					PORTB ^= Bit(LED_ACTIVE);
					break;
				}
				case 0x04:
				{
					PORTB ^= Bit(LED_MSG_READ);
					break;
				}
				case 0x08:
				{
					PORTB ^= Bit(LED_MSG_WRITE);
					break;
				}
				case 0x10:
				{
					PORTB ^= Bit(LED_ACTIVE);
					break;
				}
				case 0x20:
				{
					PORTB ^= Bit(LED_MSG_READ);
					break;
				}
				case 0x40:
				{
					PORTB ^= Bit(LED_MSG_WRITE);
					break;
				}
				case 0x80:
				{
					rb = 0;
				}
			}
			// blink ACTIVE LED
			//PORTB ^= Bit(LED_ACTIVE);
		//PORTB ^= Bit(LED_MSG_READ);
		//PORTB ^= Bit(LED_MSG_WRITE);
			break;
		}
/*		case 1:
		{
			if (!t1)
				PORTB ^= Bit(LED_MSG_READ);
			t1 = 1;
			break;
		}
		case 2:
		{
			if (!t2)
				PORTB ^= Bit(LED_MSG_WRITE);
			break;
		}
*/
	}
    if (g_timer_led_msg_read) {
        g_timer_led_msg_read--;
        if (g_timer_led_msg_read == 0)
            clrbit(PORTB, LED_MSG_READ);
    }

    if (g_timer_led_msg_write) {
        g_timer_led_msg_write--;
        if (g_timer_led_msg_write == 0)
            clrbit(PORTB, LED_MSG_WRITE);
    }
}


static NOINLINE void init_system(void) {
// sboxnet_init is done when sboxnet2usb is init over usb command CMD_SBOXNET_SET_MODE
    wdt_reset();
    uint8_t mcusr = MCUSR;
    MCUSR = 0;
    wdt_disable();
    if (bit_is_set(mcusr, WDRF)) {
        // jump to bootloader
        __asm__ __volatile__ (
            "jmp  0x3000\n"    // jump to bootloader code
        );
    }
    
	// PB7 ge Write Access
	// PB6 gn Read Access
	// PB5 rt sboxnet2usb is active/blinks
	// PB1 SCLK
	// PB2 MOSI
	// PB3 MISO
	// PC1 Reset with external pull up
    PORTB = 0b00011111; // PB5..7: LEDs
	// PC4 .. PC7 N.C.
    PORTC = 0b11111111;
	// PD2 read from bus
	// PD3 write to bus
    PORTD = 0b01111111; // PD7=HWBE is low over resistor
	// DDR
	// PB5..7 Output
    DDRB = 0b11100000; // PB5..7: LEDs
	// PC input
    DDRC = 0;
	// PD input, Bus transmitter overwrites port PD3 to be output when transmitter is enabled
    DDRD = 0;
    
    // system clock prescaler
	// set CLKPE to enable change of CLKPS bits in CLKPR
//    CLKPR = Bsv(CLKPCE,1);
//#if SYSCLK_DIVISOR == 2
//    CLKPR = 0x01;
//#else
//    CLKPR = 0x00;
//#endif
#if SYSCLK_DIVISOR == 2
	clock_prescale_set(1);
#else
	clock_prescale_set(0);
#endif
    
    // timer 1: for generic timing and sboxnet. Every 32,77ms Overflow.
    TCCR1A = Bsv(WGM01,0)|Bsv(WGM00,0);     // Normal Mode
    TCCR1B = Bsv(WGM02,0)|TC1_PRESCALER;
    TIFR1 = 0xff; // clear interrupt flags
    TIMSK1 = Bsv(TOIE1,1); // enable Timer1 TOV interrupt
    
    if (eeprom_read_word((const uint16_t *)&eeprom.serialnumber.desc) == 0xffff) {
        /* empty SerialNumber string */
        eeprom_update_byte(&eeprom.serialnumber.desc.bLength, sizeof(struct usb_string_desc)+2);        
        eeprom_update_byte(&eeprom.serialnumber.desc.bDescriptorType, USB_DESC_STRING);
        eeprom_update_word((uint16_t*)&eeprom.serialnumber.str[0], 'x');
    }
}

int main(void) {
    cli();
    init_system();
        
    usb_init(&g_device_desc_P,
             &g_config_1_P, sizeof(g_config_1_P),
             g_strings_P, NUM_USB_STRINGS, &get_string,
             USB_MEM_SRC_PROGMEM);

    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();
    sei();
    
    while(1) {
        usb_ep0_task();
    }
    
    return 0;
}

