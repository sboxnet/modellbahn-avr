#!/usr/bin/python3 -tt

import sys
import time
import math
import re
import threading
import signal
import select
import socketserver
import shlex
from datetime import datetime
from datetime import timedelta
import csv

import sboxnet
import usb.core

loglock = threading.RLock()
logfh = None

def log(h, s):
    global logfh
    global loglock
    with loglock:
        logfh.write("%s:%s:%s\n" % (datetime.now(), h, s))
    logfh.flush()
    
def logInfo(s):
    log('INFO', s)

def logError(s):
    log('ERROR', s)

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


class SrcpHandler(object):
    STATE_INIT = 0
    STATE_HANDSHAKE = 1
    STATE_COMMAND = 2
    STATE_INFO = 3
    STATE_TERM = 4

    def __init__(self, handler):
        self._h = handler
        self._state = self.STATE_INIT
        self._threadid = threading.current_thread()
        self._sessionid = 0
        self._sboxnet = handler.server.sboxnet
        self._lockinfo = threading.Condition()
        self._qinfo = []
        self._shutdown = False
        logInfo("Starting srcpd-sboxnet Thread %s" % (self._threadid))

    def shutdown(self):
        self._shutdown = True

    def readline(self):
        s = self._h.rfile.readline(1000).decode('latin_1').strip(' \n')
        logInfo('recv(%s): %s' % (self._threadid, s))
        return s
		
    def writeline(self, line, withts = True):
        if withts:
            (ctf, cts) = math.modf(time.time())
            line = "%s.%s %s" % (int(cts), int(ctf * 1000), line)
        logInfo('send(%s): %s' % (self._threadid, line))
        self._h.wfile.write((line + '\n').encode('latin_1'))
        self._h.wfile.flush()
        
    def notify_info(self, answer):
        if self._state == self.STATE_INFO:
            with self._lockinfo:
                self._qinfo.append(answer)
                self._lockinfo.notify()

    def serve_init(self):
        # send welcome
        self.writeline('srcpd-sboxnet V0.1.0; SRCP 0.8.4', False)
        self._state = self.STATE_HANDSHAKE
        return 0

    def serve_handshake(self):
        conmode = self.STATE_COMMAND
        while True:
            line = self.readline()
            if line == "":
                return 1
            
            if line.startswith('SET PROTOCOL SRCP'):
                if 'SRCP 0.8' in line:
                    self.writeline('201 OK PROTOCOL SRCP')
                else:
                    self.writeline('400 ERROR unsupported protocol')
	        
            elif line.startswith('SET CONNECTIONMODE SRCP'):
                if 'SRCP COMMAND' in line:
                    self.writeline('202 OK CONNECTIONMODE')
                    conmode = self.STATE_COMMAND
                elif 'SRCP INFO' in line:
                    self.writeline('202 OK CONNECTIONMODE')
                    conmode = self.STATE_INFO
                else:
                    self.writeline('401 ERROR unsupported connection mode') 
            
            elif line == 'GO':
                self.writeline('200 OK GO %d' % (self._sessionid))
                self._state = conmode
                return 0
            
            else:
                self.writeline('410 ERROR unknown command')
    
    def serve_command(self):
        while not self._shutdown:
            line = self.readline()
            if line == "":
                return 1
            answer = self._sboxnet.command(line)
            if answer is not None:
                self.writeline(answer);
        return 0
        
    def serve_info(self):
        self._sboxnet.send_info_init(self)
        while not self._shutdown:
            self._lockinfo.acquire()
            while len(self._qinfo) == 0:
                self._lockinfo.wait(1)
                r, w, x = select.select([self._h.request], [], [], 0)
                if self._h.request in r:
                    # if something is received -> terminate
                    return 1
            answer = self._qinfo.pop(0)
            self._lockinfo.release()
            self.writeline(answer)
        return 1
    
    def serve(self):
        self._sessionid = self._sboxnet.register_srcph(self)
        while not self._shutdown:
            rc = 0
            if self._state == self.STATE_INIT:
                rc = self.serve_init()
            elif self._state == self.STATE_HANDSHAKE:
                rc = self.serve_handshake()
            elif self._state == self.STATE_INFO:
                rc = self.serve_info()
            else:
                rc = self.serve_command()
            if rc:
                self._state = self.STATE_TERM
                break
        self._sboxnet.unregister_srcph(self)
        logInfo("End(%s)" % (self._threadid))


