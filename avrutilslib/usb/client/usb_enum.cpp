
#include <iostream>

#include <libusb.h>

using namespace std;

void pusberror(int rc, const char* s) {
    cout << s << ":" << rc << ":" << libusb_error_name(rc) << endl;
}

int main(void) {
    libusb_context* usbctx;
    int rc;
    
    rc = libusb_init(&usbctx);
    if (rc != 0) {
        pusberror(rc, "init failed");
        return 1;
    }
    
    libusb_device** devlist;
    rc = libusb_get_device_list(usbctx, &devlist);
    if (rc < 0) {
        pusberror(rc, "get_device_list failed");
        goto error_exit;
    }
    for (int i = 0, cnt = rc; i < cnt; i++) {
        libusb_device* d = devlist[i];
        libusb_device_descriptor desc;
        rc = libusb_get_device_descriptor(d, &desc);
        if (rc != 0) {
            pusberror(rc, "get_device_descriptor failed");
        } else {
            cout << "Dev: bcdUSB=" << hex << desc.bcdUSB << dec
                 << " bMaxPacketSize0=" << (int)desc.bMaxPacketSize0
                 << " idVendor=" << (int)desc.idVendor
                 << " idProduct=" << (int)desc.idProduct
                 << " numConfigs=" << (int)desc.bNumConfigurations
                 << endl;
            
            libusb_device_handle* dh;
            rc = libusb_open(d, &dh);
            if (rc != 0) {
                pusberror(rc, "can not open usb device");
            } else {
                unsigned char buf[128];
                if (desc.iManufacturer) {
                    rc = libusb_get_string_descriptor_ascii(dh, desc.iManufacturer, buf, sizeof(buf)-1);
                    if (rc > 0) {
                        buf[rc] = 0;
                        cout << " Manufacturer=" << buf << endl;
                    }
                }
                if (desc.iProduct) {
                    rc = libusb_get_string_descriptor_ascii(dh, desc.iProduct, buf, sizeof(buf)-1);
                    if (rc > 0) {
                        buf[rc] = 0;
                        cout << " Product=" << buf << endl;
                    }
                }
                libusb_close(dh);
            }
        }
    }
    libusb_free_device_list(devlist, 1);
    
    libusb_exit(usbctx);
    return 0;
    
error_exit:
    libusb_exit(usbctx);
    return 1;
}

