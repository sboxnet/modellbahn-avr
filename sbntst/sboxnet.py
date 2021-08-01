
import sys
import array
import time
import math
import re
import usb.core
import usb.util
import threading
from datetime import datetime
from datetime import timedelta
import traceback
import inspect
import threading
#import pudb
#from pudb import set_trace; set_trace()

MSG_MAX_LEN = 69

# System Commands:
# reset. clients must request a new address --> answer SBOXNET_CMD_DEV_REQ_ADDR
# destination: single client or broadcast
SBOXNET_CMD_NET_RESET = 0x01
# watchdog: len:0  sent every 200ms by the master (srcaddr 0)
# destination: broadcast
SBOXNET_CMD_NET_WATCHDOG = 0x02

# register device: len:8 puid32 productid16 vendorid16 -> len:9 puid32 productid16 vendorid16 addr8
SBOXNET_CMD_DEV_REQ_ADDR = 0x10
# set address: len:9 puid32 productid16 vendorid16 addr8   -->  len:0
SBOXNET_CMD_DEV_SET_ADDR = 0x11
# get device desc: len:0|1 strid   -->  len:n n*data8
SBOXNET_CMD_DEV_GET_DESC = 0x12
# set device desc: len:1+n strid n*data8 -> len:0
SBOXNET_CMD_DEV_SET_DESC = 0x13
# device reset: len:0
SBOXNET_CMD_DEV_RESET = 0x15
# device identify: len:1 state8 -> len:0       state8: bit0:on
SBOXNET_CMD_DEV_IDENTIFY = 0x16
# start firmware update mode: len:7 productid16 vendorid16 size16
# bitflags:0=bootloader(0)/firmware(1) -->  len:0
SBOXNET_CMD_DEV_FW_UPD_START = 0x18
# download firmware:  len:7+n productid16 vendorid16 16bit-addr bitflags n * data8  --> len:0
SBOXNET_CMD_DEV_FW_UPD_LOAD = 0x19

# Register Commands
# read register: len:3 reg16 len8   --> len:2*len8 len8*databyte16
SBOXNET_CMD_REG_READ = 0x20
# read register multi: len:n*2 n*reg16   --> len:n*2 n*data16
SBOXNET_CMD_REG_READ_MULTI = 0x21
# write register: len:4|6 reg16 data16 [mask16] .. --> len:0
SBOXNET_CMD_REG_WRITE = 0x22
# set or clear a bit in a register: len:3 reg16 0xb000nnnn  (b: bit value, n: bit number) -> len:0
SBOXNET_CMD_REG_WRITE_BIT = 0x23

# send feedback: changed sensors -> len:2 sensor16
SBOXNET_CMD_FB_CHANGED = 0x30
# send feedback: loco addr: -> len:4 sensor16 locoaddr16
SBOXNET_CMD_FB_LOCOADDR = 0x31

# loco: power on: len:1 000000ko  ; k: keep loco list, o: on/off
SBOXNET_CMD_LOCO_POWER = 0x60
# loco: drive cmd: len:3-5 addr16 Dfffffff [F7-0 F16-8]  ; D: 1=forward, f: speed 0-127
SBOXNET_CMD_LOCO_DRIVE = 0x61
# loco: function: len:4 addr16 F7-0 F16-8
SBOXNET_CMD_LOCO_FUNC = 0x62
# loco: add: len:3 addr16 flags8
SBOXNET_CMD_LOCO_ADD = 0x63
# loco: remove: len:2 addr16
SBOXNET_CMD_LOCO_DEL = 0x64
# loco: POM: len:5 addr16 cv16 data8
SBOXNET_CMD_LOCO_POM = 0x65

# Command Flow
#
# server: SBOXNET_CMD_NET_RESET --> client: SBOXNET_CMD_DEV_REQ_ADDR
# server: SBOXNET_CMD_DEV_SET_ADDR --> None


R_FB_VALUE0 = 21
R_CNTRL_VALUE0 = 31

LOCO_FLG_SPEED_14 = 0
LOCO_FLG_SPEED_28 = 1
LOCO_FLG_SPEED_128 = 2


CMD_STR = {
    SBOXNET_CMD_NET_RESET:"SBOXNET_CMD_NET_RESET",
    SBOXNET_CMD_NET_WATCHDOG:"SBOXNET_CMD_NET_WATCHDOG",
    SBOXNET_CMD_DEV_REQ_ADDR:"SBOXNET_CMD_DEV_REQ_ADDR",
    SBOXNET_CMD_DEV_SET_ADDR:"SBOXNET_CMD_DEV_SET_ADDR",
    SBOXNET_CMD_DEV_GET_DESC:"SBOXNET_CMD_DEV_GET_DESC",
    SBOXNET_CMD_DEV_SET_DESC:"SBOXNET_CMD_DEV_SET_DESC",
    SBOXNET_CMD_DEV_RESET:"SBOXNET_CMD_DEV_RESET",
    SBOXNET_CMD_DEV_IDENTIFY:"SBOXNET_CMD_DEV_IDENTIFY",
    SBOXNET_CMD_DEV_FW_UPD_START:"SBOXNET_CMD_DEV_FW_UPD_START",
    SBOXNET_CMD_DEV_FW_UPD_LOAD:"SBOXNET_CMD_DEV_FW_UPD_LOAD",
    SBOXNET_CMD_REG_READ:"SBOXNET_CMD_REG_READ",
    SBOXNET_CMD_REG_READ_MULTI:"SBOXNET_CMD_REG_READ_MULTI",
    SBOXNET_CMD_REG_WRITE:"SBOXNET_CMD_REG_WRITE",
    SBOXNET_CMD_REG_WRITE_BIT:"SBOXNET_CMD_REG_WRITE_BIT",
    SBOXNET_CMD_FB_CHANGED:"SBOXNET_CMD_FB_CHANGED",
    SBOXNET_CMD_FB_LOCOADDR:"SBOXNET_CMD_FB_LOCOADDR",
    SBOXNET_CMD_LOCO_POWER:"SBOXNET_CMD_LOCO_POWER",
    SBOXNET_CMD_LOCO_DRIVE:"SBOXNET_CMD_LOCO_DRIVE",
    SBOXNET_CMD_LOCO_FUNC:"SBOXNET_CMD_LOCO_FUNC",
    SBOXNET_CMD_LOCO_ADD:"SBOXNET_CMD_LOCO_ADD",
    SBOXNET_CMD_LOCO_DEL:"SBOXNET_CMD_LOCO_DEL",
    SBOXNET_CMD_LOCO_POM:"SBOXNET_CMD_LOCO_POM"
}

