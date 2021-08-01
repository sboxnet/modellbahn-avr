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

#ifndef _AVRUTILSLIB_USB_
#define _AVRUTILSLIB_USB_

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

// NOTE: USB uses little endian format
// NOTE: avr-gcc uses little endian format too

#define USB_DESC_DEVICE         1
#define USB_DESC_CONFIGURATION  2
#define USB_DESC_STRING         3
#define USB_DESC_INTERFACE      4
#define USB_DESC_ENDPOINT       5
#define USB_DESC_DEVICE_QUALIFIER   6
#define USB_DESC_OTHER_SPEED_CONFIGURATION  7
#define USB_DESC_INTERFACE_POWER    8

#define USB_SPEC_1_1    0x0110
#define USB_SPEC_2_0    0x0200

struct usb_device_desc {
    uint8_t     bLength;            // Size of this descriptor in bytes
    uint8_t     bDescriptorType;    // DEVICE Descriptor Type
    uint16_t    bcdUSB;             // USB Specification Release Number in BCD
    uint8_t     bDeviceClass;       // Class code
    uint8_t     bDeviceSubClass;    // Subclass code
    uint8_t     bDeviceProtocol;    // Protocol code
    uint8_t     bMaxPacketSize0;    // Maximum packet size for endpoint zero (only 8, 16, 32 or 64)
    uint16_t    idVendor;           // Vendor ID
    uint16_t    idProduct;          // Product ID
    uint16_t    bcdDevice;          // Device release number in BCD
    uint8_t     iManufacturer;      // Index of string descriptor describing manufacturer
    uint8_t     iProduct;           // Index of string descriptor describing product
    uint8_t     iSerialNumber;      // Index of string descriptor describing the device's serial number
    uint8_t     bNumConfigurations; // Number of possible configurations
};

struct usb_device_qualifier {
    uint8_t     bLength;            // Size of descriptor
    uint8_t     bDescriptorType;    // Device Qualifier Type
    uint16_t    bcdUSB;             // USB specification version number
    uint8_t     bDeviceClass;       // Class code
    uint8_t     bDeviceSubClass;    // Subclass code
    uint8_t     bDeviceProtocol;    // Protocol code
    uint8_t     bMaxPacketSize0;    // Maximum packet size for other speed
    uint8_t     bNumConfigurations; // Number of Other-speed Configurations
    uint8_t     bReserved;          // Reserved for future use, must be zero
};


#define USB_CONFIG_ATTR_SELFPOWERED     0x40
#define USB_CONFIG_ATTR_REMOTEWAKEUP    0x20
#define USB_CONFIG_ATTRIBUTES(_flags)       (0x80 | _flags)
struct usb_configuration_desc {
    uint8_t     bLength;            // Size of this descriptor in bytes
    uint8_t     bDescriptorType;    // CONFIGURATION Descriptor Type
    uint16_t    wTotalLength;       // Total length of data returned for this configuration.
    uint8_t     bNumInterfaces;     // Number of interfaces supported by this configuration.
    uint8_t     bConfigurationValue;    // Value to use as an argument to the SetConfiguration() request to select this config.
    uint8_t     iConfiguration;     // Index of string descriptor describing this configuration.
    uint8_t     bmAttributes;       // Configuration characteristics
    uint8_t     bMaxPower;          // Maximum power consumption of USB device (in 2mA units).
};

struct usb_other_speed_config_desc {
    uint8_t     bLength;            // Size of this descriptor in bytes
    uint8_t     bDescriptorType;    // CONFIGURATION Descriptor Type
    uint16_t    wTotalLength;       // Total length of data returned for this configuration.
    uint8_t     bNumInterfaces;     // Number of interfaces supported by this configuration.
    uint8_t     bConfigurationValue;    // Value to use as an argument to the SetConfiguration() request to select this config.
    uint8_t     iConfiguration;     // Index of string descriptor describing this configuration.
    uint8_t     bmAttributes;       // Configuration characteristics
    uint8_t     bMaxPower;          // Maximum power consumption of USB device (in 2mA units).
};

