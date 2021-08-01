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



#ifndef _AVRUTILSLIB_SBOXNET_STRUCT_
#define _AVRUTILSLIB_SBOXNET_STRUCT_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SBOXNET_ERR_TMIT_NOTFREE 2

#define SBOXNET_STATE_NONE       0
#define SBOXNET_STATE_BACKOFF    1
#define SBOXNET_STATE_COLLISION  2
#define SBOXNET_STATE_TRANSMIT   3
#define SBOXNET_STATE_COLLCHECK  4
    
#define SBOXNET_FLAG_SNIFFER_b   0
#define SBOXNET_FLAG_RBUFOV_b    1    
#define SBOXNET_FLAG_WBUFOV_b    2
#define SBOXNET_FLAG_TEST_b      7

#define SBOXNET_ADDR_BROADCAST  255

struct sboxnet_msg_header {
    uint8_t dstaddr;
    uint8_t srcaddr;
    uint8_t seq;
    struct opt {
        unsigned len   : 6;
        unsigned flags : 2;
    }       opt;
    uint8_t cmd;
    uint8_t data[0];
} __attribute__ ((packed));

#define SBOXNET_MSG_HEADER_LEN  5
#define SBOXNET_MSG_MIN_LEN  (SBOXNET_MSG_HEADER_LEN+1)
#define SBOXNET_MSG_DATA_LEN    63
#define SBOXNET_MSG_MAX_LEN (SBOXNET_MSG_DATA_LEN+SBOXNET_MSG_MIN_LEN)
#define SBOXNET_MSG_DATA_LEN_MASK 0x3f

struct sboxnet_msg_max {
    struct sboxnet_msg_header msgh;
    uint8_t data[SBOXNET_MSG_DATA_LEN+1];
};


struct sboxnet_debug_info {
    uint16_t recv_byte;
    uint16_t recv_msg_got;
    uint16_t recv_msg_fetched;
    uint16_t tmit_byte;
    uint16_t tmit_msg_tosend;
    uint16_t tmit_msg_sent;
    uint16_t collisions;
    uint16_t coll_rxc;
    uint16_t coll_fe;
    uint16_t coll_udr;
    uint16_t retries;
    uint16_t recverrors;
    uint16_t recverr_fe;
    uint16_t recverr_pe;
    uint16_t recverr_dor;
    uint16_t recverr_proto;
    uint16_t recverr_size;
    uint16_t crcerrors;
    uint16_t tmiterrors;
};



/* protocoll flow:
 * 
 * server: SBOXNET_CMD_NET_RESET (0x01)
 * 
 * every client: SBOXNET_CMD_DEV_REQ_ADDR (0x10)
 * 
 * client answer: SBOXNET_CMD_DEV_REQ_ADDR (0x90): len:9 puid32 productid16 vendorid16 addr8
 * 
 * server: SBOXNET_CMD_DEV_GET_DESC (0x12):  len:0|1 strid (seq=2)
 * client answer:  SBOXNET_CMD_DEV_GET_DESC (0x92): len:n n*data8 (seq=2)
 * server:  SBOXNET_CMD_DEV_GET_DESC (0x12):  len:0|1 strid (seq=3)
 * client answer:  SBOXNET_CMD_DEV_GET_DESC (0x92): len:n n*data8 (seq=3)
 */

// SBOXNET Commands

#define SBOXNET_PROTOCOL_VERSION        0x0101
// System Commands:
// reset. clients must request a new address --> answer SBOXNET_CMD_DEV_REQ_ADDR
// destination: single client or broadcast
#define SBOXNET_CMD_NET_RESET            0x01
// watchdog: len:0  sent every 300ms by the master (srcaddr 0)
// destination: broadcast
#define SBOXNET_CMD_NET_WATCHDOG         0x02

// register device: len:8 puid32 productid16 vendorid16 -> len:9 puid32 productid16 vendorid16 addr8
#define SBOXNET_CMD_DEV_REQ_ADDR         0x10
// set address: len:9 puid32 productid16 vendorid16 addr8   -->  len:0
#define SBOXNET_CMD_DEV_SET_ADDR         0x11
// get device desc: Description len:0|1 strid -->  len:n n*data8
#define SBOXNET_CMD_DEV_GET_DESC         0x12
// set device desc: len:1+n strid n*data8 -> len:0
#define SBOXNET_CMD_DEV_SET_DESC         0x13
// device reset: len:0
#define SBOXNET_CMD_DEV_RESET            0x15
// device identify: len:1 state8 -> len:0       state8: bit0:on
#define SBOXNET_CMD_DEV_IDENTIFY         0x16
// start firmware update mode: len:3+32 size16 bitflags:0=bootloader(0)/firmware(1) firmwareheader  -->  len:0
#define SBOXNET_CMD_DEV_FW_UPD_START     0x18
// download firmware:  len:3+n 16bit-addr bitflags n * data8  --> len:0
#define SBOXNET_CMD_DEV_FW_UPD_LOAD      0x19

