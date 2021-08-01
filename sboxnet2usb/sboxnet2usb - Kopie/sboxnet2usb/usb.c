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

#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>

#include "avrutilslib/logger-usart.h"
#include "avrutilslib/ringbuffer.h"
#include "avrutilslib/usb/usb.h"


#if SYSCLK_FREQ_MHZ == 8
# define USB_PLL_CLK_DIV 0
#elif SYSCLK_FREQ_MHZ == 16
# define USB_PLL_CLK_DIV 1
#else
# error "SYSCLK_FREQ_MHZ must be 8MHz or 16MHz for USB to operate"
#endif


struct usb_dev g_usb_dev;


#if USE_LOGGER_USART == 1
void usb_dump_dev_registers(void) {
    DPSTR("USBCON:"); DHEXB(USBCON);
    DPSTR("UPOE:"); DHEXB(UPOE);
    DPSTR("PLLCSR:"); DHEXB(PLLCSR);
    DPSTR("UDIEN:"); DHEXB(UDIEN);
    DPSTR("UDINT:"); DHEXB(UDINT);
    DPSTR("UENUM:"); DHEXB(UENUM);
    DPSTR("UDADDR:"); DHEXB(UDADDR);
    DPSTR("UEINT:"); DHEXB(UEINT);
    DPSTR("UERST:"); DHEXB(UERST);
    DPSTR("\n");
}
void usb_dump_ep_registers(void) {
    DPSTR("UENUM:"); DHEXB(UENUM);
    DPSTR("UECONX:"); DHEXB(UECONX);
    DPSTR("UECFG0X:"); DHEXB(UECFG0X);
    DPSTR("UECFG1X:"); DHEXB(UECFG1X);
    DPSTR("UESTA0X:"); DHEXB(UESTA0X);
    DPSTR("UESTA1X:"); DHEXB(UESTA1X);
    DPSTR("UEINTX:"); DHEXB(UEINTX);
    DPSTR("UEIENX:"); DHEXB(UEIENX);
    DPSTR("UEBCLX:"); DHEXB(UEBCLX);
    DPSTR("\n");
}
#endif // USE_LOGGER_USART


// forward declarations
void usb_intr_endofreset(void);


ISR(USB_GEN_vect) {
    DPSTR("GV");
    uint8_t intf = UDINT;
    uint8_t inte = UDIEN;
    
    uint8_t epnum_save = UENUM;
    
    if (bit_is_set(intf,UPRSMI)) {
        DPSTR(":UPR");
    }
    if (bit_is_set(intf,EORSMI)) {
        DPSTR(":EOR");
    }
    if (bit_is_set(intf,WAKEUPI)) {
        DPSTR(":WAK");
    }
    if (bit_is_set(intf,EORSTI)) {
        DPSTR(":ERS");
        if (bit_is_set(inte, EORSTE)) {
            usb_intr_endofreset();
        }
        clrbit(UDINT, EORSTI);
    }
    if (bit_is_set(intf,SOFI)) {
        DPSTR(":SOF");
    }
    if (bit_is_set(intf,SUSPI)) {
        DPSTR(":SUP");
    }
    DPSTR("\n");
    UDINT = 0; // clear all flags
    UENUM = epnum_save;
}


void usb_ep_read_data(void* buf, uint8_t size) {
    while (size) {
        *(uint8_t*)buf++ = UEDATX;
        size--;
    }
}

uint8_t usb_ep_waitIN(void) { //  wait till TXINI is set
    while (!usb_ep_isIN()) {
        if (!(g_usb_dev.state & (USB_DEVICE_STATE_ADDRESS|USB_DEVICE_STATE_CONFIGURED|USB_DEVICE_STATE_DEFAULT))) {
            return USB_RC_STATE_ABORT;
        }
    }
    return USB_RC_OK;
}