SBOXNET_ACKRC_OK = 0
SBOXNET_ACKRC_ERROR = 1
SBOXNET_ACKRC_CMD_UNKNOWN = 2
SBOXNET_ACKRC_INVALID_ARG = 3
SBOXNET_ACKRC_REG_INVALID = 4

SBOXNET_ACKRC_LOCO_INVADDR = 10
SBOXNET_ACKRC_LOCO_NOSLOT = 11
SBOXNET_ACKRC_LOCO_ADDRINUSE = 12
SBOXNET_ACKRC_LOCO_NOTFOUND = 13
SBOXNET_ACKRC_LOCO_NOTAUS = 14
SBOXNET_ACKRC_LOCO_BUSY = 15

SBOXNET_ACKRC_FWUP_ACTIVE = 100
SBOXNET_ACKRC_FWUP_INACTIVE = 101
SBOXNET_ACKRC_FWUP_OVERFLOW = 102
SBOXNET_ACKRC_FWUP_INVADDR = 103
SBOXNET_ACKRC_FWUP_INVDEV = 104
SBOXNET_ACKRC_FWUP_INVMODE = 105
SBOXNET_ACKRC_FWUP_VERIFY_FAILED = 106
SBOXNET_ACKRC_FWUP_INVCRC = 107
SBOXNET_ACKRC_FWUP_INVBLDR = 108


ACKRC_STR = {
    SBOXNET_ACKRC_OK:"SBOXNET_ACKRC_OK",
    SBOXNET_ACKRC_ERROR:"SBOXNET_ACKRC_ERROR",
    SBOXNET_ACKRC_CMD_UNKNOWN:"SBOXNET_ACKRC_CMD_UNKNOWN",
    SBOXNET_ACKRC_INVALID_ARG:"SBOXNET_ACKRC_INVALID_ARG",
    SBOXNET_ACKRC_REG_INVALID:"SBOXNET_ACKRC_REG_INVALID",
    SBOXNET_ACKRC_LOCO_INVADDR:"SBOXNET_ACKRC_LOCO_INVADDR",
    SBOXNET_ACKRC_LOCO_NOSLOT:"SBOXNET_ACKRC_LOCO_NOSLOT",
    SBOXNET_ACKRC_LOCO_ADDRINUSE:"SBOXNET_ACKRC_LOCO_ADDRINUSE",
    SBOXNET_ACKRC_LOCO_NOTFOUND:"SBOXNET_ACKRC_LOCO_NOTFOUND",
    SBOXNET_ACKRC_LOCO_NOTAUS:"SBOXNET_ACKRC_LOCO_NOTAUS",
    SBOXNET_ACKRC_LOCO_BUSY:"SBOXNET_ACKRC_LOCO_BUSY",
    SBOXNET_ACKRC_FWUP_ACTIVE:"SBOXNET_ACKRC_FWUP_ACTIVE",
    SBOXNET_ACKRC_FWUP_INACTIVE:"SBOXNET_ACKRC_FWUP_INACTIVE",
    SBOXNET_ACKRC_FWUP_OVERFLOW:"SBOXNET_ACKRC_FWUP_OVERFLOW",
    SBOXNET_ACKRC_FWUP_INVADDR:"SBOXNET_ACKRC_FWUP_INVADDR",
    SBOXNET_ACKRC_FWUP_INVDEV:"SBOXNET_ACKRC_FWUP_INVDEV",
    SBOXNET_ACKRC_FWUP_INVMODE:"SBOXNET_ACKRC_FWUP_INVMODE",
    SBOXNET_ACKRC_FWUP_VERIFY_FAILED:"SBOXNET_ACKRC_FWUP_VERIFY_FAILED",
    SBOXNET_ACKRC_FWUP_INVCRC:"SBOXNET_ACKRC_FWUP_INVCRC",
    SBOXNET_ACKRC_FWUP_INVBLDR:"SBOXNET_ACKRC_FWUP_INVBLDR"
}

SBOXNET_ERR_RECVMSG_NOMSG = -1
SBOXNET_ERR_RECVMSG_CRC = -2
SBOXNET_ERR_RECVMSG_TOLONG = -3
SBOXNET_ERR_RECVMSG_INVLEN = -4



def cmd_to_str(cmd):
    if cmd in CMD_STR:
        return CMD_STR[cmd]
    return "UNKNOWN"

def ackrc_to_str(rc):
    if rc in ACKRC_STR:
        return ACKRC_STR[rc]
    return "UNKNOWN"

def byte2int(b):
    b = int(b) & 255
    return (-(256-b)) if (b > 127) else b

def makeword(v1, v0):
    return ((v1 & 0xff) << 8) + (v0 & 0xff)
