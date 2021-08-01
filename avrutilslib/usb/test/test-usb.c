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
#include <util/delay_basic.h>



#include "avrutilslib/defines.h"

#define USE_LOGGER_USART 0
#define LOGGER_USART_BUF_SIZE 254
#define LOGGER_USART_ID 1
#define LOGGER_USART_BAUD_UBRR 12    // 38,4kBaud with F_CPU 8Mhz
#include "avrutilslib/logger-usart.c"
#include "avrutilslib/ringbuffer.c"


#include "avrutilslib/usb/usb.c"


const struct usb_device_desc g_device_desc_P PROGMEM = {
    .bLength            = sizeof(struct usb_device_desc),
    .bDescriptorType    = USB_DESC_DEVICE,
    .bcdUSB             = USB_SPEC_1_1,
    .bDeviceClass       = 0xff,
    .bDeviceSubClass    = 0xff,
    .bDeviceProtocol    = 0xff,
    .bMaxPacketSize0    = 32,
    .idVendor           = 0xf123,
    .idProduct          = 1,
    .bcdDevice          = 0x0001,
    .iManufacturer      = 1,
    .iProduct           = 2,
    .iSerialNumber      = 3,
    .bNumConfigurations = 1
};
const struct config_1 {
    struct usb_configuration_desc config;
    struct usb_interface_desc     interface;
    struct usb_endpoint_desc      ep1;
    struct usb_endpoint_desc      ep2;
} g_config_1_P PROGMEM = {
    .config = {
        .bLength            = sizeof(struct usb_configuration_desc),
        .bDescriptorType    = USB_DESC_CONFIGURATION,
        .wTotalLength       = sizeof(struct config_1),
        .bNumInterfaces     = 1,
        .bConfigurationValue = 1,
        .iConfiguration     = 0,
        .bmAttributes       = USB_CONFIG_ATTRIBUTES(USB_CONFIG_ATTR_SELFPOWERED),
        .bMaxPower          = 25
    },
    .interface = {
        .bLength            = sizeof(struct usb_interface_desc),
        .bDescriptorType    = USB_DESC_INTERFACE,
        .bInterfaceNumber   = 0,
        .bAlternateSetting  = 0,
        .bNumEndpoints      = 2,
        .bInterfaceClass    = 0xff,
        .bInterfaceSubClass = 0xff,
        .bInterfaceProtocoll = 0xff,
        .iInterface         = 0
    },
    .ep1 = {
        .bLength            = sizeof(struct usb_endpoint_desc),
        .bDescriptorType    = USB_DESC_ENDPOINT,
        .bEndpointAddress   = USB_ENDPOINT_ADDR_IN|1,
        .bmAttributes       = USB_ENDPOINT_ATTRIBUTES(USB_ENDPOINT_TYPE_INTERRUPT, 0, 0),
        .wMaxPacketSize     = 32,
        .bInterval          = 200
    },
    .ep2 = {
        .bLength            = sizeof(struct usb_endpoint_desc),
        .bDescriptorType    = USB_DESC_ENDPOINT,
        .bEndpointAddress   = USB_ENDPOINT_ADDR_OUT|2,
        .bmAttributes       = USB_ENDPOINT_ATTRIBUTES(USB_ENDPOINT_TYPE_INTERRUPT, 0, 0),
        .wMaxPacketSize     = 32,
        .bInterval          = 200
    }
};


USB_DEF_STR_CHARS_P(g_string0_P, {0x0409}, 2);
USB_DEF_STR_P(g_string1_P, L"BalaGi Inc");
USB_DEF_STR_P(g_string2_P, L"TestUsb");
USB_DEF_STR_P(g_string3_P, L"V 1.0");

#define NUM_USB_STRINGS 4
PGM_VOID_P const g_strings_P[NUM_USB_STRINGS] PROGMEM = {
    &g_string0_P,
    &g_string1_P,
    &g_string2_P,
    &g_string3_P
};

void ep1_task(void) {    
    static uint16_t paddr = 0x3000;
    
    UENUM = 1;
    if (bit_is_set(UEINTX, NAKINI) && usb_ep_isIN()) {
        DPSTR("ep1:IN");
        
        //uint8_t d = PINB;
        //usb_ep_send_in_data(&d, 1, USB_MEM_SRC_RAM);
        
        // bootloader auslesen
        if (paddr <= 0x4000-16) {
          usb_ep_send_in_data((const void*)paddr, 16, USB_MEM_SRC_PROGMEM);
          paddr += 16;
        }
        clrbit(UEINTX, NAKINI);
    }
}

void usb_event_set_configuration(void) {
    usb_ep_setup(USB_ENDPOINT_ADDR_IN|1, USB_ENDPOINT_TYPE_INTERRUPT, 32, USB_EP_BANKS_1);

    //usb_ep_setup(USB_ENDPOINT_ADDR_OUT|2, USB_ENDPOINT_TYPE_INTERRUPT, 32, USB_EP_BANKS_1);
}

void usb_event_ep0_request(struct usb_device_request* req) {
}

ISR(TIMER1_OVF_vect) {
    PORTB ^= Bit(PB5);
}


static NOINLINE void init_system(void) {
    wdt_reset();
    wdt_disable();
    
    PORTB = 0b11111111;
    PORTC = 0b11111111;
    PORTD = 0b11111111;
    DDRB = Bit(PB5); // PB5 output, Status LED
    DDRC = 0;
    DDRD = 0;
    
    // system clock prescaler to 1/1 --> 8Mhz
    CLKPR = Bsv(CLKPCE,1);
    CLKPR = 0x00;
}

int main(void) {
    cli();
    init_system();
    
    DINIT();

    // timer 1
    TCNT1 = 0x7fff;
    TCCR1C = 0;
    TCCR1A = Bsv(WGM11,0)|Bsv(WGM10,0);     // Normal Mode
    TCCR1B = Bsv(WGM13,0)|Bsv(WGM12,0)|Bsv(CS12,0)|Bsv(CS11,1)|Bsv(CS10,1); // start timer with prescaler 1/64
    TIFR1 = 0xff; // clear interrupt flags
    TIMSK1 = Bsv(TOIE1,1); // enable Timer1 TOV interrupt    
    
    usb_init(&g_device_desc_P,
             &g_config_1_P, sizeof(g_config_1_P),
             g_strings_P, NUM_USB_STRINGS,
             USB_MEM_SRC_PROGMEM);

    sei();
    
    DPSTR("system init done\n");
    while(1) {
        usb_ep0_task();
        ep1_task();
    }
    
    return 0;
}