uint8_t usb_ep_send_zlp(void) {
    uint8_t rc = usb_ep_waitIN();
    if (!rc) {
        usb_ep_clearIN();
        rc = usb_ep_waitIN();
    }
    return rc;
}


uint8_t usb_mem_read_byte(const void* psrc, uint8_t srctype) {
    switch (srctype) {
        case USB_MEM_SRC_PROGMEM:
            return pgm_read_byte((const uint8_t*)psrc);
            
        case USB_MEM_SRC_EEPROM:
            return eeprom_read_byte((const uint8_t*)psrc);
            
        case USB_MEM_SRC_RAM:
        default:
            return *(const uint8_t*)psrc;
    }
}

uint8_t usb_ep_getbufsize(void) {
    static const uint8_t epsizemap[4] PROGMEM = { 8, 16, 32, 64 };
    return pgm_read_byte(&epsizemap[(UECFG1X >> 4) & 0x03]);
}

//------------------------------------------------------------

uint8_t usb_ep_send_in_data(const void* psrc, uint16_t size, uint8_t srctype) {
    DPSTR("[");
    DHEXB(UEINTX);
    DPSTR(":");
    DHEXB(size);
    DPSTR(":");

    for (;;) {
        if (!(g_usb_dev.state & (USB_DEVICE_STATE_ADDRESS|USB_DEVICE_STATE_CONFIGURED|USB_DEVICE_STATE_DEFAULT))) {
            return USB_RC_STATE_ABORT;
        } else if (usb_ep_isSETUP()) { // new setup packet received -> abort
            DPSTR("AS");
            return USB_RC_NEW_SETUP;
        } else if (usb_ep_isOUT()) { // out packet received -> abort
            usb_ep_clearOUT();
            DPSTR("AO");
            return USB_RC_NEW_OUT;
        } else if (!usb_ep_isIN()) { // transmitter not ready -> retry
            DCHECK();
            continue;
        }

        while (size && bit_is_set(UEINTX, RWAL)) {
            uint8_t d = usb_mem_read_byte(psrc++, srctype);
            UEDATX = d;
            size--;
        }
        DPSTR("CI");
        usb_ep_clearIN();
        usb_ep_clearFIFOCON();
        
        if (size == 0) {
            break;
        }
    }
    DPSTR("SE\n");
    return USB_RC_OK;
}

//------------------------------------------------------------

uint8_t usb_ep_send_control_data_chunk(const void* psrc, uint16_t size, uint8_t srctype) {
    uint8_t epsize = usb_ep_getbufsize();

    DPSTR("#");
    DHEXB(UEINTX);
    DPSTR(":");
    DHEXB(size);
    DPSTR(":");

    for (;;) {
        if (!(g_usb_dev.state & (USB_DEVICE_STATE_ADDRESS|USB_DEVICE_STATE_CONFIGURED|USB_DEVICE_STATE_DEFAULT))) {
            return USB_RC_STATE_ABORT;
        } else if (usb_ep_isSETUP()) { // new setup packet received -> abort
            DPSTR("AS");
            return USB_RC_NEW_SETUP;
        } else if (usb_ep_isOUT()) { // out packet received -> abort
            usb_ep_clearOUT();
            DPSTR("AO");
            return USB_RC_NEW_OUT;
        } else if (!usb_ep_isIN()) { // transmitter not ready -> retry
            DCHECK();
            continue;
        }

        uint8_t written = UEBCLX; // num of bytes in fifo
        while (size && written < epsize) {
            UEDATX = usb_mem_read_byte(psrc++, srctype);
            size--;
            written++;
        }
        if (written == epsize) {  // send it
            DPSTR("CI");
            usb_ep_clearIN();
        }
        if (size == 0) {
            break;
        }
    }
    return USB_RC_OK;
}

