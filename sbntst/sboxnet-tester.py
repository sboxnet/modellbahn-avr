#!/usr/bin/python3 -tt

import sys
import time
import math
import re
import signal
import shlex
import io
import readline
import os.path
from datetime import datetime
import atexit
import rlcompleter
import csv
import inspect
import traceback

import argparse
import threading

import sboxnet
import usb.core
import usb.util

def getint(v):
    if v.startswith("0x"):
        return int(v, 16)
    return int(v)

def checkbyte(v, name):
    try:
        b = getint(v)
        if (b >= -128 and b < 256):
            return (b & 0x00ff)
        print("ERROR: %s is not a byte." % (name))
    except:
        print("ERROR: %s is not an integer." % (name))
    return None

def checkword(v, name):
    try:
        b = getint(v)
        if (b >= -32768 and b < 65536):
            return (b & 0x00ffff)
        print("ERROR: %s is not a 16bit word." % (name))
    except:
        print("ERROR: %s is not an integer." % (name))
    return None

def checkint(v, name):
    try:
        return (getint(v) & 0x0ffffffff)
    except:
        print("ERROR: %s is not an integer." % (name))
    return None

def highbyte(v):
    return ((v >> 8) & 0x00ff)

def lowbyte(v):
    return (v & 0x00ff)

def makeword(v1, v0):
    return ((v1 & 0xff) << 8) + (v0 & 0xff)
def makelong(v3, v2, v1, v0):
    return ((v3 & 0xff) << 24) + ((v2 & 0xff) << 16) + ((v1 & 0xff) << 8) + (v0 & 0xff)

def makeworda(arr, idx):
    return makeword(arr[idx+1], arr[idx])
def makelonga(arr, idx):
    return makelong(arr[idx+3],arr[idx+2],arr[idx+1],arr[idx])

def log(o, h, s):
    if o:
        print('%s:(%s)(%s)(%s.%s):%s:%s' % (datetime.now(), inspect.stack()[2][1], inspect.stack()[2][2], o.__class__.__name__, inspect.stack()[2][3], h, s))
    else:
        print('%s:(%s)(%s)(%s):%s:%s' % (datetime.now(), inspect.stack()[2][1], inspect.stack()[2][2], inspect.stack()[2][3], h, s))

def logInfo(o, s):
    log(o, 'INFO', s)

def logDebug(o, s):
    log(o, 'DEBUG', s)

def logError(o, s):
    log(o, 'ERROR', s)


