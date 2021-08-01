
import sys
import array
import usb.core
import usb.util

class PdiError(Exception):
    pass

class pdi(object):
    VENDORID  = 0xf123
    PRODUCTID = 0xa001
    
    CMD_PDI_ENABLE         = 0x40
    CMD_PDI_DISABLE        = 0x41
    CMD_PDI_LDS            = 0x42
    CMD_PDI_STS            = 0x43
    CMD_PDI_LD             = 0x44
    CMD_PDI_ST             = 0x45
    CMD_PDI_LDCS           = 0x46
    CMD_PDI_STCS           = 0x47
    CMD_PDI_REPEAT         = 0x48
    CMD_PDI_KEY            = 0x49
    CMD_PDI_GETSTATUS      = 0x4a
    CMD_ENTER_PDI          = 0x4b
    CMD_LEAVE_PDI          = 0x4c
    
    CMD_NVMC_ENABLE           = 0x50
    CMD_NVMC_DISABLE          = 0x51
    CMD_NVMC_FLASH_READ       = 0x52
    CMD_NVMC_FLASH_WRITE_PAGE = 0x53
    CMD_NVMC_EEPROM_READ      = 0x54
    CMD_NVMC_EEPROM_WRITE_PAGE  = 0x55
    CMD_NVMC_FUSE_READ        = 0x56
    CMD_NVMC_FUSE_WRITE       = 0x57
    CMD_NVMC_SIGNROW_READ     = 0x58
    CMD_NVMC_NVM_READ         = 0x59
    CMD_NVMC_GET_DEVID        = 0x60
    CMD_NVMC_ERASE_CHIP       = 0x61
 
    def __init__(self):
        self._dev = usb.core.find(idVendor=self.VENDORID, idProduct=self.PRODUCTID)
        if self._dev is None:
            raise PdiError('USB device not found!')
        self._dev.set_configuration() # set first configuration
    
    def ctrl(self, cdir, bRequest, wValue=0, wIndex=0, data_or_wLength = None, timeout = None):
        return self._dev.ctrl_transfer(cdir|usb.util.CTRL_TYPE_VENDOR|usb.util.CTRL_RECIPIENT_INTERFACE,
                                bRequest, wValue, wIndex, data_or_wLength, timeout)
    
    def pdi_enter(self):
        self.ctrl(usb.util.CTRL_OUT, self.CMD_ENTER_PDI)
    
    def pdi_leave(self):
        self.ctrl(usb.util.CTRL_OUT, self.CMD_LEAVE_PDI)
 
    def pdi_enable(self):
        self.ctrl(usb.util.CTRL_OUT, self.CMD_PDI_ENABLE)
    
    def pdi_disable(self):
        self.ctrl(usb.util.CTRL_OUT, self.CMD_PDI_DISABLE)
    
    def lds(self, addr, numbytes=1):
        return self.ctrl(usb.util.CTRL_IN, self.CMD_PDI_LDS, (addr >> 16) & 0x0ffff, addr & 0x0ffff, numbytes & 0x03)
    
    def sts(self, addr, data):
        self.ctrl(usb.util.CTRL_OUT, self.CMD_PDI_STS, (addr>>16)&0x0ffff, addr&0xffff, array.array('B', data[0:3]))
    
    def ld(self, pointer, numbytes=1):
        return self.ctrl(usb.util.CTRL_IN, self.CMD_PDI_LD, pointer, 0, numbytes&0x03)
    
    def st(self, pointer, data):
        self.ctrl(usb.util.CTRL_OUT, self.CMD_PDI_ST, pointer, 0, array.array('B', data[0:3]))
        
    def ldcs(self, register):
        d = self.ctrl(usb.util.CTRL_IN, self.CMD_PDI_LDCS, register, 0, 1)
        return d[0]
    
    def stcs(self, register, d):
        self.ctrl(usb.util.CTRL_OUT, self.CMD_PDI_STCS, register, d)

    def repeat(self, num):
        a = array.array('B', [num&0x0ff, (num>>8)&0xff, (num>>16)&0xff, (num>>24)&0xff])
        self.ctrl(usb.util.CTRL_OUT, self.CMD_PDI_REPEAT, 0, 0, a)
    
    def key(self, k):
        a = array.array('B', [k&0xff, (k>>8)&0xff, (k>>16)&0xff, (k>>24)&0xff,
                              (k>>32)&0xff, (k>>40)&0xff, (k>>48)&0xff, (k>>56)&0xff])
        self.ctrl(usb.util.CTRL_OUT, self.CMD_PDI_KEY, 0, 0, a)

    def status(self):
        a = self.ctrl(usb.util.CTRL_IN, self.CMD_PDI_GETSTATUS, 0, 0, 1)
        return a[0]
    
    NVM_SECTION_FLASH  = 0x0800000
    NVM_SECTION_EEPROM = 0x08c0000
    NVM_SECTION_CALROW = 0x08e0200 # production / calibration signature row
    NVM_SECTION_SIGROW = 0x08e0400 # user signature row
    NVM_SECTION_FUSES  = 0x08f0020
    NVM_SECTION_DATA   = 0x1000000
    
    NVM_KEY = 0x1289ab45cdd888ff
    
    NVMC_OFFSET = 0x01c0
    
    def nvmc_enable(self):
        self.ctrl(usb.util.CTRL_OUT, self.CMD_NVMC_ENABLE)
    
    def nvmc_disable(self):
        self.ctrl(usb.util.CTRL_OUT, self.CMD_NVMC_DISABLE)

    def nvmc_nvm_read(self, addr, len):
        a = self.ctrl(usb.util.CTRL_IN, self.CMD_NVMC_NVM_READ, (addr>>16), (addr & 0xffff), len)
        return a

    def nvmc_flash_read(self, addr, len):
        a = self.ctrl(usb.util.CTRL_IN, self.CMD_NVMC_FLASH_READ, (addr>>16), (addr & 0xffff), len)
        return a

    def nvmc_eeprom_read(self, addr, len):
        a = self.ctrl(usb.util.CTRL_IN, self.CMD_NVMC_EEPROM_READ, (addr>>16), (addr & 0xffff), len)
        return a
    
    def nvmc_fuse_read(self, addr):
        a = self.ctrl(usb.util.CTRL_IN, self.CMD_NVMC_FUSE_READ, addr & 0xffff, 0, 1)
        return a
    
    def nvmc_sign_read(self, addr, len):
        a = self.ctrl(usb.util.CTRL_IN, self.CMD_NVMC_SIGNROW_READ, addr & 0xffff, 0, len)
        return a
    
    def nvmc_get_devid(self):
        a = self.ctrl(usb.util.CTRL_IN, self.CMD_NVMC_GET_DEVID, 0, 0, 4)
        return a

    def nvmc_flash_write_page(self, addr, data):
        self.ctrl(usb.util.CTRL_OUT, self.CMD_NVMC_FLASH_WRITE_PAGE,
                      (addr>>16), (addr & 0xffff), array.array('B', data))
        
    def nvmc_eeprom_write_page(self, addr, data):
        self.ctrl(usb.util.CTRL_OUT, self.CMD_NVMC_EEPROM_WRITE_PAGE,
                      (addr>>16), (addr & 0xffff), array.array('B', data))
    
    def nvmc_fuse_write(self, addr, b):
        self.ctrl(usb.util.CTRL_IN, self.CMD_NVMC_FUSE_WRITE, addr & 0xffff, b & 0xff, 0)
    
    def nvmc_erase_chip(self):
        self.ctrl(usb.util.CTRL_IN, self.CMD_NVMC_ERASE_CHIP)


def px(a):
    i = 0
    for x in a:
        print("0x%x," % x, end='');
        i = i + 1
        if i == 16:
            print('')
            i = 0
    print('')