uint8_t usb_ep_send_control_data_finish(void) {
    for (;;) {
        if (!(g_usb_dev.state & (USB_DEVICE_STATE_ADDRESS|USB_DEVICE_STATE_CONFIGURED|USB_DEVICE_STATE_DEFAULT))) {
            return USB_RC_STATE_ABORT;
        } else if (usb_ep_isSETUP()) { // new setup packet received -> abort
            DPSTR("AS");
            return USB_RC_NEW_SETUP;
        } else if (usb_ep_isOUT()) { // out packet received -> abort
            usb_ep_clearOUT();
            DPSTR("AO");
            return USB_RC_NEW_OUT;
        } else if (!usb_ep_isIN()) { // transmitter not ready -> retry
            DCHECK();
            continue;
        }
        DPSTR("CI");
        usb_ep_clearIN();
        break;
    }
    DPSTR("WO");
    while (!usb_ep_isOUT()) { // STATUS stage: wait till OUT package received
        if (!(g_usb_dev.state & (USB_DEVICE_STATE_ADDRESS|USB_DEVICE_STATE_CONFIGURED|USB_DEVICE_STATE_DEFAULT))) {
            return USB_RC_STATE_ABORT;
        } else if (usb_ep_isSETUP()) { // new setup packet received -> abort
            DPSTR("AS");
            return USB_RC_NEW_SETUP;
        }
    }
    usb_ep_clearOUT();
    DPSTR("SE\n");
    return USB_RC_OK;
}

uint8_t usb_ep_send_control_data(const void* psrc, uint16_t size, uint8_t srctype, uint16_t reqsize) {
    uint8_t r = usb_ep_send_control_data_chunk(psrc, minvalof(size, reqsize), srctype);
    if (r == USB_RC_OK) {
        r = usb_ep_send_control_data_finish();
    }
    return r;
}

//------------------------------------------------------------

uint8_t usb_ep_recv_control_data_chunk(void* psrc, uint16_t* psize, uint16_t size) {
    uint16_t readb = 0;
    *psize = 0;
    if (size) {
        for (;;) {
            if (!(g_usb_dev.state & (USB_DEVICE_STATE_ADDRESS|USB_DEVICE_STATE_CONFIGURED|USB_DEVICE_STATE_DEFAULT))) {
                return USB_RC_STATE_ABORT;
            } else if (usb_ep_isSETUP()) { // new setup packet received -> abort
                return USB_RC_NEW_SETUP;
            } else if (!usb_ep_isOUT()) {
                continue;
            }
        
            while (UEBCLX && readb < size) {
                *(uint8_t*)psrc++ = UEDATX;
                readb++;
            }
            if (UEBCLX == 0) {
                usb_ep_clearOUT();
            }
            if (readb >= size) {
                *psize = readb;
                break;
            }
        }
    }
    return USB_RC_OK;
}

uint8_t usb_ep_recv_control_data_finish(void) {
    usb_ep_clearOUT();
    return usb_ep_send_zlp();
}

uint8_t usb_ep_recv_control_data(void* psrc, uint16_t* psize, uint16_t reqsize) {    
    uint8_t r = usb_ep_recv_control_data_chunk(psrc, psize, reqsize);
    if (r == USB_RC_OK) {
        r = usb_ep_recv_control_data_finish();
    }
    return r;
}

//------------------------------------------------------------

static uint8_t usb_ep0_get_configuration(void) {
    struct usb_dev* dev = &g_usb_dev;
    if (dev->state & (USB_DEVICE_STATE_ADDRESS|USB_DEVICE_STATE_CONFIGURED)) {
        usb_ep_clearSETUP();
        usb_ep_send_control_data(&dev->cur_config, 1, USB_MEM_SRC_RAM, 1);
        return 0;
    }
    return 1;
}