def makelong(v3, v2, v1, v0):
    return ((v3 & 0xff) << 24) + ((v2 & 0xff) << 16) + ((v1 & 0xff) << 8) + (v0 & 0xff)

def makeworda(arr, idx):
    return makeword(arr[idx+1], arr[idx])
def makelonga(arr, idx):
    return makelong(arr[idx+3], arr[idx+2], arr[idx+1], arr[idx])

def highbyte(v):
    return (v >> 8) & 0x00ff

def lowbyte(v):
    return v & 0x00ff

def log(o, h, s):
    if o:
        print('%s:[%s](%s)(%s)(%s.%s):%s:%s' % (datetime.now(), threading.currentThread().getName(), inspect.stack()[2][1], inspect.stack()[2][2], o.__class__.__name__, inspect.stack()[2][3], h, s))
    else:
        print('%s:(%s)(%s)(%s):%s:%s' % (datetime.now(), inspect.stack()[2][1], inspect.stack()[2][2], inspect.stack()[2][3], h, s))

def logInfo(o, s):
    log(o, 'INFO', s)

def logDebug(o, s):
    log(o, 'DEBUG', s)

def logError(o, s):
    log(o, 'ERROR', s)


class SboxnetDebugState(object):
    def __init__(self, arr):
        self.state = arr[0]
        self.flags = arr[1]
        self.retry_counter = arr[2]
        self.tmit_cnt = arr[3]
        self.tmit_bytes = arr[4]
        self.tmit_lastbyte = arr[5]
        self.backoff_bits = arr[6]
        self.recv_len = arr[7]
        self.prng_seed = arr[8]
    
    def __str__(self):
        return "".join([f"state: {self.state}\n",
                        f"flags: {self.flags}\n",
                        f"retry_counter: {self.retry_counter}\n",
                        f"tmit_cnt: {self.tmit_cnt}\n",
                        f"tmit_bytes: {self.tmit_bytes}\n",
                        f"tmit_lastbyte: {self.tmit_lastbyte}\n",
                        f"backoff_bits: {self.backoff_bits}\n",
                        f"recv_len: {self.recv_len}\n",
                        f"prng_seed: {self.prng_seed}\n",
                        ])

class SboxnetDebugInfo(object):
    def __init__(self, arr):
        self.recv_byte = makeworda(arr, 0)
        self.recv_msg_got = makeworda(arr, 2)
        self.recv_msg_fetched = makeworda(arr, 4)
        self.tmit_byte = makeworda(arr, 6)
        self.tmit_msg_tosend = makeworda(arr, 8)
        self.tmit_msg_sent = makeworda(arr, 10)
        self.collisions = makeworda(arr, 12)
        self.coll_rxc = makeworda(arr, 14)
        self.coll_fe = makeworda(arr, 16)
        self.coll_udr = makeworda(arr, 18)
        self.retries = makeworda(arr, 20)
        self.recverrors = makeworda(arr, 22)
        self.recverr_fe = makeworda(arr, 24)
        self.recverr_pe = makeworda(arr, 26)
        self.recverr_dor = makeworda(arr, 28)
        self.recverr_proto = makeworda(arr, 30)
        self.recverr_size = makeworda(arr, 32)
        self.crcerrors = makeworda(arr, 34)
        self.tmiterrors = makeworda(arr, 36)
    
    def __str__(self):
        return "".join([f"recv_byte: {self.recv_byte}\n",
        f"recv_msg_got: {self.recv_msg_got}\n",
        f"recv_msg_fetched: {self.recv_msg_fetched}\n",
        f"tmit_byte: {self.tmit_byte}\n",
        f"tmit_msg_tosend: {self.tmit_msg_tosend}\n",
        f"tmit_msg_sent: {self.tmit_msg_sent}\n",
        f"collisions: {self.collisions}\n",
        f"coll_rxc: {self.coll_rxc}\n",
        f"coll_fe: {self.coll_fe}\n",
        f"coll_udr: {self.coll_udr}\n",
        f"retries: {self.retries}\n",
        f"recverrors: {self.recverrors}\n",
        f"recverr_fe: {self.recverr_fe}\n",
        f"recverr_pe: {self.recverr_pe}\n",
        f"recverr_dor: {self.recverr_dor}\n",
        f"recverr_proto: {self.recverr_proto}\n",
        f"recverr_size: {self.recverr_size}\n",
        f"crcerrors: {self.crcerrors}\n",
        f"tmiterrors: {self.tmiterrors}\n"])

class SboxnetDebugRingBuf(object):
    def __init__(self, arr):
        self.rd = arr[0]
        self.rd_count = arr[1]
        self.rd_rollback = arr[2]
        self.wr = arr[3]
        self.wr_count = arr[4]
        self.wr_rollback = arr[5]
        self.count = arr[6]
        self.size = arr[7]
        self.buf = arr[8:]
    
    def __str__(self):
        return "".join([f"rd: {self.rd}\n",
                f"rd_count: {self.rd_count}\n",
                f"rd_rollback: {self.rd_rollback}\n",
                f"wr: {self.wr}\n",
                f"wr_count: {self.wr_count}\n",
                f"wr_rollback: {self.wr_rollback}\n",
                f"count: {self.count}\n",
                f"size: {self.size}\n",
                f"buf: {self.buf}\n"])

class SboxnetDebugStack(object):
    def __init__(self, arr):
        self.size = makeworda(arr, 0)
        self.free = makeworda(arr, 2)


