#include "usbcmd.h"
#include "c_usb.hpp"


/* Device Descriptor */
const USB_DeviceDescr MyDeviceDescr = {
  0x12, 0x01,              // bLength, bDescriptorType
  0x0110,                  // bcdUSB v1.1
  0xFF, 0x80, 0x55,        // Class, SubClass, Protocol
  0x40,                    // bMaxPacketSize0
  0xFFFF, 0xA05C, 0x0100,  // VID, PID, bcdDevice
  0x01, 0x02, 0x00,        // iManu, iProd, iSerNum
  0x01,                    // bNumConfigurations
};

/* Configration Descriptor */
const struct MyCfgDescr{
  USB_ConfigDescr c = {0x09, 0x02, sizeof(MyCfgDescr), 0x01, 0x01, 0x00, 0x80, 0x32};
  USB_IntfDescr  i  = {0x09, 0x04, 0x00, 0x00, 0x02, 0xFF, 0x80, 0x55, 0x00};
  USB_EndpDescr e81 = {0x07, 0x05, 0x81, 0x02, 0x0040, 0x00};
  USB_EndpDescr e01 = {0x07, 0x05, 0x01, 0x02, 0x0040, 0x00};
  u8 tail = 0;
}MyConfigDescr;

const u8 dbufep[] = {0x02, 0x00};

/* String Descriptors */
const struct StringDescrs{
  struct MyLangDescr{
    u8  bLength = sizeof(MyLangDescr);
    u8  bDescriptorType = 0x03;
    u16 wLANGIDs[1] = {0x0409};
  }MyLangDescr;
  USB_UTF16_DESC(u"Xu Ruijun DIY") MyManuInfo;
  USB_UTF16_DESC(u"My Scale") MyProdInfo;
  u8 tail = 0;
}MyStringDescrs;

C_USBD *usbd;
uint8_t USB_buffer[128];


void USBHD_Init()
{
	usbd = new C_USBD(&R8_USB_CTRL);
	usbd->Init(&MyDeviceDescr,
		&MyConfigDescr,
		&MyStringDescrs);
}

void USBHD_IRQHandler(void)
{
        int endp = usbd->USB_ISR();
	if(endp == 0x81){
		u8 RxLen = usbd->regs->RX_LEN;
	}
}