struct usb_interface_desc {
    uint8_t     bLength;            // Size of this descriptor in bytes
    uint8_t     bDescriptorType;    // INTERFACE Descriptor Type
    uint8_t     bInterfaceNumber;   // Number of this interface
    uint8_t     bAlternateSetting;  // Value used to select this alternate setting for this interface.
    uint8_t     bNumEndpoints;      // Number of endpoints (exluding endpoint zero)
    uint8_t     bInterfaceClass;    // Class code.
    uint8_t     bInterfaceSubClass; // Subclass code
    uint8_t     bInterfaceProtocoll;    // Protocol code.
    uint8_t     iInterface;         // Index of string descriptor describing this interface.
};

#define USB_ENDPOINT_ADDR_IN  0x80
#define USB_ENDPOINT_ADDR_OUT 0x00

#define USB_ENDPOINT_TYPE_CONTROL     0
#define USB_ENDPOINT_TYPE_ISOCHRONOUS 1
#define USB_ENDPOINT_TYPE_BULK        2
#define USB_ENDPOINT_TYPE_INTERRUPT   3

#define USB_ENDPOINT_ISO_SYNC_NONE  0
#define USB_ENDPOINT_ISO_SYNC_ASYNC 1
#define USB_ENDPOINT_ISO_SYNC_ADAPTIVE 2
#define USB_ENDPOINT_ISO_SYNC_SYNC  3

#define USB_ENDPOINT_ISO_USAGE_DATA 0
#define USB_ENDPOINT_ISO_USAGE_FEEDBACK 1
#define USB_ENDPOINT_ISO_USAGE_IMPLFEEDBACK 2

#define USB_ENDPOINT_ATTRIBUTES(_typ,_sync,_usage) (((_usage)<<4)|((_sync)<<2)|(_typ))
struct usb_endpoint_desc {
    uint8_t     bLength;            // Size of this descriptor in bytes
    uint8_t     bDescriptorType;    // ENDPOINT Descriptor Type
    uint8_t     bEndpointAddress;   // The address of the endpoint.
    uint8_t     bmAttributes;       // Endpoint attributes.
    uint16_t    wMaxPacketSize;     // Maximum packet size of this endpoint.
    uint8_t     bInterval;          // Interval for polling endpoint for data transfers.
};

struct usb_string_desc {
    uint8_t     bLength;
    uint8_t     bDescriptorType;
};


#define USB_DEF_STR_CHARS_P(_n,_x,_s) const struct { struct usb_string_desc d; wchar_t b[]; } _n PROGMEM = \
                            {{sizeof(struct usb_string_desc)+(_s), USB_DESC_STRING}, _x };
#define USB_DEF_STR_P(_n,_x) USB_DEF_STR_CHARS_P(_n,_x,sizeof(_x)-2)

#define USB_DEF_STR_CHARS(_n,_x,_s) const struct { struct usb_string_desc d; wchar_t b[]; } _n = \
                            {{sizeof(struct usb_string_desc)+(_s), USB_DESC_STRING}, _x };
#define USB_DEF_STR(_n,_x) USB_DEF_STR_CHARS(_n,_x,sizeof(_x)-2)


#define USB_REQTYPE_HOST2DEV       0x00
#define USB_REQTYPE_DEV2HOST       (1<<7)
#define USB_REQTYPE_TYP_STANDARD   (0<<5)
#define USB_REQTYPE_TYP_CLASS      (1<<5)
#define USB_REQTYPE_TYP_VENDOR     (2<<5)
#define USB_REQTYPE_TYP_mask       0x60
#define USB_REQTYPE_REC_DEVICE     0
#define USB_REQTYPE_REC_INTERFACE  1
#define USB_REQTYPE_REC_ENDPOINT   2
#define USB_REQTYPE_REC_OTHER      3
#define USB_REQTYPE_REC_mask       0x1f

#define USB_REQUEST_GET_STATUS      0
#define USB_REQUEST_CLEAR_FEATURE   1
#define USB_REQUEST_SET_FEATURE     3
#define USB_REQUEST_SET_ADDRESS     5
#define USB_REQUEST_GET_DESCRIPTOR  6
#define USB_REQUEST_SET_DESCRIPTOR  7
#define USB_REQUEST_GET_CONFIGURATION   8
#define USB_REQUEST_SET_CONFIGURATION   9
#define USB_REQUEST_GET_INTERFACE   10
#define USB_REQUEST_SET_INTERFACE   11
#define USB_REQUEST_SYNCH_FRAME     12

#define USB_FEATURE_SEL_DEVICE_REMOTE_WAKEUP    1
#define USB_FEATURE_SEL_ENDPOINT_HALT           0
#define USB_FEATURE_SEL_TEST_MODE               2

