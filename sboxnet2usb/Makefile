
PRJNAME=sboxnet2usb
MCU=atmega32u2

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
	mcp avr --chipprops=/home/balagi/bin/avrchips.properties --prgdev=gpio-usb-avr --chip=$(MCU)

progdfu:
	dfu-programmer $(MCU) erase
	dfu-programmer $(MCU) flash $(PRJNAME).hex
	dfu-programmer $(MCU) start