class SboxnetMsg(object):
    dstaddr = 0
    srcaddr = 0
    seq = 0
    flags = 0
    dlen = 0
    cmd = 0
    data = []
    crc = 0

    # SboxnetMsg.
    def __init__(self):
        pass
    @classmethod
    def new_from_array(cls, arr):
        x = cls()
        if arr is not None:
            #print("sboxnetmsginit: %s" % (arr))
            x.dstaddr = arr[0]
            x.srcaddr = arr[1]
            x.seq = arr[2]
            x.dlen = arr[3] & 0x3f
            x.flags = arr[3] & 0xc0
            x.cmd = arr[4]
            x.data = arr[5:5+x.dlen]
            x.crc = arr[5+x.dlen]
        return x
    @classmethod
    def new(cls, dstaddr, cmd, seq, data=[]):
        x = cls()
        if dstaddr is not None:
            x.dstaddr = int(dstaddr)
        x.seq = seq
        x.flags = 0
        x.dlen = len(data)
        x.cmd = int(cmd)
        x.data = data
        return x

    def to_array(self):
        arr = array.array('B', [self.dstaddr, self.srcaddr, self.seq,
                                (self.flags & 0xc0)|(self.dlen & 0x3f), self.cmd])
        arr.extend(self.data[0:63])
        #arr.append(0) # crc
        return arr
    
    def print(self, o):
        s = cmd_to_str(self.cmd&0x7f) + " ("+ cmd_to_str(self.cmd&0x7f)+ f"  is_answer {self.cmd&0x80 == 0x80}) {('--> SEND' if (self.cmd&0x80== 0) else '<-- RECEIVE ')}| dstaddr={self.dstaddr} srcaddr={self.srcaddr} seq={self.seq} dlen={self.dlen} cmd={self.cmd} cmd_hex={self.cmd:x} data={self.data} crc={self.crc}"
        if o._debug: logDebug(self, s)
    
    def __str__(self):
        return cmd_to_str(self.cmd&0x7f) + " ("+ cmd_to_str(self.cmd&0x7f) + f"  is_answer {self.cmd&0x80 == 0x80}) {('--> SEND' if (self.cmd&0x80== 0) else '<-- RECEIVE ')}| dstaddr={self.dstaddr} srcaddr={self.srcaddr} seq={self.seq} dlen={self.dlen} cmd={self.cmd} cmd_hex={self.cmd:x} data={self.data} crc={self.crc}"

class SboxnetError(Exception):
    pass
        