static uint8_t usb_ep0_get_descriptor(struct usb_device_request* req) {
    struct usb_dev* dev = &g_usb_dev;
    switch (highbyte(req->wValue)) {
        case USB_DESC_DEVICE:
        {
            usb_ep_clearSETUP();
            usb_ep_send_control_data(dev->pdesc_dev, sizeof(struct usb_device_desc), dev->desc_dtype, req->wLength);
            return 0;
        }
        
        case USB_DESC_CONFIGURATION:
        {
            uint8_t idx = lowbyte(req->wValue);
            if (idx == 0) {
                usb_ep_clearSETUP();
                usb_ep_send_control_data(dev->pdesc_config, dev->desc_config_size, dev->desc_dtype, req->wLength);
                return 0;
            }
            break;
        }
            
        case USB_DESC_STRING:
        {   // ignore language
            uint8_t idx = lowbyte(req->wValue);
            if (dev->get_string_func != NULL && (*dev->get_string_func)(req, idx) == 0) {
                return 0;
            }
            if (idx < dev->desc_string_num) {
                const struct usb_string_desc* paddr;
                
                switch (dev->desc_dtype) {
                    case USB_MEM_SRC_PROGMEM:
                        paddr = (const struct usb_string_desc*)pgm_read_word(dev->pdesc_string + idx);
                        break;
                
                    case USB_MEM_SRC_EEPROM:
                        paddr = (const struct usb_string_desc*)eeprom_read_word((const uint16_t*)dev->pdesc_string + idx);
                        break;
                        
                    default:
                        paddr = (const struct usb_string_desc*)dev->pdesc_string[idx];
                        break;
                }
                if (paddr != NULL) {
                    usb_ep_clearSETUP();
                    usb_ep_send_control_data(paddr, usb_mem_read_byte(paddr, dev->desc_dtype), dev->desc_dtype, req->wLength);
                    return 0;
                }
            }
            break;
        }
        
        default:
            break;
    }
    return 1;
}

static uint8_t usb_ep0_set_address(struct usb_device_request* req) {
    struct usb_dev* dev = &g_usb_dev;
    
    if (dev->state & (USB_DEVICE_STATE_DEFAULT|USB_DEVICE_STATE_ADDRESS)) {
        uint8_t addr = req->wValue & 0x007f;
        usb_ep_clearSETUP();
        UDADDR = addr;
        usb_ep_send_zlp();
        setbit(UDADDR, ADDEN);
        if (addr) {
            dev->state = USB_DEVICE_STATE_ADDRESS;
        } else {
            dev->state = USB_DEVICE_STATE_DEFAULT;
        }
        return 0;
    }
    return 1;
}

static uint8_t usb_ep0_set_configuration(struct usb_device_request* req) {
    struct usb_dev* dev = &g_usb_dev;
    if (dev->state & (USB_DEVICE_STATE_ADDRESS|USB_DEVICE_STATE_CONFIGURED)) {
        uint8_t confnr = lowbyte(req->wValue);
        if (confnr == 0) {
            dev->state = USB_DEVICE_STATE_ADDRESS;
            dev->cur_config = 0;
            
        } else {
            uint8_t cnr = usb_mem_read_byte(&((struct usb_configuration_desc*)dev->pdesc_config)->bConfigurationValue, dev->desc_dtype);
            if (confnr == cnr) {
                dev->state = USB_DEVICE_STATE_CONFIGURED;
                dev->cur_config = confnr;

                usb_event_set_configuration();
                
                UENUM = 0; // switch back to ep0 !

            } else {
                return 1;
            }
        }
        usb_ep_clearSETUP();
        usb_ep_send_zlp();
        return 0;
    }
    return 1;
}


