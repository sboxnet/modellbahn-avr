
INC=-I../..

COPTS= -Os -fno-inline-small-functions -Wall -std=gnu99 $(INC) -save-temps \
         -fno-move-loop-invariants -fno-tree-loop-optimize -fno-jump-tables \
         -fno-tree-ter \
         -fno-caller-saves \
         -mstrict-X

all: test-sboxnet.avr xmega-sboxnet.avr

test-sboxnet.avr: test-sboxnet.c
	avr-gcc $(COPTS) -Wa,-als=test-sboxnet.lst -o $@ test-sboxnet.c  -mmcu=atmega88pa
	avr-readelf -S $@

xmega-sboxnet.avr: test-sboxnet.c
	avr-gcc $(COPTS) -Wa,-als=xmega-sboxnet.lst -o $@ test-sboxnet.c  -mmcu=atxmega16a4
	avr-readelf -S $@

clean:
	/bin/rm -f *.o *.si *.i *.s *.avr *.lst

prog-sboxnet:
	mcp avr --chipprops=/home/balagi/bin/avrchips.properties --prgdev=gpio-usb-avr --chip=atmega88pa \
		--noask  --cmd="erase flash ; progflash test-sboxnet.avr ; showeeprom 0 4"

xmegaprog: xmega-sboxnet.avr
	mcp avr --chipprops=/home/balagi/bin/avrchips.properties --prgdev=gpio-usb-xmega --chip=atxmega16a4 \
		--noask  --cmd="progflash xmega-sboxnet.avr"