# an usb object of the sboxnet device
class SboxnetUSB(object):
    VENDORID  = 0xf123
    PRODUCTID = 0xa002
    #
    CMD_SBOXNET_SET_MODE = 0x30
    CMD_SBOXNET_GET_STATUS = 0x31
    CMD_SBOXNET_RECEIVE = 0x32 # 50
    CMD_SBOXNET_TRANSMIT = 0x33 # 51
    CMD_SBOXNET_GET_DBGINFO = 0x3a # 58
    CMD_SBOXNET_GET_DBGSTATE = 0x3b # 59
    CMD_SBOXNET_GET_DBGRECVBUF = 0x3c # 60
    CMD_SBOXNET_GET_DBGTMITBUF = 0x3d # 61
    CMD_DBG_STACK = 0x40 # 64
    CMD_SET_SERIALNUMBER = 0x51 # 81
    #
    SBOXNET_FLG_ENABLED = 0x01
    SBOXNET_FLG_SNIFFER = 0x02
    SBOXNET_FLG_BOOTLOADER = 0x80
    #
    SBOXNET_STATUS_RX_MSG = 0x01
    SBOXNET_STATUS_RX_CANREAD = 0x02
    SBOXNET_STATUS_RX_CRC = 0x04
    SBOXNET_STATUS_TX_CANSEND = 0x10
    SBOXNET_STATUS_TX_ERR = 0x20
    SBOXNET_STATUS_TX_OVF = 0x40
    #
    # SboxnetUSB.
    def __init__(self, serialn=None, dev=None, sniffer=False, debug=False):

        #traceback.print_stack()
        self._lock = threading.Lock()
        self._lock.acquire()
        self._debug = debug
        if self._debug: logDebug(self, "serialn=%s dev=%s sniffer=%s debug=%s" % (serialn, dev, sniffer, debug))
        if dev is None:
            if serialn is None: # SerialNumber given?
                raise SboxnetError('NO device or serialnumber specified!')
            self._dev = None
            #xyz 
            # find all usb devices
            devlist = usb.core.find(idVendor=self.VENDORID, idProduct=self.PRODUCTID, find_all=True)
            if devlist is not None:                     # found
                for dev in devlist:                     # walk through dev list
                    #print("Vendor="+str(dev.idVendor())+" , Product="+str(dev.idProduct()))#+" Serial="+dev.getserialnumber())
                    # get serialstring of dev, see if it is the requested
                    #if SboxnetUSB(dev=dev).getserialnumber() == serialn:
                        self._dev = dev                             # YES
                        break
            if self._dev is None:
                raise SboxnetError('no USB device found! vendor:0x%x product:0x%x serialnumber:%s' % (self.VENDORID, self.PRODUCTID, serialn))
        else:
            self._dev = dev
        self._dev.set_configuration() # set first configuration
        self._sniffer = sniffer
        #self._debug = debug
        if self._debug: logDebug(self, f'exeuting with DEBUG')
    #
    def sniffer(self, on):
        #if self._debug: logDebug(self, "sniffer(%d)" % (on))
        self._sniffer = on
        
    # SboxnetUSB.ctrl: send a control request to sboxnet usb
    def ctrl(self, cdir, bRequest, wValue=0, wIndex=0, data_or_wLength = None, timeout = None):
        return self._dev.ctrl_transfer(cdir|usb.util.CTRL_TYPE_VENDOR|usb.util.CTRL_RECIPIENT_INTERFACE,
                                bRequest, wValue, wIndex, data_or_wLength, timeout)
    #
    # SboxnetUSB.reset: 
    def reset(self):
        #if self._debug: logDebug(self, "reset")
        self.ctrl(usb.util.CTRL_OUT, SboxnetUSB.CMD_SBOXNET_SET_MODE, SboxnetUSB.SBOXNET_FLG_BOOTLOADER, 0)
    #
    # SboxnetUSB.enable: send SET_MODE: ENABLED+flags
    def enable(self, devaddr=0, flags=0):
        if self._sniffer: flags = flags + 2
        #if self._debug: logDebug(self, "SboxnetUSB.enable(): devaddr=%d flags=%d" % (devaddr, flags))
        self.ctrl(usb.util.CTRL_OUT, SboxnetUSB.CMD_SBOXNET_SET_MODE, SboxnetUSB.SBOXNET_FLG_ENABLED|flags, devaddr)
    #
    # SboxnetUSB.disable() # disable sboxnet
    def disable(self):
        #if self._debug: logDebug(self, "disable")
        self.ctrl(usb.util.CTRL_OUT, SboxnetUSB.CMD_SBOXNET_SET_MODE, 0, 0)
    #
    #  SboxnetUSB.sendmsg(msg): send a msg to sboxnet, the msg is an array (see SboxnetMsg)
    def sendmsg(self, msg):
        #if self._debug: logDebug(self, "sendmsg")
        return self.ctrl(usb.util.CTRL_OUT, SboxnetUSB.CMD_SBOXNET_TRANSMIT, 0, 0, msg.to_array())
    #
    # SboxnetUSB.recvmsg(): recv a sboxnet msg, the msg is returend as an array (see SboxnetMsg)
    def recvmsg(self):
        #if self._debug: logDebug(self, "recvmsg")
        data = self.ctrl(usb.util.CTRL_IN, SboxnetUSB.CMD_SBOXNET_RECEIVE, 0, 0, 255)
        if len(data) == 0:
            return None
        st = data[0]
        msgs = []
        datalen = len(data)
        idx = 1
        while idx < datalen:
            msglen = byte2int(data[idx])
            idx = idx + 1
            #print("datalen=%d idx=%d msglen=%d"%(datalen,idx,msglen))
            if msglen > 0:
                msgs.append( {'msglen':msglen, 'msg':SboxnetMsg.new_from_array(data[idx:idx+msglen])} )
                idx = idx + msglen
            else:
                #print(data)
                msgs.append( {'msglen':msglen} )
                break
        return (st, msgs)   
    #
    #  SboxnetUSB.getstatus()
    def getstatus(self):
        #if self._debug: logDebug(self, "getstatus")
        data = self.ctrl(usb.util.CTRL_IN, SboxnetUSB.CMD_SBOXNET_GET_STATUS, 0, 0, 1)
        if len(data) != 1:
            raise SboxnetError('getstatus: return value len != 1')
        return (data[0] & 0x00ff)
    #
    # SboxnetUSB.getdbginfo(): returns a SboxnetDebugInfo object
    def getdbginfo(self):
        #if self._debug: logDebug(self, "getdbginfo")
        data = self.ctrl(usb.util.CTRL_IN, SboxnetUSB.CMD_SBOXNET_GET_DBGINFO, 0, 0, 39)
        if len(data) != 38:
            raise SboxnetError('getdbginfo: return value len != 38')
        return SboxnetDebugInfo(data)
    #
    # SboxnetUSB.getdbgstate(): returns a SboxnetDebugState object
    def getdbgstate(self):
        #if self._debug: logDebug(self, "getdbgstate")
        data = self.ctrl(usb.util.CTRL_IN, SboxnetUSB.CMD_SBOXNET_GET_DBGSTATE, 0, 0, 9)
        if len(data) != 9:
            raise SboxnetError('getdbgstate: return value len != 9')
        return SboxnetDebugState(data)
    #
    # SboxnetUSB.getdbgrecvbuf(): returns the receiver debug buffer as SboxnetDebugRingBuf object
    def getdbgrecvbuf(self):
        #if self._debug: logDebug(self, "getdbgrecvbuf")
        data = self.ctrl(usb.util.CTRL_IN, SboxnetUSB.CMD_SBOXNET_GET_DBGRECVBUF, 0, 0, 140+8+2)
        print(len(data))
        #if len(data) != 128+8:
        #    raise SboxnetError('getdbgrecvbuf: return value len != 128+8')
        return SboxnetDebugRingBuf(data)
    #
    # SboxnetUSB.getdbgtmitbuf(): returns the transmitter debug buffer as SboxnetDebugRingBuf object
    def getdbgtmitbuf(self):
        #if self._debug: logDebug(self, "getdbgtmitbuf")
        data = self.ctrl(usb.util.CTRL_IN, SboxnetUSB.CMD_SBOXNET_GET_DBGTMITBUF, 0, 0, 140+8+2)
        #if len(data) != 128+8:
        #    raise SboxnetError('getdbgtmitbuf: return value len != 128+8')
        return SboxnetDebugRingBuf(data)
    #
    #  SboxnetUSB.getdbgstack(): returns the stack info (size, free) as an SboxnetDebugStack object
    def getdbgstack(self):
        #if self._debug: logDebug(self, "getdbgstack")
        data = self.ctrl(usb.util.CTRL_IN, SboxnetUSB.CMD_DBG_STACK, 0, 0, 4)
        if len(data) != 4:
            raise SboxnetError('getdbgstack: return value len != 4')
        return SboxnetDebugStack(data)
    #
    # SboxnetUSB.getfwversion() : get the firmware Version as USB bcdDevice
    def getfwversion(self):
        #if self._debug: logDebug(self, "getfwversion")
        return self._dev.bcdDevice
        
    # SboxnetUSB.setserialnumber(sn): set the serialnumber as string
    def setserialnumber(self, sn):
        #if self._debug: logDebug(self, "setserialnumber")
        self.ctrl(usb.util.CTRL_OUT, SboxnetUSB.CMD_SET_SERIALNUMBER, 0, 0, sn[0:31].encode("utf-16-le"))
    
    # SboxnetUSB.getserialnumber(): get the serialnumber as string
    def getserialnumber(self):
        #if self._debug: logDebug(self, "getserialnumber")
        return usb.util.get_string(self._dev, self._dev.iSerialNumber, 0x0409) #32, 0x0409) # """""")

    @staticmethod
    def find_devices():
        #logDebug(None, "find_devices()")
        return usb.core.find(idVendor=SboxnetUSB.VENDORID, idProduct=SboxnetUSB.PRODUCTID, find_all=True)