void usb_ep0_task(void) {
    struct usb_device_request req;
    
    UENUM = 0;
    if (usb_ep_isSETUP()) { // setup packet received
        // read packet
        usb_ep_read_data(&req, sizeof(req));
        
        uint8_t rc = 1;
        
        DPSTR("ep0:");
        DHEXB(req.bmRequestType);
        DHEXB(req.bRequest);
        DPSTR(",V"); DHEXW(req.wValue);
        DPSTR(",I"); DHEXW(req.wIndex);
        DPSTR(",L"); DHEXW(req.wLength);
        if ((req.bmRequestType & USB_REQTYPE_TYP_mask) == USB_REQTYPE_TYP_STANDARD) {
            switch(req.bRequest) {
                case USB_REQUEST_CLEAR_FEATURE:
                    // not supported
                    break;
                    
                case USB_REQUEST_GET_CONFIGURATION:
                    if (req.bmRequestType == (USB_REQTYPE_DEV2HOST|USB_REQTYPE_TYP_STANDARD|USB_REQTYPE_REC_DEVICE)) {
                        rc = usb_ep0_get_configuration();
                    }
                    break;
                
                case USB_REQUEST_GET_DESCRIPTOR:
                    if (req.bmRequestType == (USB_REQTYPE_DEV2HOST|USB_REQTYPE_TYP_STANDARD|USB_REQTYPE_REC_DEVICE)) {
                        rc = usb_ep0_get_descriptor(&req);
                    }
                    break;
                    
                case USB_REQUEST_GET_INTERFACE:
                    // not supported
                    break;
                    
                case USB_REQUEST_GET_STATUS:
                    // not supported
                    break;
                    
                case USB_REQUEST_SET_ADDRESS:
                    if (req.bmRequestType == (USB_REQTYPE_HOST2DEV|USB_REQTYPE_TYP_STANDARD|USB_REQTYPE_REC_DEVICE)) {
                        rc = usb_ep0_set_address(&req);
                    }
                    break;
                
                case USB_REQUEST_SET_CONFIGURATION:
                    if (req.bmRequestType == (USB_REQTYPE_HOST2DEV|USB_REQTYPE_TYP_STANDARD|USB_REQTYPE_REC_DEVICE)) {
                        rc = usb_ep0_set_configuration(&req);
                    }
                    break;
                    
                case USB_REQUEST_SET_DESCRIPTOR:
                    // not supported
                    break;
                    
                case USB_REQUEST_SET_FEATURE:
                    // not supported
                    break;
                    
                case USB_REQUEST_SET_INTERFACE:
                    // not supported yet. only needed if there are alternate interface settings
                    break;
                    
                case USB_REQUEST_SYNCH_FRAME:
                    // not supported yet. only needed in isochronous transfer mode
                    break;
                    
                default:
                    break;
            }
        }
        if (rc != 0) {
            rc = usb_event_ep0_request(&req);
        }
        
        UENUM = 0;
        
        if (rc != 0) { // if SETUP packet was not processed, enter STALL
            usb_ep_clearSETUP();
            usb_ep_set_stall();
            DPSTR("np");
        }
        DPSTR("\n");
    }
}


void usb_ep_setup(uint8_t epaddr, uint8_t epattr, uint8_t epsize, uint8_t epbanks) {
    // epaddr, epattr, epsize same as in endpoint descriptor
    uint8_t sz;
    uint8_t type;
    switch (epsize) {
        case 8:  sz = (USB_EP_SIZE_8 << 4)|Bsv(ALLOC,1); break;
        case 16: sz = (USB_EP_SIZE_16 << 4)|Bsv(ALLOC,1); break;
        case 32: sz = (USB_EP_SIZE_32 << 4)|Bsv(ALLOC,1); break;
        default: sz = (USB_EP_SIZE_64 << 4)|Bsv(ALLOC,1); break;
    }
    type = (epattr & 0x03);
    
    UENUM = epaddr & 0x07;
    setbit(UECONX, EPEN);
    UECFG0X = (type << 6)|(((epaddr & 0x80) == USB_ENDPOINT_ADDR_IN) ? Bit(EPDIR) : 0);
    UECFG1X = sz | ((epbanks & 0x01) << 2);
    
    switch (type) {
/*
        case USB_ENDPOINT_TYPE_CONTROL:
                UEIENX = Bsv(RXSTPE,1); // enable receive setup packet interrupts
                break;

        case USB_ENDPOINT_TYPE_INTERRUPT:
                if ((epaddr & 0x80) == USB_ENDPOINT_ADDR_IN) {
                    UEIENX = Bsv(NAKINE,1);
                } else {
                    UEIENX = Bsv(RXOUTE,1);
                }
                break;
*/                
        default:
                UEIENX = 0;
                break;
    }

    DPSTR("EP"); DHEXB(UENUM); DPSTR(":"); DHEXB((UESTA0X & Bit(CFGOK)));
}

