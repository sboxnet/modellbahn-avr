DEVICENAMES=booster gbmelder weichen-servo dccgen dgboost

.PHONY: $(DEVICENAMES)

FLASH=32

INC=-I..

MCU=atxmega$(FLASH)a4
ARCH=avrxmega2

# avr-gcc options: see http://www.mikrocontroller.net/articles/AVR-GCC-Codeoptimierung
COPTS= -Os -fno-inline-small-functions -Wall -std=gnu99 $(INC) -save-temps=obj \
         -fno-move-loop-invariants -fno-tree-loop-optimize -fno-jump-tables \
         -fno-tree-ter \
         -fno-caller-saves \
         -mstrict-X \
         -mmcu=$(MCU)
         #-mrelax       # nicht benutzen, zerstoert die Boot Vector Table !
         #-fno-optimize-sibling-calls

comma:= ,

ifeq ($(FLASH),32)
LDFLAGSX=-Wl,--section-start=.appcrc=6ffe \
        -Wl,--section-start=.bldrapp=7000 \
        -Wl,--section-start=.bldrinit=8000 \
        -Wl,--section-start=.bldrvec=8200\
        -Wl,--section-start=.bootloader=8240 \
        -Wl,--section-start=.bldrcrc=8dfe \
        -Wl,--section-start=.bldrstatic=8e00 \
        -Wl,--section-start=.bldrstatvec=8ff8 \
        -Wl,--section-start=.bldrfwheader=9000 \
        -Wl,--section-start=.fwheader=9100
FLASHADDR=0x3800

else
$(error Variable FLASH with invalid content)
endif

LDFLAGS=$(subst -Wl$(comma),,$(LDFLAGSX))


OBJCOPYBLDR=avr-objcopy -j .bldrapp -j .bldrinit -j .bldrvec -j .bootloader -j .bldrstatic -j .bldrstatvec --gap-fill 255 -O binary
OBJCOPYBLDRFW=avr-objcopy -j .bldrapp -j .bldrinit  -j .bldrvec -j .bootloader --gap-fill 255 --pad-to=0x8dfe -O binary
OBJCOPYAPP=avr-objcopy -j .text -j .data --gap-fill 255 -O binary
OBJCOPYBLDRFWH=avr-objcopy -j .bldrfwheader -O binary
OBJCOPYAPPFWH=avr-objcopy -j .fwheader -O binary
CRCCALC=./crc-ccitt.py

SRCFILES=bootloader-crt0.c bootloader-fw.c common.c weichen-servo.c gbmelder.c booster.c

all: $(DEVICENAMES)

$(DEVICENAMES): %: build/%-$(FLASH).avr

build/%.o: %.c
	mkdir -p build
	avr-gcc $(COPTS) -o $@ -c $<
build/booster.o: booster.c
	mkdir -p build
	avr-gcc $(COPTS) -DUSE_BOOSTER -o $@ -c $<
build/dccgen.o: booster.c
	mkdir -p build
	avr-gcc $(COPTS) -DUSE_DCCGEN -o $@ -c $<
build/dgboost.o: booster.c
	mkdir -p build
	avr-gcc $(COPTS) -DUSE_BOOSTER -DUSE_DCCGEN -o $@ -c $<


$(SRCFILES): common.h

%-$(FLASH).avr: %.o build/common.o build/bootloader-$(FLASH).avr
	avr-gcc $(LDFLAGSX) -mmcu=$(MCU) -o $@ $^
	$(OBJCOPYAPP) $@ $(@:.avr=.bin)
	$(OBJCOPYAPPFWH) $@ $(@:.avr=.fwh)
	cat $(@:.avr=.fwh) $(@:.avr=.bin) > $(@:.avr=.tmp)
	$(CRCCALC) calc $(@:.avr=.tmp) $(@:.avr=.fwu)
	cp $(@:.avr=.bin) $(@:.avr=.fwu) .
	avr-readelf -S $@

