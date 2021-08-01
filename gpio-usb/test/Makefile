INC=-I../..

XCOPTS= -Os -fno-inline-small-functions -Wall -std=gnu99 $(INC) -save-temps \
         -fno-move-loop-invariants -fno-tree-loop-optimize -fno-jump-tables \
         -fno-tree-ter \
         -fno-caller-saves \
         -mstrict-X

XMCU=atxmega16a4

xmega.avr: xmega.c
	avr-gcc $(XCOPTS) -Wa,-als=xmega.lst -c -o xmega.o $<  -mmcu=$(XMCU)
	avr-gcc $(XCOPTS) -Wl,-Map,xmega.map -o $@ xmega.o  -mmcu=$(XMCU)
	avr-readelf -S $@

xmegaprog: xmega.avr
	mcp avr --chipprops=$(HOME)/bin/avrchips.properties --prgdev=gpio-usb-xmega --chip=$(XMCU)