void usb_ep_setup_control(void) {
    struct usb_dev* dev = &g_usb_dev;
    uint8_t size = usb_mem_read_byte(&dev->pdesc_dev->bMaxPacketSize0, dev->desc_dtype);
    usb_ep_setup(USB_ENDPOINT_ADDR_OUT|0, USB_ENDPOINT_TYPE_CONTROL, size, USB_EP_BANKS_1);
}

void usb_intr_endofreset(void) {
    usb_ep_setup_control();

    g_usb_dev.state = USB_DEVICE_STATE_DEFAULT;
    g_usb_dev.cur_config = 0;
}

void usb_init(const struct usb_device_desc* pdevdesc,
              const void* pconfig, uint8_t configsize,
              const void* const* pstrings, uint8_t numstrings,
              get_string_func_t getstringfunc,
              uint8_t desc_dtype) {
    DPSTR("USB init\n");
    
    struct usb_dev* dev = &g_usb_dev;
    memset(dev, 0, sizeof(struct usb_dev));
    dev->state = USB_DEVICE_STATE_NONE;
    dev->cur_config = 0;
    dev->pdesc_dev = pdevdesc;
    dev->pdesc_config = pconfig;
    dev->desc_config_size = configsize;
    dev->pdesc_string = pstrings;
    dev->desc_string_num = numstrings;
    dev->get_string_func = getstringfunc;
    dev->desc_dtype = desc_dtype;
    
    USBCON = Bsv(USBE,0)|Bsv(FRZCLK,1); // make sure USB device is disabled
    UPOE = 0; // disable direct drive of USB/PS2 pads
    
    // startup PLL
    PLLCSR = (USB_PLL_CLK_DIV<<2);
    PLLCSR = (USB_PLL_CLK_DIV<<2)|Bsv(PLLE,1); // set PLL clock division and enable PLL
    // wait till PLL is locked
    while (bit_is_clear(PLLCSR, PLOCK));
    // PLL locked

    // enable USB device.
    USBCON = Bsv(USBE,1)|Bsv(FRZCLK,1); // first enable device
    USBCON = Bsv(USBE,1)|Bsv(FRZCLK,0); // then unfreeze clock !
    

    UDIEN = 0; // disable all usb device interrupts
    UDINT = 0; // clear usb device interrupt flags (should be already cleared)
    
#if USE_LOGGER_USART == 1
    UDIEN = Bsv(UPRSME,1)|Bsv(EORSME,1)|Bsv(WAKEUPE,0)|Bsv(EORSTE,1)|Bsv(SOFE,0)|Bsv(SUSPE,1); // enable some usb generic interrupts
#else
    UDIEN = Bsv(UPRSME,0)|Bsv(EORSME,0)|Bsv(WAKEUPE,0)|Bsv(EORSTE,1)|Bsv(SOFE,0)|Bsv(SUSPE,1); // enable some usb generic interrupts
#endif
    UDCON = Bsv(RSTCPU,0)|Bsv(RMWKUP,0)|Bsv(DETACH,0); // attach usb device, no cpu reset and no remote wakeup
    
    dev->state = USB_DEVICE_STATE_DEFAULT;
    
    DPSTR("USB init done\n");
#if USE_LOGGER_USART == 1
    usb_dump_dev_registers();
#endif
}