build/bootloader-$(FLASH).avr: build/bootloader-crt0.o build/bootloader-fw.o
	avr-ld $(LDFLAGS) -m$(ARCH) -o build/bootloader-$(FLASH).avr build/bootloader-crt0.o build/bootloader-fw.o
	$(OBJCOPYBLDR) $@ $(@:.avr=.bin)
	$(OBJCOPYBLDRFW) $@ $(@:.avr=-fwu.tmp)
	$(OBJCOPYBLDRFWH) $@ $(@:.avr=.fwh)
	cat $(@:.avr=.fwh) $(@:.avr=-fwu.tmp) > $(@:.avr=-fwu2.tmp)
	$(CRCCALC) calc $(@:.avr=-fwu2.tmp) $(@:.avr=.fwu)
	cp $(@:.avr=.bin) $(@:.avr=.fwu) .
	avr-readelf -S $@

clean:
	/bin/rm -rf build *.o *.i *.s *.si *.avr *.lst *.bin *.fwu *.fwh *.tmp

prog:
	mcp avr --chipprops=/home/balagi/bin/avrchips.properties --prgdev=gpio-usb-xmega --chip=$(MCU)

progfuses:
	echo "y" >cmdfile
	echo "readfuses" >>cmdfile
	echo "writefuse 2 0xbf" >>cmdfile
	echo "writefuse 4 0xf2" >>cmdfile
	echo "writefuse 5 0xe2" >>cmdfile
	echo "readfuses" >>cmdfile
	mcp avr --chipprops=/home/balagi/bin/avrchips.properties --prgdev=gpio-usb-xmega --chip=$(MCU) <cmdfile
	
#	mcp avr --chipprops=/home/balagi/bin/avrchips.properties --prgdev=gpio-usb-xmega --chip=$(MCU)
#	     --cmd="readfuses; writefuse 2 0xbf; writefuse 4 0xf2; writefuse 5 0xe2; readfuses"

progboot: build/bootloader-$(FLASH).avr
	mcp avr --chipprops=/home/balagi/bin/avrchips.properties --prgdev=gpio-usb-xmega --chip=$(MCU)  \
	     --cmd="progflash bin:bootloader-$(FLASH).bin $(FLASHADDR) ; puteeprom 0x300 0xff"

progws: build/weichen-servo-$(FLASH).avr
	mcp avr --chipprops=/home/balagi/bin/avrchips.properties --prgdev=gpio-usb-xmega --chip=$(MCU) \
	     --cmd="progflash bin:bootloader-$(FLASH).bin $(FLASHADDR) ; progflash bin:$(<:.avr=.bin) ; puteeprom 0x300 0"

proggbm: build/gbmelder-$(FLASH).avr
	mcp avr --chipprops=/home/balagi/bin/avrchips.properties --prgdev=gpio-usb-xmega --chip=$(MCU) \
	     --cmd="progflash bin:bootloader-$(FLASH).bin $(FLASHADDR) ; progflash bin:$(<:.avr=.bin) ; puteeprom 0x300 0"

progbo: build/booster-$(FLASH).avr
	mcp avr --chipprops=/home/balagi/bin/avrchips.properties --prgdev=gpio-usb-xmega --chip=$(MCU) \
	     --cmd="progflash bin:bootloader-$(FLASH).bin $(FLASHADDR) ; progflash bin:$(<:.avr=.bin) ; puteeprom 0x300 0"

progdg: build/dccgen-$(FLASH).avr
	mcp avr --chipprops=/home/balagi/bin/avrchips.properties --prgdev=gpio-usb-xmega --chip=$(MCU) \
	     --cmd="progflash bin:bootloader-$(FLASH).bin $(FLASHADDR) ; progflash bin:$(<:.avr=.bin) ; puteeprom 0x300 0"

progdb: build/dgboost-$(FLASH).avr
	mcp avr --chipprops=/home/balagi/bin/avrchips.properties --prgdev=gpio-usb-xmega --chip=$(MCU) \
	     --cmd="progflash bin:bootloader-$(FLASH).bin $(FLASHADDR) ; progflash bin:$(<:.avr=.bin) ; puteeprom 0x300 0"