# MapEintrag der die PUID (PseudoUID) mit Adresse verknüft.
class AddrMapEntry(object):
    puid = None
    productid = None
    vendorid = None
    addr = None
    desc0 = ""
    desc1 = ""
    
    def __str__(self):
        return f"PUID: 0x{self.puid:x} |PRODUCTID: 0x{self.productid:x} |VENDORID: 0x{self.vendorid:x} |ADDR: {self.addr} |DESC0: {self.desc0} |DESC1: {self.desc1}"

# Eine Map der Devices mit PUID (puid) und Adresse
class AddrMap(object):
    def __init__(self):
        # frei Adressen, begingend mit 1
        self._freeaddrlist = list(range(200, 0, -1))
        # AdressMap
        self._addrmap = dict()
        # Locking von AdressMap
        self._addrmaplock = threading.Lock()
    
    # AddrMap.add(ame)
    # Füge neue Adresse (ame) hinzu
    def add(self, ame):
        with self._addrmaplock:
            # ist FreeList leer ?
            if len(self._freeaddrlist) == 0:
                # keine weiteren Einträge erlaubt
                return False
            # hole nächste Adresse
            ame.addr = self._freeaddrlist.pop()
            # füge AdressMapEntry hinzu mit dieser Adresse 
            self._addrmap[ame.addr] = ame
        return True
    
    # AddrMap.find(ame)
    # suche eine Adresse mit PUID==ame.puid && PRODUCTID==ame.productid && VENDORID==ame.vendorid
    def find(self, ame):
        with self._addrmaplock:
            # Loop over List of Values
            for e in self._addrmap.values():
                # ist entry (e) puid == ame.puid && e.productid == ame.productid && e.vendorid == ame.vendorid
                if e.puid == ame.puid and e.productid == ame.productid and e.vendorid == ame.vendorid:
                    # wenn ja, dann AdressMapEintrag zurückliefern
                    return e
        # Nichts gefunden
        return None

    # AddrMap.find_addr(addr)
    # Finde eine Adresse (addr) in der AddrMap.
    def find_addr(self, addr):
        # hole Lock auf AddrMap
        with self._addrmaplock:
            # Loop over AddrMap values
            for e in self._addrmap.values():
                # ist Eintrag Adresse gleich der gesuchten?
                if addr == e.addr:
                    # Wenn ja, zurücklieren
                    return e
        # Nichts gefunden.
        return None
    
    # AddrMap.print_entries()
    # AddrMap Einträge listen
    def print_entries(self):
        for e in self._addrmap.values():
            logInfo(self, f"ADDR={e.addr} PUID=0x{e.puid:x} PRODUCTID=0x{e.productid:x} VENDORID=0x{e.vendorid:x}  {e.desc0},{e.desc1}")
    
    # AddrMap.get_puid_addr(puid)
    # Den AddrMap eintrag liefern für die PUID puid.
    def get_puid_addr(self, puid):
        with self._addrmaplock:
            for e in self._addrmap.values():
                if e.puid == puid:
                    return e.addr
        return None

    # AddrMap.get_addr_puid(addr)
    # Den AddrMap Eintrag liefern für Addresse addr.
    def get_addr_puid(self, addr):
        with self._addrmaplock:
            for e in self._addrmap.values():
                if e.addr == addr:
                    return e.puid
        return None
        
