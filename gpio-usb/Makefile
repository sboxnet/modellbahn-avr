
PRJNAME=gpio-usb
MCU=at90usb162

INC=-I..

COPTS= -Os -fno-inline-small-functions -Wall -std=gnu99 $(INC) -save-temps \
         -fno-move-loop-invariants -fno-tree-loop-optimize -fno-jump-tables \
         -fno-tree-ter \
         -fno-caller-saves \
         -mstrict-X

all: $(PRJNAME).avr

$(PRJNAME).avr: $(PRJNAME).c
	avr-gcc $(COPTS) -Wa,-als=$(PRJNAME).lst -c -o $(PRJNAME).o $<  -mmcu=$(MCU)
	avr-gcc $(COPTS) -Wl,-Map,$(PRJNAME).map -o $@ $(PRJNAME).o  -mmcu=$(MCU)
	avr-objcopy -j .text -j .data -O ihex $@  $(PRJNAME).hex
	avr-readelf -S $@

clean:
	/bin/rm -f *.o *.si *.avr *.lst *.map

progmcp:
	mcp avr --chipprops=/home/balagi/bin/avrchips.properties --prgdev=gpio-ser-avr --chip=$(MCU) \
		--noask  --cmd="erase chip ; progflash $(PRJNAME).avr"

progdfu:
	dfu-programmer $(MCU) erase
	dfu-programmer $(MCU) flash $(PRJNAME).hex
	dfu-programmer $(MCU) start

cats0:
	stty -F /dev/ttyS0 speed 38400 clocal raw cs8 -parenb
	cat /dev/ttyS0

tpdi:
	cd test; python -i -m testpdi

