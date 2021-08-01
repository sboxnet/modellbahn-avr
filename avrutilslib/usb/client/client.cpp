
#include <iostream>

#include <unistd.h>

#include <libusb.h>

using namespace std;


#define VENDOR_ID   0xf123
#define PRODUCT_ID  1


void pusberror(int rc, const char* s) {
    cerr << s << ":" << rc << ":" << libusb_error_name(rc) << endl;
}

int main(void) {
    libusb_context* usbctx;
    int rc;
    int reqbytes;
    
    rc = libusb_init(&usbctx);
    if (rc != 0) {
        pusberror(rc, "ERROR: init failed");
        return 1;
    }
    
    libusb_device** devlist;
    libusb_device_handle* dh = NULL;
    rc = libusb_get_device_list(usbctx, &devlist);
    if (rc < 0) {
        pusberror(rc, "ERROR: get_device_list failed");
        goto error_exit;
    }
    for (int i = 0, cnt = rc; i < cnt; i++) {
        libusb_device* d = devlist[i];
        libusb_device_descriptor desc;
        rc = libusb_get_device_descriptor(d, &desc);
        if (rc != 0) {
            pusberror(rc, "ERROR: get_device_descriptor failed");
        } else {
            if (desc.idVendor == VENDOR_ID && desc.idProduct == PRODUCT_ID) {
                cerr << "found USB device" << endl;
                rc = libusb_open(d, &dh);
                if (rc != 0 || dh == NULL) {
                    pusberror(rc, "ERROR: can not open usb device");
                    libusb_free_device_list(devlist, 1);
                    goto error_exit;
                }
                break;
            }
        }
    }
    libusb_free_device_list(devlist, 1);

    if (dh == NULL) {
        cerr << "ERROR: usb device not found" << endl;
        goto error_exit;
    }
    
    reqbytes = 4096;
    while(reqbytes > 0) {
        uint8_t buf[64];
        int transfered = 0;
        rc = libusb_interrupt_transfer(dh, 0x81, buf, sizeof(buf), &transfered, 10000);
        if (rc == 0) {
            cerr << "got " << transfered << " bytes: " << reqbytes << endl;
            for (int i = 0; i < transfered; i++) {
                cout << buf[i];
            }
            reqbytes -= transfered;
        } else {
            pusberror(rc, "transfer failed");
            break;
        }
    //    ::sleep(1);
    }
    
    libusb_exit(usbctx);
    return 0;
    
error_exit:
    libusb_exit(usbctx);
    return 1;
}