class SboxnetTester(object):

    _prdid = sboxnet.SboxnetUSB.PRODUCTID
    _seq = 0
    

    # SboxnetTester.
    def __init__(self, dccmap, debug):
        print("--- SboxnetTester ---")
        print("creating SboxnetUSB:...")
        #self._old = False
        self._dccmap = dccmap
        self._serialn = u"1"
        self._debug = debug
        if self._debug: logDebug(None, f"-- running with debug {debug}--")
            
    # SboxnetTester.main()
    def main(self):
        print("type 'help' for help.")
        cmdlist = [ self.cmd_help,
                    self.cmd_reset,
                    self.cmd_dbginfo,
                    self.cmd_dbgstate,
                    self.cmd_devsbndbg,
                    self.cmd_getfwversion,
                    self.cmd_getserialnumber,
                    self.cmd_setserialnumber,
                    self.cmd_tobootloader,
                    self.cmd_list,
                    self.cmd_dbgrecvbuf,
                    self.cmd_dbgtmitbuf,
                    self.cmd_dbgstack,
                    self.cmd_devgetdesc,
                    self.cmd_devsetdesc,
                    self.cmd_devidentify,
                    self.cmd_devreset,
                    self.cmd_regread,
                    self.cmd_regreadm,
                    self.cmd_regwrite,
                    self.cmd_regwritebit,
                    self.cmd_locopower,
                    self.cmd_locodrive,
                    self.cmd_locofunc,
                    self.cmd_locoadd,
                    self.cmd_locodel,
                    self.cmd_locopom,
                    self.cmd_fwupd,
                    self.cmd_avrgetbootloader ]
        try:
            if self._debug: logDebug(self, "find devices")
            # find devices...
            devices = sboxnet.SboxnetUSB.find_devices()
            if self._debug: logDebug(self, "-- found devices ---")
            for d in devices:
                sboxnet.SboxnetUSB(dev=d).getserialnumber()
                if self._debug: logDebug(self, f"device {d}")
                
            if devices is None or len(devices)==0:
                logError(self, "no sboxnet2usb devices found!")
                return
            if len(devices) == 1:
                if self._debug: logDebug(self, "found one device, use it")
                # one found, use it
                self._dev = devices[0]
                if self._debug: logDebug(self, "--------")
                if self._debug: logDebug(self, f"{self._dev}")
                sb = sboxnet.SboxnetUSB(dev=self._dev)
                logInfo(self, f"using device with serialnumber: {sb.getserialnumber()}")
            else:
                logInfo(self, "select sboxnet2usb device:")
                idx = 0
                for dev in devices:
                    sb = sboxnet.SboxnetUSB(dev=dev)
                    print("%d: serialnumber=%s" % (idx, sb.getserialnumber()))
                    idx = idx + 1
                while 1:
                    sel = input("select: ")
                    if sel.isdigit():
                        i = int(sel)
                        if i >= 0 and i < idx:
                            self._dev = devices[i]
                            break
            #logInfo(self, f"using device {self._dev}")
            #logDebug(self, "--- create SboxnetUSB +++")
            
            #lk = threading.Lock()
            #lk.acquire()
            sbnusb=sboxnet.SboxnetUSB(dev=self._dev, debug=self._debug)
            if sbnusb.getserialnumber() in ['modellbahn','test2','test3']:
                #logDebug(self, "### create SboxnetMaster ???")
                self._sbnm = sboxnet.SboxnetMaster(debug=self._debug, sbnusb=sbnusb, cbobj=self)
                self._sbnm.start()
                while 1:
                    rline = input("> ")
                    tokens = shlex.split(rline, comments=True, posix=True)
                    if len(tokens) == 0:
                        while self._sbnm.has_msg():
                            rmsg = self._sbnm.recvmsg()
                            self.printmsg(rmsg)
                        continue
                    if tokens[0] in ["exit","quit","q"]:
                        break
                    found = False
                    for cmd in cmdlist:
                        try:
                            if cmd(tokens):
                                found = True
                                break
                        except (sboxnet.SboxnetError, usb.core.USBError) as e:
                            print("EXCEPTION: " + str(e))
                            found = True
                    if not found:
                        print("ERROR: unknown command: %s" % (tokens[0]))
            else:
                logDebug(self, "Sboxnet device is not 'modellbahn','test2','test3'")
        except Exception as e:
            print("\nEXCEPTION: "+str(e))
        self._sbnm.terminate()
        print("end")
    #
    # SboxnetTester.cmd_help(toks)
    # print the help message
    def cmd_help(self, toks):
        if toks[0] != "help":
            return 0
        print("exit|quit|q")
        print("reset")
        print("dbgstate|ds")
        print("dbginfo|di")
        print("dbgrecvbuf|dr")
        print("dbgtmitbuf|dt")
        print("dbgstack|dst")
        print("getfwversion")
        print("getserialnumber")
        print("setserialnumber s")
        print("tobootloader")
        print("list")
        print("regread|rr addr reg [num]")
        print("regreadm|rrm addr reg0 ...")
        print("regwrite|rw addr reg data")
        print("regwritebit|rwb addr reg(31..) bit val")
        print("devgetdesc addr [1..id]")
        print("devsetdesc addr [1..id] text")
        print("devreset addr")
        print("identify addr on")
        print("devsbndbg addr")
        print("avrgetbootloader")
        print("fwupd addr flag file")
        print("locopower|lp addr flags")
        print("locodrive|ld addr locaddr locspeed [fnkeys]")
        print("locofunc|lf addr locaddr fnkeys")
        print("locoadd|la addr locaddr flags")
        print("locodel addr locaddr")
        print("locopom addr locaddr cv data")
        return 1
    #
    # SboxnetTester.getfwversion()
    # print the sboxnet USB firmware version (hex string)
    def cmd_getfwversion(self, toks):
        if toks[0] not in ['getfwversion']:
            return 0
        fwv = self._sbnm.sboxnet().getfwversion()
        print("firmware version: 0x%x" % (fwv))
        return 1
    #
    # SboxnetTester.cmd_getserialnumber(toks)
    # get the sboxnet USB serial number (string)
    # getserialnumber
    def cmd_getserialnumber(self, toks):
        if toks[0] not in ['getserialnumber']:
            return 0
        sn = self._sbnm.sboxnet().getserialnumber()
        print("serial number: %s" % (sn))
        return 1
    #
    # SboxnetTester.setserialnumber(toks)
    # set the sboxnet USB serial number (string)
    # setserialnumber udesc
    def cmd_setserialnumber(self, toks):
        if toks[0] not in ['setserialnumber']:
            return 0
        if len(toks) == 2:
            udesc = toks[1]
            self._sbnm.sboxnet().setserialnumber(udesc)
        else:
            print("ERROR: not serialnumber given!")
        return 1
    #
    # SboxnetTester.cmd_tobootloader(toks)
    # ???
    def cmd_tobootloader(self, toks):
        if toks[0] not in ["tobootloader"]:
            return 0
        self._sbnm.sboxnet().reset()
        return 1
    #
    # SboxnetTester.cmd_list(toks)
    # list all registered devices
    def cmd_list(self, toks):
        if toks[0] not in ["list"]:
            return 0
        self._sbnm._addrmap.print_entries()
        return 1
    #
    # SboxnetTester.cmd_devreset(toks)
    # sboxnet dev reset
    # [sboxnet-addr]
    # if no addr is given, do a net reset
    def cmd_devreset(self, toks):
        if toks[0] not in ["devreset"]:
            return 0
        addr = None
        if len(toks) == 2:
            addr = checkbyte(toks[1], "addr")
        if (addr is not None):
            self.execmsg(addr, sboxnet.SBOXNET_CMD_DEV_RESET)
        else:
            self.execmsg(255, sboxnet.SBOXNET_CMD_DEV_RESET)
        return 1
    #
    # SboxnetTester.cmd_avrgetbootloader(toks)
    # print the avr bootloader to file avr-boot-loader.bin
    def cmd_avrgetbootloader(self, toks):
        if toks[0] not in ["avrgetbootloader"]:
            return 0
        data = self._sbnm.sboxnet().ctrl(usb.util.CTRL_IN, 0xff, 0, 0, 4096)
        with open("avr-boot-loader.bin", "wb") as fh:
             data.tofile(fh)
        return 1
    #
    # SboxnetTester.cmd_dbgstate(toks)
    # print state
    def cmd_dbgstate(self, toks):
        if toks[0] not in ["dbgstate", "ds"]:
            return 0
        print("status        0x%x" % (self._sbnm.laststatus))
        ds = self._sbnm.sboxnet().getdbgstate()
        print("state         0x%x" % (ds.state))
        print("flags         0x%x" % (ds.flags))
        print("retry_counter %d" % (ds.retry_counter))
        print("tmit_cnt      %d" % (ds.tmit_cnt))
        print("tmit_bytes    %d" % (ds.tmit_bytes))
        print("tmit_lastbyte %d" % (ds.tmit_lastbyte))
        print("backoff_bits  %d" % (ds.backoff_bits))
        print("recv_len      %d" % (ds.recv_len))
        print("prng_seed     %d" % (ds.prng_seed))
        return 1
    #
    # SboxnetTester.cmd_dbgrecvbuf(toks)
    # print receive buffer
    def cmd_dbgrecvbuf(self, toks):
        if toks[0] not in ["dbgrecvbuf", "dr"]:
            return 0
        b = self._sbnm.sboxnet().getdbgrecvbuf()
        self.print_dgbringbuf(b)
        return 1
    #
    # SboxnetTester.cmd_dbgtmitbuf(toks)
    # print transmit buffer
    def cmd_dbgtmitbuf(self, toks):
        if toks[0] not in ["dbgtmitbuf", "dt"]:
            return 0
        b = self._sbnm.sboxnet().getdbgtmitbuf()
        self.print_dgbringbuf(b)
        return 1
    #
    # SboxnetTester.cmd_dbgstack(toks)
    # debug stack: size, free
    def cmd_dbgstack(self, toks):
        if toks[0] not in ["dbgstack", "dst"]:
            return 0
        b = self._sbnm.sboxnet().getdbgstack()
        print("stack size: %d" % (b.size))
        print("stack free: %d" % (b.free))
        return 1
    #
    # SboxnetTester.cmd_dbginfo(toks)
    # print debug info
    def cmd_dbginfo(self, toks):
        if toks[0] not in ["dbginfo", "di"]:
            return 0
        d = self._sbnm.sboxnet().getdbginfo()
        self.print_dbginfo(d)
        return 1
    #
    # SboxnetTester.print_dbginfo(d)
    # print debug info
    def print_dbginfo(self, d):
        print("recv_byte        %d" % (d.recv_byte))
        print("recv_msg_got     %d" % (d.recv_msg_got))
        print("recv_msg_fetched %d" % (d.recv_msg_fetched))
        print("tmit_byte        %d" % (d.tmit_byte))
        print("tmit_msg_tosend  %d" % (d.tmit_msg_tosend))
        print("tmit_msg_sent    %d" % (d.tmit_msg_sent))
        print("collisions       %d" % (d.collisions))
        print("coll_rxc         %d" % (d.coll_rxc))
        print("coll_fe          %d" % (d.coll_fe))
        print("coll_udr         %d" % (d.coll_udr))
        print("retries          %d" % (d.retries))
        print("recverrors       %d" % (d.recverrors))
        print("recverr_fe       %d" % (d.recverr_fe))
        print("recverr_pe       %d" % (d.recverr_pe))
        print("recverr_dor      %d" % (d.recverr_dor))
        print("recverr_proto    %d" % (d.recverr_proto))
        print("recverr_size     %d" % (d.recverr_size))
        print("crcerrors        %d" % (d.crcerrors))
        print("tmiterrors       %d" % (d.tmiterrors))
    #
    # SboxnetTester.print_dgbringbuf(b)
    # print debug ring buffer
    def print_dgbringbuf(self, b):
        print("rd           %d" % (b.rd))
        print("rd_count     %d" % (b.rd_count))
        print("rd_rollback  %d" % (b.rd_rollback))
        print("wr           %d" % (b.wr))
        print("wr_count     %d" % (b.wr_count))
        print("wr_rollback  %d" % (b.wr_rollback))
        print("count        %d" % (b.count))
        print("size         %d" % (b.size))
        i = 0
        while i < len(b.buf):
            print("%-4d: " % (i), end="")
            for k in range(0, 8):
                if i < len(b.buf):
                    print("%02x " % (b.buf[i]), end="")
                i = i + 1
            print("")
    #
    # SboxnetTester.cmd_devidentify(toks)
    # sboxnet command: identify
    # sboxnet-addr 1|0
    def cmd_devidentify(self, toks):
        if toks[0] != "identify":
            return 0
        if len(toks) != 3:
            print("ERROR: usage: identify addr on")
        else:
            addr = checkbyte(toks[1], "addr")
            on = checkbyte(toks[2], "on")
            if (addr is not None) and (on is not None):
                self.execmsg(addr, sboxnet.SBOXNET_CMD_DEV_IDENTIFY, [on])
        return 1
    #
    # SboxnetTester.cmd_fwupd(toks)
    # init firmware update
    # sboxnet-addr flags (1 = appication) firmware-file
    def cmd_fwupd(self, toks):
        if toks[0] != "fwupd":
            return 0
        if len(toks) != 4:
            print("ERROR: usage: fwupd addr flag file")
            return 1
        addr = checkbyte(toks[1], "addr")
        flag = checkbyte(toks[2], "flag")
        fn = toks[3]
        if (addr is not None) and (flag is not None):
            self._fwupdload(addr, fn, None, None, flag)
        return 1
    #
    # SboxnetTester._fwupload(addr, fwfile, productid, vendorid, flag)
    # sboxnet load firmware
    # sboxnet-addr firmware-file productid vendorid flags (1 = application, else bootloader)
    def _fwupdload(self, addr, fn, prdid, vendorid, flag):
        if not os.path.isfile(fn):
            print("ERROR: file not found: %s" % (fn));
            return
        try:
            with open(fn, "rb") as fh:
                size = os.path.getsize(fn)
                if size <= 32:
                    print("ERROR: not a firmware update file, too small: %s" % (fn))
                    return
                b = fh.read(32)
                x = [lowbyte(size-32), highbyte(size-32), flag]
                for i in range(0,len(b)):
                    x.append(b[i])
                rmsg = self.execmsg(addr, sboxnet.SBOXNET_CMD_DEV_FW_UPD_START, x)
                if rmsg is None:
                    return
                if rmsg.cmd != sboxnet.SBOXNET_CMD_DEV_FW_UPD_START|0x80:
                    return
                pc = 0
                print("")
                while True:
                    print("\rload pc=0x%x " % (pc), end='')
                    sys.stdout.flush()
                    b = fh.read(32)
                    x = []
                    x.append(lowbyte(pc))
                    x.append(highbyte(pc))
                    x.append(flag)
                    if len(b) == 0:
                        rmsg = self.execmsg(addr, sboxnet.SBOXNET_CMD_DEV_FW_UPD_LOAD, x, printit=False) # finish load
                        if rmsg is None or rmsg.cmd != sboxnet.SBOXNET_CMD_DEV_FW_UPD_LOAD|0x80:
                            print("ERROR: failed to load firmware")
                        break
                    for i in range(0,len(b)):
                        x.append(b[i])
                    rmsg = self.execmsg(addr, sboxnet.SBOXNET_CMD_DEV_FW_UPD_LOAD, x, printit=False)
                    if rmsg is None or rmsg.cmd != sboxnet.SBOXNET_CMD_DEV_FW_UPD_LOAD|0x80:
                        print("ERROR: failed to load firmware")
                        break
                    pc = pc + len(b)
                print("")
        except Exception as e:
            print("\nEXCEPTION: "+str(e))
    #
    # SboxnetTester.cmd_reset(toks)
    # sboxnet net reset
    def cmd_reset(self, toks):
        if toks[0] != "reset":
            return 0
        if len(toks) != 1:
            print("ERROR: usage: reset")
        else:
            self._sbnm.send_reset()
        return 1
    #
    # SboxnetTester.cmd_devgetdesc(toks)
    # sboxnet get device description
    # sboxnet-addr stridx (1..id)
    def cmd_devgetdesc(self, toks):
        if toks[0] != "devgetdesc":
            return 0
        if len(toks) != 3:
            print("ERROR: usage: devgetdesc addr [1..id]")
        else:
            addr = checkbyte(toks[1], "addr")
            did = checkbyte(toks[2], "id");
            if addr is not None and did is not None:
                self.execmsg(addr, sboxnet.SBOXNET_CMD_DEV_GET_DESC, [did])
        return 1
    #
    # SboxnetTester.cmd_devsetdesc(toks)
    # set device description
    # sboxnet-addr stridx (1..id) string
    def cmd_devsetdesc(self, toks):
        if toks[0] != "devsetdesc":
            return 0
        if len(toks) != 4:
            print("ERROR: usage: devgetdesc addr [1..id] string")
        else:
            addr = checkbyte(toks[1], "addr")
            did = checkbyte(toks[2], "id")
            strx = toks[3]
            if len(strx) > 32:
                print("ERROR: description string is to long, >32 chars.")
            elif addr is not None and did is not None:
                x = [did];
                x.extend(bytes(strx, "ascii"))
                self.execmsg(addr, sboxnet.SBOXNET_CMD_DEV_SET_DESC, x)
        return 1
    #
    # SboxnetTester.regread(toks)
    # sboxnet read register
    # sboxnet-addr register [count]
    def cmd_regread(self, toks):
        if toks[0] not in ["regread", "rr"]:
            return 0
        if len(toks) not in [3, 4]:
            print("ERROR: usage: regread addr reg [count]")
        else:
            addr = checkbyte(toks[1], "addr")
            reg = checkword(toks[2], "reg")
            count = 1
            if len(toks) == 4:
                count = checkbyte(toks[3], "count")
            if addr is not None and reg is not None:
                x = [lowbyte(reg), highbyte(reg)]
                if count is not None:
                    x.append(count)
                self.execmsg(addr, sboxnet.SBOXNET_CMD_REG_READ, x)
        return 1
    #
    # SboxnetTester.regeadm(toks)
    # sboxnet regsiter read multiple
    # sboxnet-addr register0 [register1 [register2...]]
    def cmd_regreadm(self, toks):
        if toks[0] not in ["regreadm", "rrm"]:
            return 0
        if len(toks) < 3:
            print("ERROR: usage: regread addr reg0 ...")
        else:
            addr = checkbyte(toks[1], "addr")
            regs = []
            i = 2
            while i < 33:
                if len(toks) == i:
                    break
                reg = checkword(toks[i], "reg%d" % i)
                if reg is None:
                    return 1
                regs.append(lowbyte(reg))
                regs.append(highbyte(reg))
                i = i + 1
            if addr is not None:
                self.execmsg(addr, sboxnet.SBOXNET_CMD_REG_READ_MULTI, regs)
        return 1    
    #
    # SboxnetTester.cmd_regwrite(toks)
    # sboxnet command: register write
    # sboxnet-addr, register, value, [mask]
    def cmd_regwrite(self, toks):
        if toks[0] not in ["regwrite", "rw"]:
            return 0
        if len(toks) != 4 and len(toks) != 5:
            print("ERROR: usage: regwrite addr reg value [mask]")
        else:
            addr = checkbyte(toks[1], "addr")
            reg = checkword(toks[2], "reg")
            val = checkword(toks[3], "value")
            mask = None
            if len(toks) == 5:
                mask = checkword(toks[4], "mask")
                if mask is None:
                    return 1
            if addr is not None and reg is not None and val is not None:
                x = [lowbyte(reg), highbyte(reg), lowbyte(val), highbyte(val)]
                if mask is not None:
                    x.append(lowbyte(mask))
                    x.append(highbyte(mask))
                self.execmsg(addr, sboxnet.SBOXNET_CMD_REG_WRITE, x)
        return 1

    #
    # SboxnetTester.devsbndbg(toks)
    # sboxnet print dev debug info
    # debug info is register 2010 (see bootloader-fw.c, R_DEBUG_SBN_0)
    def cmd_devsbndbg(self, toks):
        if toks[0] not in ["devsbndbg"]:
            return 0
        if len(toks) != 2:
            print("ERROR: usage: devsbndbg addr")
        else:
            addr = checkbyte(toks[1], "addr")
            if addr is None:
                return 1
            rmsg  = self.execmsg(addr, sboxnet.SBOXNET_CMD_REG_READ, [lowbyte(2010), highbyte(2010), 31])
            if rmsg:
                d = sboxnet.SboxnetDebugInfo(rmsg.data)
                self.print_dbginfo(d)
        return 1
    #
    # SboxnetTester.regwritebit(toks)
    # sboxnet set a bit in a register
    # sboxnet-addr register bitnumber (0-15) value
    def cmd_regwritebit(self, toks):
        if toks[0] not in ["regwritebit","rwb"]:
            return 0
        if len(toks) != 5:
            print("ERROR: usage: regwritebit addr reg bitnr val")
        else:
            addr = checkbyte(toks[1], "addr")
            reg = checkword(toks[2], "reg")
            bitnr = checkbyte(toks[3], "bitnr")
            val = checkbyte(toks[4], "val")
            if (addr is not None) and (reg is not None) and (bitnr is not None) and (val is not None):
                if val: val = 0x80
                else:   val = 0
                bitnr = bitnr & 0x0f
                self.execmsg(addr, sboxnet.SBOXNET_CMD_REG_WRITE_BIT, [lowbyte(reg), highbyte(reg), (val | bitnr)])
        return 1
    #
    # SboxnetTester.cmd_locopower(toks)
    # sboxnet set booster on/off
    # sboxnet-addr flags: 1 = on
    def cmd_locopower(self, toks):
        if toks[0] not in ["locopower", "lp"]:
            return 0
        if len(toks) != 3:
            print("ERROR: usage: locopower|lp addr flags:off=0|on=1")
        else:
            addr = checkbyte(toks[1], "addr")
            flags = checkbyte(toks[2], "flags")
            if (addr is not None) and (flags is not None):
                self.execmsg(addr, sboxnet.SBOXNET_CMD_LOCO_POWER, [flags])
        return 1
    #
    # SboxnetTester.cmd_locodrive(toks)
    # sboxnet commenad: drive a loco
    # sboxnet-addr dcc-addr speed [fnkeys]
    def cmd_locodrive(self, toks):
        if toks[0] not in ["locodrive", "ld"]:
            return 0
        if not len(toks) in [4,5]:
            print("ERROR: usage: locodrive|ld addr locaddr locspeed [fnkeys]")
        else:
            addr = checkbyte(toks[1], "addr")
            locaddr = checkword(toks[2], "locaddr")
            locspeed = checkbyte(toks[3], "locspeed")
            data = [lowbyte(locaddr), highbyte(locaddr), locspeed]
            if len(toks) == 5:
                fnkeys = checkword(toks[4], "fnkeys")
                data.append(lowbyte(fnkeys))
                data.append(highbyte(fnkeys))
            if (addr is not None) and (locaddr is not None) and (locspeed is not None):
                self.execmsg(addr, sboxnet.SBOXNET_CMD_LOCO_DRIVE, data)
        return 1
    #
    # SboxnetTester.cmd_locofunc(toks)
    # sboxnet command: enable loco funtion
    # sboxnet-addr dcc-locaddr fnkeys
    def cmd_locofunc(self, toks):
        if toks[0] not in ["locofunc", "lf"]:
            return 0
        if len(toks) != 4:
            print("ERROR: usage: locofunc|lf addr locaddr fnkeys")
        else:
            addr = checkbyte(toks[1], "addr")
            locaddr = checkword(toks[2], "locaddr")
            fnkeys = checkword(toks[3], "fnkeys")
            if (addr is not None) and (locaddr is not None) and (fnkeys is not None):
                self.execmsg(addr, sboxnet.SBOXNET_CMD_LOCO_FUNC, [lowbyte(locaddr), highbyte(locaddr),
                                                             lowbyte(fnkeys), highbyte(fnkeys)])
        return 1
    #
    # SboxnetTester.cmd_locoadd(toks)
    # sboxnet command: add a loco to the dcc stack
    # sboxnet-addr, dcc-locaddr, flags (speed-steps)
    # flags: 0 speed-steps 14
    #        1 speed-steps 28
    #        2 speed-steps 128
    def cmd_locoadd(self, toks):
        if toks[0] not in ["locoadd", "la"]:
            return 0
        if len(toks) != 4:
            print("ERROR: usage: locoadd|la addr locaddr speedflag:-14=0|-28=1|-128=2")
        else:
            addr = checkbyte(toks[1], "addr")
            locaddr = checkword(toks[2], "locaddr")
            flags = checkbyte(toks[3], "flags")
            if (addr is not None) and (locaddr is not None) and (flags is not None):
                self.execmsg(addr, sboxnet.SBOXNET_CMD_LOCO_ADD, [lowbyte(locaddr), highbyte(locaddr), flags])
        return 1
    
    #def cmd_locoadd_all(self, toks):
    #    if toks[0] not in ["locoaddall"]:
    #        return 0
    #    if len(toks) != 2:
    #        print("ERROR: usage: locoaddall addr")
    #    else:
    #        addr = checkbyte(toks[1], "addr")
    #        if (addr is not None):
    #            locaddr = 1
    #            for i in range(0, 100):
    #               self.execmsg(addr, sboxnet.SBOXNET_CMD_LOCO_ADD, [lowbyte(locaddr), highbyte(locaddr), 2])
    #                locaddr = locaddr + 1
    #    return 1
    #
    # SboxnetTester.cmd_locodel(toks)
    # sboxnet command: delete a loco from the dcc stack: sboxnet-addr, dcc-addr
    def cmd_locodel(self, toks):
        if toks[0] not in ["locodel"]:
            return 0
        if len(toks) != 3:
            print("ERROR: usage: locodel addr locaddr")
        else:
            addr = checkbyte(toks[1], "addr")
            locaddr = checkword(toks[2], "locaddr")
            if (addr is not None) and (locaddr is not None):
                self.execmsg(addr, sboxnet.SBOXNET_CMD_LOCO_DEL, [lowbyte(locaddr), highbyte(locaddr)])
        return 1
    #
    # SboxnetTester.cmd_locopom(toks)
    # sboxnet command: loco pom: sboxnet-addr, dcc-addr, cv, data (1 byte)
    def cmd_locopom(self, toks):
        if toks[0] not in ["locopom"]:
            return 0
        if len(toks) != 5:
            print("ERROR: usage: locopom addr locaddr cv data")
        else:
            addr = checkbyte(toks[1], "addr")
            locaddr = checkword(toks[2], "locaddr")
            cv = checkword(toks[3], "cv")
            cvd = checkbyte(toks[4], "data")
            if (addr is not None) and (locaddr is not None) and (cv is not None) and (cvd is not None):
                if cv > 0 and cv <= 0x400:
                    cv = cv - 1
                    self.execmsg(addr, sboxnet.SBOXNET_CMD_LOCO_POM, [lowbyte(locaddr), highbyte(locaddr),
                                                           lowbyte(cv), highbyte(cv & 0x3ff)|0x0c,
                                                           cvd])
                else:
                    print("ERROR: cv out of range")
        return 1
    #
    # SboxnetTester.printmsg(msg, fh): msg as an array (see SboxnetMsg). fh if true log to file sboxnet.log
    # user readable msg as string
    def printmsg(self, msg, fh=None):
        if msg is None:
            return None
        if self._debug: logDebug(self, f'~~~~~ PRINT MSG ')
        outstr = "%s srcaddr=%d destaddr=%d seq=%d len=%d %scmd=%s (0x%x) crc=0x%x" % (("-->" if (msg.cmd & 0x80==0x80) else "<--"), 
                                                                    msg.srcaddr, msg.dstaddr, msg.seq, msg.dlen,
                                                                    (">" if (msg.cmd & 0x80) else " "), sboxnet.cmd_to_str(msg.cmd & 0x7f), msg.cmd, msg.crc)
        if self._debug: logDebug(self, f"Outstr: {outstr}")
        
        if msg.cmd == (0x80|sboxnet.SBOXNET_CMD_DEV_GET_DESC):
            if self._debug: logDebug(self, f'---- device description ----')
            desc = bytes(msg.data[0:msg.dlen]).decode(encoding="ascii")
            outstr = outstr + " DEV DESC: %s" % (desc)
        
        elif msg.cmd == (0x80|sboxnet.SBOXNET_CMD_REG_READ) or msg.cmd == (0x80|sboxnet.SBOXNET_CMD_REG_READ_MULTI):
            outstr = outstr + ":"
            i = 0
            while i < msg.dlen/2:
                w = makeworda(msg.data, 2*i)
                outstr = outstr + " 0x%x (%d)," % (w, w)
                i = i + 1
        
        elif msg.cmd == sboxnet.SBOXNET_CMD_FB_LOCOADDR:
            dcc = makeworda(msg.data, 1)
            lok = None
            if dcc in self._dccmap:
                lok = self._dccmap[dcc]
            outstr = outstr + ": sensor %d = %d, locoaddr=%d %s" % (int(msg.data[0] & 0x7f), int((msg.data[0] >> 7) & 1),
                                                    dcc, lok if lok else "Unknown-DCC-Address")
            
        elif msg.cmd == 0x80:
            rcmd = msg.data[0]
            rc = msg.data[1]
            outstr = outstr +": ackrc: %s (%d)  cmd: %s (0x%x)" % (sboxnet.ackrc_to_str(rc), rc,
                                                                    sboxnet.cmd_to_str(rcmd), rcmd)
        else:
            i = 0
            while i < msg.dlen:
                w = msg.data[i]
                outstr = outstr + " 0x%x (%d)," % (w, w)
                i = i + 1
        #if fh is None:
            #with open("sboxnet.log", "a") as fh:
            #    logInfo(self, outstr, fh)
        #else:
        #logInfo(self, outstr, fh)
        #logInfo(self, outstr)
        print(outstr)
        #if self._debug: logDebug(self, outstr)
        return msg
    #
    # SboxnetTester.execmsg(addr, cmd, data = [], printit = True)
    # create a sboxnet msg and send it
    def execmsg(self, addr, cmd, data = [], printit = True):
        rmsg = self.sendmsg(sboxnet.SboxnetMsg.new(addr, cmd, self._seq, data))
        seq = (self._seq + 1) & 255
        if seq < 10:
            seq = 10
        self._seq = seq
        if printit:
            self.printmsg(rmsg, sys.stdout)
        return rmsg
    #
    # SboxnetTester.sendmsg(msg)
    # send a sboxnet msg
    def sendmsg(self, msg):
        dstaddr = msg.dstaddr
        seq = msg.seq
        if self._sbnm.has_msg():
            # messages are in the buffer! print and flush
            while self._sbnm.has_msg():
                rmsg = self._sbnm.recvmsg()
        #        if rmsg:
        #            self.printmsg(rmsg)
        #    #print("============")
        #
        print("SEND to %s: %s  s:%d l:%d" % (msg.dstaddr, sboxnet.cmd_to_str(msg.cmd), msg.seq, msg.dlen))
        self._sbnm.sendmsg(msg)
        endtime = time.time() + 5
        while time.time() < endtime:
            #if self._debug: logDebug(self, f"try to receive a msg")
            if self._sbnm.has_msg():
                #if self._debug: logDebug(self, f"have a msg, receive it")
                #if self._debug: logDebug(self, 'TEST2')
                rmsg = self._sbnm.recvmsg()
                #if self._debug: logDebug(self, 'TEST3')
                rmsg.print(self)
                #if self._debug: logDebug(self, "TEST1")
                if rmsg.srcaddr != dstaddr:
                    print("ERROR: answer from wrong device: addr=%s" % (rmsg.srcaddr))
                if rmsg.seq != seq:
                    rmsg.print(self)
                    print("ERROR: wrong sequence!")
                return rmsg
        print("ERROR: no answer from device %s" % (dstaddr))
        return None
    #
    # SboxnetTester.recvmsg()
    # receive a sboxnet msg if one is available
    def recvmsg(self):
        if self._sbnm.has_msg():
            rmsg = self._sbnm.recvmsg()
            return rmsg
        return None
    #
    # SboxnetTester.cb_processmsg(msg): callback process a msg, do actions on msg.cmd
    #  SBOXNET_CMD_FB_CHANGED
    #   ack it
    #  SBOXNET_CMD_FB_LOCOADDR
    #   ack it
    def cb_processmsg(self, msg):
        if self._debug: logDebug(self, f"{sboxnet.cmd_to_str(msg.cmd & 0x7f)}  dstaddr={msg.dstaddr} srcaddr={msg.srcaddr} seq={msg.seq} dlen={msg.dlen} ")
        if msg.cmd == sboxnet.SBOXNET_CMD_FB_CHANGED:
            self._sbnm.sendmsg(sboxnet.SboxnetMsg.new(msg.srcaddr, msg.cmd|0x80, msg.seq))
            return True
        if msg.cmd == sboxnet.SBOXNET_CMD_FB_LOCOADDR:
            self._sbnm.sendmsg(sboxnet.SboxnetMsg.new(msg.srcaddr, msg.cmd|0x80, msg.seq))
            return True
        return False
    #
    # SboxnetTester.cb_printmsg(msg): print a sboxnet msg if not Net Watchdog Msg
    # callback
    def cb_printmsg(self, msg):
        #if self._debug: logDebug(self, sboxnet.cmd_to_str(msg.cmd))
        if msg.cmd != sboxnet.SBOXNET_CMD_NET_WATCHDOG:
            #if self._debug: logDebug(self, f"msg.cmd: {msg.cmd&0x7f} 0x{(msg.cmd&0x7f):x} "+sboxnet.cmd_to_str(msg.cmd))
            #self.printmsg(msg)
            pass

