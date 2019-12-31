#include "hidcomm.h"
//仅支持1个USB设备
libusb_device_handle *HID_Open(unsigned short vid, unsigned short pid)
{
    int ret;
    libusb_device_handle *l_devh;

    ret = libusb_init(NULL);
    if (ret < 0) {
        fprintf(stderr, "Error initializing libusb: %s\n", libusb_error_name(ret));
        return NULL;
    }

    l_devh = libusb_open_device_with_vid_pid(NULL, vid, pid);
    if (!l_devh) {
        fprintf(stderr, "Error finding USB device\n");
        libusb_exit(NULL);
        return NULL;
    }

    if (libusb_kernel_driver_active(l_devh, 0) == 1) { //kernel driver is active
        ret = libusb_detach_kernel_driver(l_devh, 0);
        if (ret < 0) {
            fprintf(stderr, "Error Detach kernel driver: %s\n", libusb_error_name(ret));
            libusb_close(l_devh);
            l_devh = NULL;
            libusb_exit(NULL);
            return NULL;
        }
    }

    ret = libusb_claim_interface(l_devh, 0);
    if (ret < 0) {
        fprintf(stderr, "Error claiming interface: %s\n", libusb_error_name(ret));
        libusb_close(l_devh);
        l_devh = NULL;
        libusb_exit(NULL);
        return NULL;
    }
    return l_devh;
}

int HID_Close(libusb_device_handle *l_devh)
{
    if (l_devh) {
        libusb_release_interface(l_devh, 0);
        libusb_close(l_devh);
        l_devh = NULL;
        libusb_exit(NULL);
        return 0;
    }
    return -1;
}

int HID_Ctrl_Comm(libusb_device_handle *l_devh,
    unsigned long  cbCommAPDU,	//apdu len
    unsigned char  *pbCommAPDU,	//apdu cmd
    unsigned long *cbRespAPDU,	//resp len
    unsigned char  *pbRespAPDU,	//resp buf
    unsigned long  dw_Timeout	//TimeOutValue
)
{
    unsigned char  OutPutReport[512];
    unsigned char  InPutReport[512],BlockNo;
    unsigned short BlockLen;
    unsigned char  SeqNumber,MoreData,PCB;
    unsigned char *pbBlock;
    int      wStataWord = 0xffff;
    int      iLoop;

    if (!l_devh) {
        return -1;
    }

    BlockNo = 0;
    pbBlock = pbCommAPDU;
    BlockLen = 0x40 - 4;
    SeqNumber = 0;
    iLoop = 0;
    do {
        memset(OutPutReport,0x00,512);
        OutPutReport[0]= (unsigned char)dw_Timeout;  //NAD_TimeOut;
        //len
        if(cbCommAPDU > BlockLen) {
            OutPutReport[2] = 0;
            OutPutReport[3] = (unsigned char)BlockLen;
            MoreData = 0x20;
        } else {
            OutPutReport[2] = 0;
            OutPutReport[3] =(unsigned char)cbCommAPDU;
            MoreData = 0;
        }
        //pcb
        OutPutReport[1] = SeqNumber + MoreData + BlockNo;
        BlockNo++;
        if(SeqNumber == 0x40) {
            SeqNumber = 0x0;
        } else {
            SeqNumber = 0x40;
        }
        memcpy(OutPutReport+4,pbBlock,OutPutReport[3]);
        cbCommAPDU -= OutPutReport[3];
        pbBlock += OutPutReport[3];
        if(libusb_control_transfer(l_devh, 0x21, 0x09, 0x3000, 0x0000, OutPutReport, 0x0040, 3000) < 0) {
            return -1;
        }
    } while (cbCommAPDU>0);

    pbBlock = pbRespAPDU;
    *cbRespAPDU =0;
    iLoop = 20;
    do {
RETRY:
        if(0 > libusb_control_transfer(l_devh, 0xa1, 0x01, 0x3000, 0x0000, InPutReport, 0x0040, 5000)) {
            return -1;
        }
        PCB = InPutReport[1];
        if(PCB == 0xCC) {
            goto RETRY;
        }
        MoreData = PCB & 0x20;
        if(InPutReport[3]>0) {
            memcpy(pbBlock,InPutReport+4,InPutReport[3]);
        }
        pbBlock += InPutReport[3];
        *cbRespAPDU += InPutReport[3];
    } while(MoreData > 0);
    if(*cbRespAPDU >= 2) {
        wStataWord = pbRespAPDU[*cbRespAPDU-2]*256 + pbRespAPDU[*cbRespAPDU-1];
        *cbRespAPDU -= 2;
    }
    else {
        return -1;
    }
    return (wStataWord);
}
