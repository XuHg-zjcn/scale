#include "usbcmd.h"
#include "c_usb.hpp"
#include "params.h"


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
uint8_t USB_EP1_buffer[128];

extern int32_t hx_rawv[256];  //HX711原始数据
extern volatile int hx_i; //存放下一次数据的位置

USBCMD_DataReq usb_auto_report = {0};

void USBHD_Init()
{
	usbd = new C_USBD(&R8_USB_CTRL);
	usbd->Init(&MyDeviceDescr,
		&MyConfigDescr,
		&MyStringDescrs);
	usbd->Set_Mode(0x01, true, true, false);
	usbd->Set_Buffer(0x01, USB_EP1_buffer);
}

int USBCMD_CollData(USBCMD_DataReq *req, uint8_t *pTx)
{
  int n = 0;
  if(req->raw_ad){
	u32 data = hx_rawv[(hx_i-1)&0xff];
	*pTx++ = data&0xff;
	*pTx++ = (data>>8)&0xff;
	*pTx++ = (data>>16)&0xff;
    n += 3;
  }
  return n;
}

int USBCMD_Proc(u8 *pRx)
{
  int n = 0;
  u8 *pTx = &USB_EP1_buffer[64];
  switch((USBCMD_Type)(*pRx++)){
  case USBCMD_ReadOnce_Data:
    n += USBCMD_CollData((USBCMD_DataReq *)(pRx++), pTx);
    break;
  case USBCMD_ReadRepeat_Data:
    *(u8*)&usb_auto_report = *pRx++;
    break;
  case USBCMD_Read_Stat:
    break;
  case USBCMD_Read_Params:
    break;
  case USBCMD_Write_Params:
    Params_Write((Params *)pRx);
    break;
  default:
    break;
  }
  return n;
}

void USBHD_IRQHandler(void)
{
  int endp = usbd->USB_ISR();
  if(endp == 0x01){
    u8 RxLen = usbd->regs->RX_LEN;
    if(RxLen >= 1){
      u8 *pRx = &USB_EP1_buffer[0];
      int n = USBCMD_Proc(pRx);
      if(n > 0){
        usbd->Send_Pack(0x81, n);
      }
    }
  }
}
