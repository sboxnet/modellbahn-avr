
all: client usb_enum

client: client.cpp
	g++ -O -Wall -I/usr/include/libusb-1.0 -o $@ $<  -lusb-1.0

usb_enum: usb_enum.cpp
	g++ -O -Wall -I/usr/include/libusb-1.0 -o $@ $<  -lusb-1.0