class PuidRoc(object):
    def __init__(self, puid, roca, senscnt = None, reports_locoaddr=False, is_ga=False):
        self.puid = puid
        self.roca = roca
        self.senscnt = senscnt
        self.reports_locoaddr = reports_locoaddr
        self.is_ga = is_ga

class PuidRocMap(object):
    def __init__(self):
        self._map_puid = dict()
        self._map_roca = dict()
    
    def add(self, pr):
        self._map_puid[pr.puid] = pr
        self._map_roca[pr.roca] = pr
    
    def roca(self, puid):
        try:
            return self._map_puid[puid]
        except:
            logInfo("key puid="+str(puid)+" not found")
    
    def puid(self, roca):
        try:
            return self._map_roca[roca]
        except:
            logInfo("key roca="+str(roca)+" not found")
    
        
    
    def to_list(self):
        return self._map_puid.values()

    def printIt(self):
        print("puid ----")
        for k,v in self._map_puid.items():
            pobj=v
            print("puid="+str(k)+". roca="+str(pobj.roca))
        print("roca ----")
        for k,v in self._map_roca.items():
            pobj=v
            print("roca="+str(k)+". puid="+str(pobj.puid))

class Feedback(object):
    def __init__(self):
        self.on = 0
        self.locoaddr = None
                
class Sboxnet(object):
    def __init__(self, srcpd):
        self._srcpd = srcpd
        self._lockcmd = threading.RLock()
        self._lockreg = threading.RLock()
        self._locksend = threading.RLock()
        self._srcph_dict = dict()
        self._srcph_sessid_cnt = 1
        self._bus = 7
        self._power = "OFF"
        self._stat_ga = dict()
        self._stat_fb = dict()
        self._stat_gl = dict()
        self._shutdown = False
        
        self._init_fb = PuidRocMap()