class SboxnetMaster(threading.Thread):
    # SboxnetMaster.
    def __init__(self, debug=None, serialn=None, sbnusb=None, cbobj=None):
        threading.Thread.__init__(self, name="Sboxnet")
        #if 1: logDebug(self, f"serialn={serialn} sbnusb={sbnusb} cbobj={cbobj}")
        self._sbnusb = sbnusb
        self._reset = True
        self._run = True
        self._recvmsglist = []
        self._tmitmsglist = []
        self._msglistlock = threading.Lock()
        self._addrmap = AddrMap()
        self.daemon = False
        self.cbobj_processmsg = cbobj
        self.laststatus = 0
        self._serialn = serialn
        self._debug = debug
    
    # SboxnetMaster.
    def sboxnet(self):
        return self._sbnusb
    
    # SboxnetMaster.
    def printEntries(self):
        self._addrmap.print_entries()
    
    # SboxnetMaster.
    def _process_msg(self, msg):
        if self._debug: logDebug(self, "== PROCESS MSG ==")
        if self._debug: logDebug(self, f"MSG.CMD: {msg.cmd & 0x7f} (0x{(msg.cmd&0xff):x}) : {cmd_to_str(msg.cmd & 0x7f)}")
        msg.print(self)
        if msg.cmd == SBOXNET_CMD_DEV_REQ_ADDR:
            # msg request addr
            # print this msg
            if self._debug: msg.print(self)
            if self._debug: logDebug(self, "process msg SBOXNET_CMD_DEV_REQ_ADDR")
            if msg.dlen != 8:
                logError(self, "ERROR: SBOXNET_CMD_DEV_REQ_ADDR wrong message format!")
                return True # discard msg
            
            # create addr map entry
            ame = AddrMapEntry()
            # add puid
            ame.puid = makelonga(msg.data, 0)
            # add productid
            ame.productid = makeworda(msg.data, 4)
            # add vendorid
            ame.vendorid = makeworda(msg.data, 6)
            if self._debug: logDebug(self, f"received ANWSER PUID=0x{ame.puid:x} ProdctID=0x{ame.productid:x} VendorID=0x{ame.vendorid:x}")
            # ist der eintrag schon da?
            f = self._addrmap.find(ame)
            if self._debug: logDebug(self, f"entry in ADDRMAP? {f}")
            if not f:
                if self._debug: logDebug(self, "+++++++++++++++++++++++++++++++++++++++++++")
                if self._debug: logDebug(self, f"no, add it")
                if self._debug: logDebug(self, "-------------------------------------------")
                # nein, add to addrmap
                if self._addrmap.add(ame):
                    f = ame
                else:
                    logError(self, "ERROR: failed to add a new Sboxnet Logon Entry!")
                    return True
            
            # sende SBOXNET_CMD_DEV_SET_ADDR
            if self._debug: logDebug(self, f"send SBOXNET_CMD_DEV_SET_ADDR ")
            logInfo(self, "-------------------------------------------------------")
            logInfo(self, f"ADDR={f.addr} PUID=0x{f.puid:x} ProductID=0x{f.productid:x} VendorID=0x{f.vendorid:x}")
            logInfo(self, "-------------------------------------------------------")
            x = msg.data[0:8]
            x.append(f.addr)
            # cls, dstaddr (255), cmd (SBOXNET_CMD_DEV_SET_ADDR), seq (0), data=[same as SBOXNET_CMD_DEV_REQ_ADDR + Address] 
            # src ist die Broadcast Adresse (255)
            nmsg = SboxnetMsg.new(255, SBOXNET_CMD_DEV_SET_ADDR, 0, x)
            self.sendmsg(nmsg)
            return True
        elif msg.cmd == SBOXNET_CMD_DEV_SET_ADDR|0x80:
            if self._debug: logDebug(self, f"(1) request SBOXNET_CMD_DEV_GET_DESC with seq=2 idx=0")
            self.sendmsg(SboxnetMsg.new(msg.srcaddr, SBOXNET_CMD_DEV_GET_DESC, 2, [0]))
            if self._debug: logDebug(self, f"(2) request SBOXNET_CMD_DEV_GET_DESC with seq=3 idx=1")
            self.sendmsg(SboxnetMsg.new(msg.srcaddr, SBOXNET_CMD_DEV_GET_DESC, 3, [1]))
        elif msg.cmd == SBOXNET_CMD_DEV_GET_DESC|0x80:
            if self._debug: logDebug(self, "==================================")
            if self._debug: logDebug(self, "CMD DEV GET_DESC_GET_DESC answer")
            if self._debug: logDebug(self, "__________________________________")
            #if self._debug: logDebug(self, msg.print(self))
            desc = bytes(msg.data[0:msg.dlen]).decode(encoding="ascii")
            ame = self._addrmap.find_addr(msg.srcaddr)
            if self._debug: logDebug(self, f"logon msg {ame}")
            if self._debug: logDebug(self, "++++ ame {ame}")
            if ame:
                if self._debug: logDebug(self, f"seq={msg.seq}")
                if self._debug: logDebug(self, f"addr={ame.addr} puid={ame.puid} ")
                if msg.seq == 2:
                    ame.desc0 = desc
                if msg.seq == 3:
                    ame.desc1 = desc
                    logInfo(self, "--------------")
                    logInfo(self, "LOGON: addr=%d puid=0x%x productid=0x%x vendorid=0x%x  %s, %s" % (ame.addr, ame.puid, ame.productid, ame.vendorid, ame.desc0, ame.desc1))
                    logInfo(self, "++++++++++++++")
        if self.cbobj_processmsg:
            return self.cbobj_processmsg.cb_processmsg(msg)
        return False
    
    # SboxnetMaster.
    def get_puid_addr(self, puid):
        return self._addrmap.get_puid_addr(puid)
    
    # SboxnetMaster.
    def get_addr_puid(self, addr):
        return self._addrmap.get_addr_puid(addr)
    
    # SboxnetMaster.
    def sendmsg(self, msg):
        with self._msglistlock:
            if msg.cmd != SBOXNET_CMD_NET_WATCHDOG:
                if self._debug: logDebug(self, "### send msg: ---")
                msg.print(self)
            self._tmitmsglist.append(msg)
            
    # SboxnetMaster.
    def recvmsg(self):
        with self._msglistlock:
            if len(self._recvmsglist) > 0:
                return self._recvmsglist.pop(0)
            return None
    
    # SboxnetMaster.
    def has_msg(self):
        with self._msglistlock:
            return len(self._recvmsglist) > 0
    
    # SboxnetMaster.
    def send_reset(self):
        self._reset = True
    
    #def start(self):
    #    pudb.set_trace()
        
    # SboxnetMaster.
    def run(self):
        #pudb.set_trace()
        if self._init_conn():
            self._serve()
            self._deinit_conn()
    
    # SboxnetMaster.
    def terminate(self):
        self._run = False
        
    # SboxnetMaster.
    def _serve(self):
        self._run = True
        self._reset = True
        watchdog_timer = datetime.now() + timedelta(milliseconds=100)
        while self._run:
            if self._sbnusb is None:
                with self._msglistlock:
                    self._init_conn()
                    time.sleep(1.0)
                    self._reset = True
            if self._sbnusb is not None:
                try:
                    if self._reset:
                        self._reset = False
                        with self._msglistlock:
                            self._recvmsglist = []
                            self._tmitmsglist = []
                            resetmsg = SboxnetMsg.new(255, SBOXNET_CMD_NET_RESET, 0)
                            self._tmitmsglist.append(resetmsg)
                            print("tmit reset")
                            #
                    d = datetime.now()
                    if watchdog_timer < d:
                        self.sendmsg(SboxnetMsg.new(255, SBOXNET_CMD_NET_WATCHDOG, 0))
                        watchdog_timer = d + timedelta(milliseconds=300)
                        #
                    try:
                        self.laststatus, msgs = self._sbnusb.recvmsg()
                        for rm in msgs:
                            msglen = rm['msglen']
                            #if self._debug: logDebug(self, "receive msg: rm")
                            if msglen < 0:
                                if msglen == SBOXNET_ERR_RECVMSG_NOMSG:
                                    pass
                                elif msglen == SBOXNET_ERR_RECVMSG_TOLONG:
                                    pass
                                else:
                                    print("--- recv error: %s" % (msglen))                                    
                            else:
                                rmsg = rm['msg']
                                if self._debug: logDebug(self, "-- ANSWER --------------------")
                                if self._debug: rmsg.print(self)
                                #if self._debug: logDebug(self, "receive msg: rmsg")
                                #if self._debug: print("--- recv: %s (0x%x) sa:%d da:%d s:%d l:%d" % (cmd_to_str(rmsg.cmd & 0x7f), rmsg.cmd, rmsg.srcaddr, rmsg.dstaddr, rmsg.seq, rmsg.dlen))
                                if self.cbobj_processmsg:
                                    self.cbobj_processmsg.cb_printmsg(rmsg)
                                if self._debug: logDebug(self, "************************")
                                if self._debug: logDebug(self, "** CALL _process_msg ***")
                                if self._debug: logDebug(self, "************************")
                                if not self._process_msg(rmsg):
                                    if self._debug: logDebug(self, "---- add to recvmsglist ------")
                                    if self._debug: rmsg.print(self)
                                    with self._msglistlock:
                                        if self._debug: logDebug(self, "____ APPEND to recvmsglist _______")
                                        rmsg.print(self)
                                        self._recvmsglist.append(rmsg)
                    except (SboxnetError) as e:
                        pass
                    #
                    if self.laststatus & SboxnetUSB.SBOXNET_STATUS_TX_CANSEND:
                        msg = None
                        with self._msglistlock:
                            if len(self._tmitmsglist) > 0:
                                msg = self._tmitmsglist.pop(0)
                        if msg:
                            #if msg.cmd != SBOXNET_CMD_NET_WATCHDOG:
                            #print("--- send: %s (0x%x) sa:%d da:%d s:%d l:%d" % (cmd_to_str(msg.cmd), msg.cmd, msg.srcaddr, msg.dstaddr, msg.seq, msg.dlen))
                            if self.cbobj_processmsg:
                                self.cbobj_processmsg.cb_printmsg(msg)
                            while True:
                                self._sbnusb.sendmsg(msg)
                                self.laststatus = self._sbnusb.getstatus()
                                if self.laststatus & (SboxnetUSB.SBOXNET_STATUS_TX_OVF|SboxnetUSB.SBOXNET_STATUS_TX_ERR|0x80):
                                    print("--- send: retry: 0x%x" % (self.laststatus))
                                else:
                                    break
                    time.sleep(0.001)
                     
                except (SboxnetError, usb.core.USBError) as e:
                    print(e)
                    self._deinit_conn()
                    self._run = False
                    with self._msglistlock:
                        time.sleep(1.0)

    # SboxnetMaster.
    def _init_conn(self):
        #print("4) starting\n")
        try:
            #print("5) --SboxnetUSB initialising")
            #print(self._sbnusb)
            #self._sbnusb = None
            if self._sbnusb is None:
                #logInfo(self, "SboxnetUSB initialising...")
                if self._debug: logDebug(self, f"serialNR={self._serialn} dev={self._sbnusb}")
                try:
                    self._sbnusb = SboxnetUSB(serialn=self._serialn)
                except (SboxnetError, usb.core.USBError) as e:
                    self._sbnusb = None
                    logError(self, "XXx unable to init SboxnetUSB: " + str(e))
                    return False
                self._sbnusb.disable() # flush recv/tmit buffers
                self._sbnusb.enable(devaddr=0, flags=2) # 2 = sniffer
                #logInfo(self, "SboxnetUSB initialized")
            return True
        except (SboxnetError, usb.core.USBError) as e:
            self._sbnusb = None
            logError(self, "unable to init SboxnetUSB: " + str(e))
            return False
    
    # SboxnetMaster.
    def _deinit_conn(self):
        try:
            if self._sbnusb:
                logInfo(self, "SboxnetUSB disabling...")
                self._sbnusb.disable()
                logInfo(self, "SboxnetUSB disabled")
        except (SboxnetError, usb.core.USBError) as e:
            logError(self, "unable to deinit SboxnetUSB: " + str(e))
        self._sbnusb = None

    