struct usb_device_request {
    uint8_t     bmRequestType;      // Request type.
    uint8_t     bRequest;           // Specific request.
    uint16_t    wValue;             // Word-sized field that varies according to request.
    uint16_t    wIndex;             // Word-sized field that varies according to request.
    uint16_t    wLength;            // Number of bytes to transfer if there is a Data stage.
};

#define USB_EP_SIZE_8   0
#define USB_EP_SIZE_16  1
#define USB_EP_SIZE_32  2
#define USB_EP_SIZE_64  3

#define USB_EP_BANKS_1  0
#define USB_EP_BANKS_2  1


#define USB_DEVICE_STATE_NONE       0x00
#define USB_DEVICE_STATE_ATTACHED   0x01
#define USB_DEVICE_STATE_POWERED    0x02
#define USB_DEVICE_STATE_DEFAULT    0x04
#define USB_DEVICE_STATE_ADDRESS    0x08
#define USB_DEVICE_STATE_CONFIGURED 0x10
#define USB_DEVICE_STATE_SUSPENDED  0x20


#define USB_MEM_SRC_RAM     0
#define USB_MEM_SRC_PROGMEM 1
#define USB_MEM_SRC_EEPROM  2

typedef uint8_t (*get_string_func_t)(struct usb_device_request*, uint8_t);

struct usb_dev {
    volatile uint8_t state;
    uint8_t cur_config;
    const struct usb_device_desc* pdesc_dev;
    const void* pdesc_config;
    uint8_t desc_config_size;
    const void* const* pdesc_string;
    uint8_t desc_string_num;
    get_string_func_t get_string_func;
    uint8_t desc_dtype;
};

extern struct usb_dev g_usb_dev;

#define USB_RC_OK           0
#define USB_RC_NEW_SETUP    1
#define USB_RC_NEW_OUT      2
#define USB_RC_STATE_ABORT  3

// event declarations
void usb_event_set_configuration(void);
uint8_t usb_event_ep0_request(struct usb_device_request*);



static inline uint8_t usb_ep_isSETUP(void) {
    return bit_is_set(UEINTX, RXSTPI);
}
static inline void usb_ep_clearSETUP(void) {
    clrbit(UEINTX, RXSTPI);
}
uint8_t inline usb_ep_isIN(void) {
    return bit_is_set(UEINTX, TXINI);
}
static inline void usb_ep_clearIN(void) {
    clrbit(UEINTX, TXINI);
}
static inline void usb_ep_clearFIFOCON(void) {
    clrbit(UEINTX, FIFOCON);
}
static inline uint8_t usb_ep_isOUT(void) {
    return bit_is_set(UEINTX, RXOUTI);
}
static inline void usb_ep_clearOUT(void) {
    clrbit(UEINTX, RXOUTI);
}
static inline void usb_ep_set_stall(void) {
    setbit(UECONX, STALLRQ);
}

static inline uint8_t usb_req_isOUT(struct usb_device_request* ur) {
    return (ur->bmRequestType & 0x80) == USB_REQTYPE_HOST2DEV;
}
static inline uint8_t usb_req_isIN(struct usb_device_request* ur) {
    return (ur->bmRequestType & 0x80) == USB_REQTYPE_DEV2HOST;
}


void usb_ep_read_data(void* buf, uint8_t size);
uint8_t usb_ep_waitIN(void);
uint8_t usb_ep_send_zlp(void);
uint8_t usb_mem_read_byte(const void* psrc, uint8_t srctype);
uint8_t usb_ep_getbufsize(void);
uint8_t usb_ep_send_in_data(const void* psrc, uint16_t size, uint8_t srctype);
uint8_t usb_ep_send_control_data_chunk(const void* psrc, uint16_t size, uint8_t srctype);
uint8_t usb_ep_send_control_data_finish(void);
uint8_t usb_ep_send_control_data(const void* psrc, uint16_t size, uint8_t srctype, uint16_t reqsize);
uint8_t usb_ep_recv_control_data_chunk(void* psrc, uint16_t* psize, uint16_t size);
uint8_t usb_ep_recv_control_data_finish(void);
uint8_t usb_ep_recv_control_data(void* psrc, uint16_t* psize, uint16_t reqsize);
void usb_ep0_task(void);

#ifdef __cplusplus
}
#endif

#endif // _AVRUTILSLIB_USB_
