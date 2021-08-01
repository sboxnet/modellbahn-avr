
INC=-I../../..
COPTS= -O2 -fno-inline-small-functions -Wall -std=gnu99 $(INC) -g

all: test-usb.avr

test-usb.avr: test-usb.c
	avr-gcc $(COPTS) -Wa,-als=test-usb.lst -c -o test-usb.o $<  -mmcu=at90usb162
	avr-gcc $(COPTS) -Wl,-Map,test-usb.map -o $@ test-usb.o  -mmcu=at90usb162
	avr-objcopy -j .text -j .data -O ihex $@  test-usb.hex
	avr-readelf -S $@

clean:
	/bin/rm -f *.o *.si *.avr *.lst *.map

prog:
	mcp avr --chipprops=/home/balagi/bin/avrchips.properties --prgdev=gpio-usb-avr-raw --chip=at90usb162 \
		--noask  --cmd="erase chip ; progflash test-usb.avr"

progdfu:
	dfu-programmer at90usb162 erase
	dfu-programmer at90usb162 flash test-usb.hex
	dfu-programmer at90usb162 start

cats0:
	stty -F /dev/ttyS0 speed 38400 clocal raw cs8 -parenb
	cat /dev/ttyS0

