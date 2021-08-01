#!/usr/bin/python3 -tt

import sys
import array

# from AVR libc function: _crc_ccitt_update()
# Polynomial: x^16 + x^12 + x^5 + 1 (0x8408)
# Initial value: 0xffff
def crc_ccitt_update(crc, data):
    data = data & 0x00ff
    data = data ^ (crc & 0x00ff)
    data = data ^ ((data << 4) & 0x00ff)
    return (((data << 8) | ((crc >> 8) & 0x00ff)) ^ ((data >> 4) & 0x00ff) ^ (data << 3)) & 0x0ffff

def calc_crc(infile, outfile):
    crc = 0xffff
    with open(infile, "rb") as infh:
        with open(outfile, "wb") as outfh:
            while True:
                ba = infh.read(1)
                if ba is None or len(ba) == 0:
                    ba = array.array('B', [ crc & 0xff, (crc >> 8) & 0xff ])
                    outfh.write(ba)
                    break
                crc = crc_ccitt_update(crc, ba[0])
                outfh.write(ba)

def check_crc(infile):
    crc = 0xffff
    with open(infile, "rb") as infh:
        while True:
            ba = infh.read(1)
            if ba is None or len(ba) == 0:
                print("CRC: 0x%x" % (crc))
                break
            crc = crc_ccitt_update(crc, ba[0])

if len(sys.argv) == 4 and sys.argv[1] == 'calc':
    calc_crc(sys.argv[2], sys.argv[3])
elif len(sys.argv) == 3 and sys.argv[1] == 'check':
    check_crc(sys.argv[2])
else:
    print("usage: crc-ccitt calc <inputfile> <outputfile>")
    print("       crc-ccitt check <inputfile>")
    exit(1)
    
exit(0)