#        self._init_dccgen = None
#        self._init_booster = None
        with open('puid.csv') as csvfile:
            reader = csv.DictReader(csvfile)
            for row in reader:
                puid = int(row['puid'])
                roca = int(row['roca'])
                senscnt = int(row['senscnt'])
                isga = int(row['isga'])
                sens = []
                if senscnt > 0:
                    sens = [1] * senscnt
                reports_locoaddr = int(row['reports_locoaddr'])
                comment = row['comment']
                pr = PuidRoc(puid, roca, sens, True if reports_locoaddr != 0 else False, True if isga > 0 else False)
                #print("pr "+str(pr)+comment)
                if comment == "DCC Gen":
                    self._init_dccgen = pr
                    #print("-dcc gen "+str(self._init_dccgen))
                elif comment == "Booster":
                    self._init_booster = pr
                    #print("-booster "+str(self._init_booster))
                else:
                    self._init_fb.add(pr)
                pr.comment = comment
        #self._init_fb = PuidRocMap()
        #self._init_fb.add(PuidRoc(puid=0x2, roca=1, senscnt=[1, 2, 4, 3, 2, 2, 1], is_ga=True))
        #self._init_fb.add(PuidRoc(puid=0x4, roca=2, senscnt=[1, 2, 4, 3, 2, 2], is_ga=True))
        #self._init_fb.add(PuidRoc(puid=0x5, roca=5, senscnt=[1] * 16, reports_locoaddr=True))
        #self._init_fb.add(PuidRoc(puid=0xa, roca=6, senscnt=[1] * 16, reports_locoaddr=True))
        #self._init_fb.add(PuidRoc(puid=0x7, roca=7, senscnt=[1] * 16, reports_locoaddr=True))
        #print(""+self._init_dccgen+", ")#+self._init_booster)
                
        self._sbnm = sboxnet.SboxnetMaster(serialn="modellbahn", cbobj=self)
        self._sbnm.start()
        self._seq = 10
        
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

    def debug_info(self):
        print("============================================")
        for pr in (list(self._init_fb.to_list()) + [self._init_booster, self._init_dccgen]):
            addr = self._sbnm.get_puid_addr(pr.puid)
            if addr:
                rmsg  = self.sendmsg(sboxnet.SboxnetMsg.new(addr, sboxnet.SBOXNET_CMD_REG_READ, 0, [lowbyte(2010), highbyte(2010), 31]))
                if rmsg:
                    d = sboxnet.SboxnetDebugInfo(rmsg.data)
                    print("Debug-Info -- addr=%d puid=%d -----------" % (addr, pr.puid))
                    self.print_dbginfo(d)
                
    def cb_processmsg(self, msg):
        if msg.cmd == sboxnet.SBOXNET_CMD_FB_CHANGED:
            self.process_fb(msg)
            self._sbnm.sendmsg(sboxnet.SboxnetMsg.new(msg.srcaddr, msg.cmd|0x80, msg.seq))
            return True
        if msg.cmd == sboxnet.SBOXNET_CMD_FB_LOCOADDR:
            self.process_fb(msg)
            self._sbnm.sendmsg(sboxnet.SboxnetMsg.new(msg.srcaddr, msg.cmd|0x80, msg.seq))
            return True
        if msg.cmd == sboxnet.SBOXNET_CMD_DEV_SET_ADDR|0x80:
            return True
        return False
    
    def cb_printmsg(self, msg):
        if msg.cmd != sboxnet.SBOXNET_CMD_NET_WATCHDOG:
            self.printmsg(msg)

    def printmsg(self, msg):
        if msg is None:
            return None
        
        outstr = "%s srcaddr=%d destaddr=%d seq=%d len=%d %scmd=%s (0x%x)" % (("-->" if (msg.srcaddr == 0) else "<--"), 
                                                                    msg.srcaddr, msg.dstaddr, msg.seq, msg.dlen,
                                                                    (">" if (msg.cmd & 0x80) else " "), sboxnet.cmd_to_str(msg.cmd & 0x7f), msg.cmd)
        
        if msg.cmd == (0x80|sboxnet.SBOXNET_CMD_DEV_GET_DESC):
            desc = bytes(msg.data[0:msg.dlen]).decode(encoding="ascii")
            outstr = outstr + " dev desc: %s" % (desc)
        
        elif msg.cmd == (0x80|sboxnet.SBOXNET_CMD_REG_READ) or msg.cmd == (0x80|sboxnet.SBOXNET_CMD_REG_READ_MULTI):
            outstr = outstr + ":"
            i = 0
            while i < msg.dlen/2:
                w = makeworda(msg.data, 2*i)
                outstr = outstr + " 0x%x (%d)," % (w, w)
                i = i + 1
        
        elif msg.cmd == sboxnet.SBOXNET_CMD_FB_CHANGED:
            outstr = outstr + ": sensor %d = %d" % (int(msg.data[0] & 0x7f), int((msg.data[0] >> 7) & 1))
        
        elif msg.cmd == sboxnet.SBOXNET_CMD_FB_LOCOADDR:
            outstr = outstr + ": sensor %d = %d, locoaddr=%d" % (int(msg.data[0] & 0x7f), int((msg.data[0] >> 7) & 1),
                                                    makeworda(msg.data, 1))
            
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
        
        logInfo(outstr)
        return msg
    
    def get_ga(self, addr):
        return self._stat_ga.get(int(addr))
    
    def set_ga(self, addr, value):
        v = (int(value) != 0)
        a = int(addr) & (-4) # & 0xfffffffc
        self._stat_ga[a] = v
        self._stat_ga[a+1] = v
        self._stat_ga[a+2] = v
        self._stat_ga[a+3] = v
    
    def sendmsg(self, msg):
        dstaddr = msg.dstaddr
        seq = (self._seq + 1) & 255
        if seq < 10:
            seq = 10
        self._seq = seq
        msg.seq = self._seq
        with self._locksend:
            self._sbnm.sendmsg(msg)
            endtime = datetime.now() + timedelta(milliseconds=500)
            while datetime.now() < endtime:
                if self._sbnm.has_msg():
                    rmsg = self._sbnm.recvmsg()
                    if rmsg is not None:
                        if rmsg.srcaddr == dstaddr and rmsg.seq == seq:
                            return rmsg
                        if rmsg.cmd == 0x80:
                            rcmd = rmsg.data[0]
                            rc = rmsg.data[1]
                            logInfo("** NOT processed message: %s cmd=%s srcaddr:%d dstaddr:%d s:%d l:%d" % (sboxnet.ackrc_to_str(rc), sboxnet.cmd_to_str(rcmd), rmsg.srcaddr, rmsg.dstaddr, rmsg.seq, rmsg.dlen))
                        else:
                            logInfo("** NOT processed message: cmd=0x%x %s srcaddr:%d dstaddr:%d s:%d l:%d" % (rmsg.cmd, sboxnet.cmd_to_str(rmsg.cmd & 0x7f), rmsg.srcaddr, rmsg.dstaddr, rmsg.seq, rmsg.dlen))
        logError("no answer from device %s" % (dstaddr))
        return None
    
    def wait_for_devices(self):
        haveall = False
        logInfo("waiting for devices to logon...")
        while not haveall and not self._shutdown:
            time.sleep(1)
            logInfo("waiting...")
            haveall = True
            for pr in (list(self._init_fb.to_list()) + [self._init_booster, self._init_dccgen]):
                if self._sbnm.get_puid_addr(pr.puid) is None:
                    haveall = False
                    logInfo("- device with puid %s still not available" % (pr.puid))
                else:
                    logInfo("- device with puid %s available" % (pr.puid))
        if haveall:
            logInfo("all devices available.")
        return haveall
        
    def read_booster(self):
        amps = 0.0
        volts = 0.0
        amps0 = 0
        volts0 = 0
        dstaddr = self._sbnm.get_puid_addr(self._init_booster.puid)
        if dstaddr is None:
            logError("booster not found")
            return
        for i in [0, 1]:
            msg = sboxnet.SboxnetMsg.new(dstaddr, sboxnet.SBOXNET_CMD_REG_READ, 0, [41+i, 0, 1])
            rmsg = self.sendmsg(msg)
            if (rmsg is not None) and (rmsg.cmd == (0x80|sboxnet.SBOXNET_CMD_REG_READ)):
                adval = sboxnet.makeworda(rmsg.data, 0)
                if adval < 0:
                    adval = 0.0
                advolt = (2.06 * adval) / 2048.0
                if i == 0:
                    amps = advolt / 0.47
                    amps0 = adval
                elif i == 1:
                    volts = advolt * 31
                    volts0 = adval
        msg = sboxnet.SboxnetMsg.new(dstaddr, sboxnet.SBOXNET_CMD_REG_READ, 0, [203, 0, 1])
        rmsg = self.sendmsg(msg)
        maxsc = 0
        if rmsg is not None:
            maxsc = sboxnet.makeworda(rmsg.data, 0)
        logInfo("Booster: U=%.2fV (%d)   I=%.2fA (%d)    max shortcut: %d" % (volts, volts0, amps, amps0, maxsc))
        
    
    def read_fb(self):
        for puidroc in self._init_fb.to_list():
            if puidroc.senscnt is None:
                continue
            dstaddr = self._sbnm.get_puid_addr(puidroc.puid)
            if dstaddr is None:
                continue
            msg = sboxnet.SboxnetMsg.new(int(dstaddr), sboxnet.SBOXNET_CMD_REG_READ, 0, [lowbyte(sboxnet.R_FB_VALUE0), highbyte(sboxnet.R_FB_VALUE0), 1])
            rmsg = self.sendmsg(msg)
            if rmsg is None or rmsg.dlen != 2:
                continue
            v = sboxnet.makeworda(rmsg.data, 0)
            for i in range(0,16):
                x = (1 if (v & (1<<i)) else 0)
                self._make_fb(puidroc, i, x)
                        
    def process_fb(self, msg):
        if msg.cmd == sboxnet.SBOXNET_CMD_FB_CHANGED or msg.cmd == sboxnet.SBOXNET_CMD_FB_LOCOADDR:
            sensidx = msg.data[0] & 0x7f
            senson = (msg.data[0] >> 7) & 1
            locoaddr = None
            if msg.cmd == sboxnet.SBOXNET_CMD_FB_LOCOADDR:
                locoaddr = makeworda(msg.data, 1)
            puid = self._sbnm.get_addr_puid(msg.srcaddr)
            if puid:
                puidroc = self._init_fb.roca(puid)
                if puidroc:
                    self._make_fb(puidroc, sensidx, senson, locoaddr)
            
    def _make_fb(self, puidroc, i, senson, locoaddr=None):
        # puidroc PuidRoc
        # i Sensorindex
        # is sensor on/off
        # locoaddr or None
        senson = (True if senson else False)
        senscnt = 0
        if i < len(puidroc.senscnt):
            senscnt = puidroc.senscnt[i]
        if senscnt == 0:
            return
        faddr = puidroc.roca * 64 + i * 4
        oldx = self.get_fb(faddr)
        if 1: #(oldx.on != senson) or (senson and (locoaddr is not None) and (locoaddr != oldx.locoaddr)):
            logInfo("fb: value=%s faddr=%d roca=%d i=%d locoaddr=%s report=%s" % (senson,faddr,puidroc.roca,i,locoaddr,puidroc.reports_locoaddr))
            fb = self.set_fb(faddr, senson, locoaddr)
            k = 0
            while k < senscnt and k < 4:
                s = "100 INFO %s FB %d %d" % (self._bus, faddr+k, 1 if senson else 0)
                if puidroc.reports_locoaddr and fb.on and (fb.locoaddr is not None):
                     s = s + " %d" % (int(fb.locoaddr))
                logInfo(s)
                self.notify_info(s)
                k = k + 1
        if puidroc.is_ga:
            ga = self.get_ga(faddr)
            if ga is not None:
                if senson != ga:
                    logError("fb and ga different: faddr=%d senson=%s ga=%s" % (faddr, senson, ga))
                else:
                    logInfo("fb == ga  faddr=%d" % (faddr))
            else:
                logError("no GA for FB faddr=%d" % (faddr))
                self.set_ga(faddr, senson)
        
        
    def get_fb(self, faddr):
        fb = self._stat_fb.get(faddr)
        if fb is None:
            fb = Feedback()
            self._stat_fb[faddr] = fb
        return fb
    
    def set_fb(self, faddr, value, locoaddr=None):
        fb = self.get_fb(faddr)
        if value:
            fb.on = True
            if locoaddr is not None:
                fb.locoaddr = locoaddr
        else:
            fb.on = False
            fb.locoaddr = None
        return fb
        
    def get_gl(self, locoaddr):
        return self._stat_gl.get(int(locoaddr))
 
    def set_gl(self, locoaddr, x):
        self._stat_gl[int(locoaddr)] = x
        
    def run(self):
        if self.wait_for_devices():
            self.cmd_set_power("OFF", reset=1)
            time.sleep(6) # wait some time before init feedbacks, else rocrail shows undefined feedbacks in sensor monitor
            self.read_fb()
            count = 0
            while not self._shutdown:
                #self.read_fb()
                #if (count & 0x03) == 0:
                self.read_booster()
                #if (count & 0x07) == 0:
                #    self.debug_info()
                time.sleep(10)
                count = (count + 1) & 0x0fff
            logInfo("SboxnetUSB: shuting down...")
            self.cmd_set_power("OFF", reset=1)
        self._sbnm.terminate()
    
    def shutdown(self):
        logInfo("SboxnetUSB: shutdown sent")
        with self._lockreg:
            for h in list(self._srcph_dict.values()):
                h["handler"].shutdown()
        self._shutdown = True
    
    def register_srcph(self, srcph):
        with self._lockreg:
            sessid = self._srcph_sessid_cnt
            self._srcph_sessid_cnt = self._srcph_sessid_cnt + 1
            self._srcph_dict[srcph] = { "sessid": sessid, "handler": srcph }
            return sessid
    
    def unregister_srcph(self, srcph):
        with self._lockreg:
            del self._srcph_dict[srcph]
            
    def send_info_init(self, srcph):
        srcph.writeline('100 INFO 0 DESCRIPTION SESSION SERVER TIME')
        srcph.writeline('100 INFO %s DESCRIPTION GA FB POWER' % (self._bus))
        for k in list(self._stat_fb.keys()):
            v = self._stat_fb[k]
            srcph.writeline("100 INFO %s FB %d %d" % (self._bus, k, 1 if v.on else 0))
    
    def notify_info(self, reply):
        with self._lockreg:
            for h in list(self._srcph_dict.values()):
                h["handler"].notify_info(reply)
    
    def cmdreply(self, reply):
        # notify info sessions
        if re.match(r"1\d+ INFO", reply):
            self.notify_info(reply)
        return reply      

    def make_gl_info(self, v):
        drvmode = v["drvmode"]
        speed = v["speed"]
        maxspeed = v["maxspeed"]
        fncnt = v["fncnt"]
        fn = v["fn"]
        info = "%d %d %d" % (drvmode, speed, maxspeed)
        for i in range(0, fncnt):
            x = 0
            if fn & (1 << i):
                x = 1
            info = info + " " + str(x)
        return info

    def command(self, cmd):
        #self._init_fb.printIt()
        #print("----")
        print(cmd)
        #self._sbnm.printEntries()
        #print("++++")
        with self._lockcmd:
            # INIT
            #mo = re.match(r"INIT \d*", cmd)
            #if mo:
                #return r"100 INFO 7 FB"# % (self._bus)
            # GET GL
            mo = re.match(r"GET %s GL (\d+)" % (self._bus), cmd)
            if mo:
                locoaddr = int(mo.group(1))
                v = self.get_gl(locoaddr)
                if v is None:
                    return "416 ERROR no data"
                info = self.make_gl_info(v)
                return "100 INFO %s GL %s %s" % (self._bus, locoaddr, info)
            # INIT GL
            mo = re.match(r"INIT %s GL (\d+) ([A-Z]+) (\d+) (\d+) (\d+)" % (self._bus), cmd)
            if mo:
                locoaddr = int(mo.group(1))
                prot = mo.group(2)
                protver = int(mo.group(3))
                maxspeed = int(mo.group(4))
                fncnt = int(mo.group(5))
                if (prot != "P") or (locoaddr <= 0) or (locoaddr > 10000) or (maxspeed not in [14,28,128]) \
                                 or (fncnt <= 0) or (fncnt > 20):
                    return "412 ERROR wrong value"
                v = { "drvmode": 2, "speed": 0, "maxspeed": maxspeed, "fncnt": fncnt, "fn": 0 }
                self.set_gl(locoaddr, v)
                flags = 0
                if maxspeed == 14:
                    flags |= sboxnet.LOCO_FLG_SPEED_14
                elif maxspeed == 28:
                    flags |= sboxnet.LOCO_FLG_SPEED_28
                else:
                    flags |= sboxnet.LOCO_FLG_SPEED_128
                msg = sboxnet.SboxnetMsg.new(self._sbnm.get_puid_addr(self._init_dccgen.puid), sboxnet.SBOXNET_CMD_LOCO_ADD, 0,
                                         [lowbyte(locoaddr), highbyte(locoaddr), flags])
                self.sendmsg(msg)
                return "101 INFO %s GL %d %s %d %d %d" % (self._bus, locoaddr, prot, protver, maxspeed, fncnt)
            # SET GL
            #print("SET GL")
            mo = re.match(r"SET %s GL (\d+) (\d+) (\d+) (\d+)( .*)?" % (self._bus), cmd)
            if mo:
                #SET 7 GL 12 1 5 28 1 0 0 0 0 
                #print("doing")
                locoaddr = int(mo.group(1)) #12
                drivemode = int(mo.group(2)) # 1
                speed = int(mo.group(3)) # 5
                maxspeed = int(mo.group(4)) #28
                fnstr = mo.group(5) #1
                v = self.get_gl(locoaddr)
                if (v is None) or (drivemode not in [0,1,2]) or speed < 0 or maxspeed < 14 \
                               or (maxspeed > 1000) or (speed > maxspeed):
                    print("locaddr: %d" % (locoaddr))
                    print("drivemode: %d"% (drivemode))
                    print("speed: %d" % (speed))
                    print("maxspeed: %d" % (maxspeed))
                    print("ERROR wrong value")
                    return "412 ERROR wrong value"
                v["drvmode"] = drivemode
                v["speed"] = speed
                v["maxspeed"] = maxspeed
                fncnt = v["fncnt"]
                fnk = shlex.split(fnstr)
                fn = 0
                for i in range(0, min(fncnt, len(fnk))):
                    if int(fnk[i]) != 0:
                        fn |= (1 << i)
                v["fn"] = fn
                self.set_gl(locoaddr, v)
                
                drive = 0
                if drivemode == 2: # emergency stop
                    drive = 1
                else:
                    drive = int(speed * 127 / maxspeed)
                    if drive > 0x7f:
                        drive = 0x7f
                    elif drive == 1:
                        drive = 0
                if drivemode == 1:
                    drive |= 0x80
                msg = sboxnet.SboxnetMsg.new(self._sbnm.get_puid_addr(self._init_dccgen.puid), sboxnet.SBOXNET_CMD_LOCO_DRIVE, 0,
                                         [lowbyte(locoaddr), highbyte(locoaddr), drive, lowbyte(fn), highbyte(fn)])
                self.sendmsg(msg)
                
                info = self.make_gl_info(v)
                return "100 INFO %s GL %d %s" % (self._bus, locoaddr, info)
            # GET GA
            mo = re.match(r"GET %s GA (\d+) (\d+)" % (self._bus), cmd)
            if mo:
                addr = mo.group(1)
                port = mo.group(2)
                v = self.get_ga(addr)
                if v is None:
                    return "416 ERROR no data"
                return "100 INFO %s GA %s %s %d"% (self._bus, addr, 0, 1 if v else 0)
            # SET GA
            mo = re.match(r"SET %s GA (\d+) (\d+) (\d+) (-?\d+)" % (self._bus), cmd)
            if mo:
                addr = mo.group(1)
                port = mo.group(2)
                value = mo.group(3)
                delay = mo.group(4)
                self.set_ga(addr, value)
                print("---- addr=%s port=%s value=%s delay=%s" % (addr,port,value,delay))
                # send it
                roca, bitnr = divmod(int(addr), 64)
                #print("++++1 vor bitnr=%d" % (bitnr))
                bitnr = int(bitnr / 4)
                #print("++++2 bitnr=%d" % (bitnr))
                pr = self._init_fb.puid(roca)
                #print("---- roca=%d  bit=%d puid=%d" % (roca,bitnr,pr.puid))
                if pr is None:
                    return "416 ERROR no data"
                puid = pr.puid
                data = [lowbyte(sboxnet.R_CNTRL_VALUE0), highbyte(sboxnet.R_CNTRL_VALUE0), bitnr & 0x7f]
                if int(value) != 0:
                    data[2] = data[2] | 0x80
                if puid is None:
                    print("-- puid is None")
                if self._sbnm.get_puid_addr(puid) is None:
                    print("-- self._sbnm.get_puid_addr(puid) is None")
                if puid is not None and self._sbnm.get_puid_addr(puid) is not None:
                    print("addr=%d puid=%d" % (self._sbnm.get_puid_addr(puid), puid))
                msg = sboxnet.SboxnetMsg.new(self._sbnm.get_puid_addr(puid), sboxnet.SBOXNET_CMD_REG_WRITE_BIT, 0, data)
                self.sendmsg(msg)
                return "100 INFO %s GA %s %s %s"% (self._bus, addr, port, value)
            # INIT GA
            mo = re.match(r"INIT %s GA (\d+) ([A-Z])" % (self._bus), cmd)
            if mo:
                addr = mo.group(1)
                prot = mo.group(2)
                if prot != "P":
                    return "420 ERROR unsupported device protocol"
                return "101 INFO %s GA %s %s" % (self._bus, addr, prot)
            # GET POWER
            mo = re.match(r"GET (\d+) POWER", cmd)
            if mo:
                bus = mo.group(1)
                return self.cmdreply("100 INFO %s POWER %s" % (bus,self._power))
            # INIT POWER
            mo = re.match(r"INIT (\d+) POWER", cmd)
            if mo:
                bus = int(mo.group(1))
                if bus == self._bus:
                    self._power = "OFF"
                    self.cmd_set_power("OFF", reset=0)
                return self.cmdreply("101 INFO %s POWER" % (bus))
            # SET POWER
            mo = re.match(r"SET (\d+) POWER (ON|OFF)( .*)?", cmd)
            if mo:
                bus = int(mo.group(1))
                onoff = mo.group(2)
                freetext = mo.group(3)
                if freetext is None:
                    freetext = ""
                if bus == self._bus:
                    self._power = onoff
                    self.cmd_set_power(onoff, reset=0)
                return self.cmdreply("100 INFO %s POWER %s%s" % (bus,onoff,freetext))
            # SET SM
            mo = re.match(r"SET %s SM (\d+) CV (\d+) (\d+)" % (self._bus), cmd)
            if mo:
                locoaddr = int(mo.group(1))
                cv = int(mo.group(2))
                cvd = int(mo.group(3))
                if cv <= 0 or cv > 0x400:
                    return "412 ERROR wrong value"
                if cv == 1 or cv == 17 or cv == 18:
                    return "412 ERROR wrong value"
                cv1 = cv - 1
                msg = sboxnet.SboxnetMsg.new(self._sbnm.get_puid_addr(self._init_booster.puid), sboxnet.SBOXNET_CMD_LOCO_POM, 0
                                [lowbyte(locoaddr), highbyte(locoaddr),
                                 lowbyte(cv1 & 0x03ff)|0xc, highbyte(cv1), cvd])
                self.sendmsg(msg)
                return "100 INFO %s SM %s CV %s %s" % (self._bus, locoaddr, cv, cvd)
            #                
            return "410 ERROR unknown command"
    
    def cmd_set_power(self, onoff, reset):
        v = 0
        if not reset:
            v |= 0x02
        if onoff == "ON":
            v |= 0x01
        dstaddr = self._sbnm.get_puid_addr(self._init_dccgen.puid)
        if dstaddr is None:
            logError("dccgen not available!")
            return
        msg = sboxnet.SboxnetMsg.new(dstaddr, sboxnet.SBOXNET_CMD_LOCO_POWER, 0, [v])
        self.sendmsg(msg)
        dstaddr = self._sbnm.get_puid_addr(self._init_booster.puid)
        if dstaddr is None:
            logError("booster not available!")
            return
        msg = sboxnet.SboxnetMsg.new(dstaddr, sboxnet.SBOXNET_CMD_LOCO_POWER, 0, [v])
        self.sendmsg(msg)
        
    