// Register Commands
// read register: len:3 reg16 len8   --> len:2*len8 len8*databyte16
#define SBOXNET_CMD_REG_READ             0x20
// read register multi: len:n*2 n*reg16   --> len:n*2 n*data16
#define SBOXNET_CMD_REG_READ_MULTI       0x21
// write register: len:4|6 reg16 data16 [mask16] .. --> len:0
#define SBOXNET_CMD_REG_WRITE            0x22
// set or clear a bit in a register: len:3 reg16 0xb000nnnn  (b: bit value, n: bit number) -> len:0
#define SBOXNET_CMD_REG_WRITE_BIT        0x23

// send feedback: changed sensors -> len:2 sensor16
#define SBOXNET_CMD_FB_CHANGED            0x30
// send feedback: loco addr: -> len:3 onnnnnnn (o:on/off n:sensor number) locoaddr16
#define SBOXNET_CMD_FB_LOCOADDR           0x31

// loco: power on:  len:1 000000ko  ; k: keep loco list, o: on/off 
#define SBOXNET_CMD_LOCO_POWER       0x60
// loco: drive cmd: len:3-5 addr16 Dfffffff [F7-0 F16-8]  ; D: 1=forward, f: speed 0-127
#define SBOXNET_CMD_LOCO_DRIVE       0x61
// loco: function:  len:4 addr16 F7-0 F16-8
#define SBOXNET_CMD_LOCO_FUNC        0x62
// loco: add: len:3 addr16 flags8
#define SBOXNET_CMD_LOCO_ADD         0x63
// loco: remove: len:2 addr16
#define SBOXNET_CMD_LOCO_DEL         0x64
// loco: POM: len:5 addr16 cv16 data8
#define SBOXNET_CMD_LOCO_POM         0x65


// SBOXNET_ACKRC_* range: 0..127 (!)
#define SBOXNET_ACKRC_OK             0
#define SBOXNET_ACKRC_ERROR          1
#define SBOXNET_ACKRC_CMD_UNKNOWN    2
#define SBOXNET_ACKRC_INVALID_ARG    3
#define SBOXNET_ACKRC_REG_INVALID    4

#define SBOXNET_ACKRC_LOCO_INVADDR   10
#define SBOXNET_ACKRC_LOCO_NOSLOT    11
#define SBOXNET_ACKRC_LOCO_ADDRINUSE 12
#define SBOXNET_ACKRC_LOCO_NOTFOUND  13
#define SBOXNET_ACKRC_LOCO_NOTAUS    14
#define SBOXNET_ACKRC_LOCO_BUSY      15

#define SBOXNET_ACKRC_FWUP_ACTIVE    100
#define SBOXNET_ACKRC_FWUP_INACTIVE  101
#define SBOXNET_ACKRC_FWUP_OVERFLOW  102
#define SBOXNET_ACKRC_FWUP_INVADDR   103
#define SBOXNET_ACKRC_FWUP_INVDEV    104
#define SBOXNET_ACKRC_FWUP_INVMODE   105
#define SBOXNET_ACKRC_FWUP_VERIFY_FAILED    106
#define SBOXNET_ACKRC_FWUP_INVCRC    107
#define SBOXNET_ACKRC_FWUP_INVBLDR   108

// only for internal usage
#define SBOXNET_ACKRC_FWUP_FINISHED  254
#define SBOXNET_ACKRC_SEND_NO_ANSWER 255


#define SBOXNET_ERR_RECVMSG_NOMSG    (-1)
#define SBOXNET_ERR_RECVMSG_CRC      (-2)
#define SBOXNET_ERR_RECVMSG_TOLONG   (-3)
#define SBOXNET_ERR_RECVMSG_INVLEN   (-4)

#ifdef __cplusplus
}
#endif

#endif // _AVRUTILSLIB_SBOXNET_STRUCT_