# --- main ---

if __name__ == "__main__":
    # ignore SIGINT
    def signal_sigint(sig, frame):
        pass
    # set SIGINT handler to signal_sidint 
    signal.signal(signal.SIGINT, signal_sigint)
    # set history path to ~/.sboxnet-tester.history
    historyPath = os.path.expanduser("~/.sboxnet-tester.history")

    def save_history(historyPath=historyPath):
        readline.write_history_file(historyPath)
    # if ~/.sboxnet-tester.history exists set as readline history path
    if os.path.exists(historyPath):
        readline.read_history_file(historyPath)
    # atexit handler: save_history
    atexit.register(save_history)
    # read moba-dcc.csv
    dccmap = dict()
    with open('moba-dcc.csv') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            dcc = int(row['DCC'])
            m = dict()
            m['dcc'] = dcc;
            m['lok'] = row['Lok']
            m['artikel'] = row['Artikel']
            dccmap[dcc] = m
    
    parser = argparse.ArgumentParser()
    parser.add_argument("-d", "--debug", help="run with debug output", action="store_true")
    args = parser.parse_args()
    debug = False
    if args.debug:
        debug = True
    #print(f"args.debug={args.debug}")
    print(f"----- debug={args.debug} ++++++{debug}")
    
    # create SboxnetTester
    sbntest = SboxnetTester(dccmap, debug)
    # SboxnetTester.main()
    sbntest.main()