class SrcpRequestHandler(socketserver.StreamRequestHandler):
    def handle(self):
        # self.request is the TCP socket connected to the client
        # self.rfile and self.wfile are stream objects
        srcph = SrcpHandler(self)
        srcph.serve()


class ThreadedTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    def set_sboxnet(self, sboxnet):
        self.sboxnet = sboxnet


class Srcpd(object):
    def __init__(self, host, port):
        self._host = host
        self._port = port
        self._server = None
        self._serverthread = None
        self._sbnusb = None
        logInfo("Starting Srcpd-sboxnet...")
    
    def run(self):
        self._sbnusb = Sboxnet(self)

        self._server = ThreadedTCPServer((self._host, self._port), SrcpRequestHandler)
        self._server.set_sboxnet(self._sbnusb)
        self._serverthread = threading.Thread(target=self._server.serve_forever)
        self._serverthread.daemon = False
        self._serverthread.start()
        
        self._sbnusb.run()
        logInfo("Sboxnet stopped")
        
        self._server.shutdown()
        logInfo("Srcpd Daemon stopped")
        logInfo("Srcpd-sboxnet terminated.")
        
    def shutdown(self):
        logInfo("Srcpd-sboxnet shutdown request.")
        self._server.shutdown()
        self._sbnusb.shutdown()
        

# --- main ---

if __name__ == "__main__":
    HOST, PORT = "localhost", 4303

    logfh = open("sboxnet.log", "a")

    srcpd = Srcpd(HOST, PORT)

    def signal_sigint(sig, frame):
        srcpd.shutdown()
    
    signal.signal(signal.SIGINT, signal_sigint)

    srcpd.run()
    
    logfh.close()


