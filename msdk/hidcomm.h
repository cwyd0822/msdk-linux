#ifndef _HID_COMM_H_
#define _HID_COMM_H_

#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <libusb.h> // used libusb(1.0) to operate usb device
libusb_device_handle *HID_Open(unsigned short vid, unsigned short pid);
int HID_Close(libusb_device_handle *l_devh);
int HID_Ctrl_Comm(libusb_device_handle *l_devh,
    unsigned long  cbCommAPDU,	//apdu len
    unsigned char  *pbCommAPDU,	//apdu cmd
    unsigned long *cbRespAPDU,	//resp len
    unsigned char  *pbRespAPDU,	//resp buf
    unsigned long  dw_Timeout	//TimeOutValue
);

#endif
