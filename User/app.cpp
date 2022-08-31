#include "app.h"
#include "delay.hpp"
#include "c_pin.hpp"
#include "s_i2c.hpp"
#include "c_usb.hpp"
#include "hx711.hpp"
#include "ssd1306.hpp"

char str[20];
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
  USB_IntfDescr  i  = {0x09, 0x04, 0x00, 0x00, 0x01, 0xFF, 0x80, 0x55, 0x00};
  USB_EndpDescr e81 = {0x07, 0x05, 0x81, 0x02, 0x0020, 0x00};
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


int app(void)
{
	int32_t hx_rawv[8];
	usbd = new C_USBD(&R8_USB_CTRL);
	usbd->Init(&MyDeviceDescr,
	           &MyConfigDescr,
	           &MyStringDescrs);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	C_Pin scl = C_Pin(1, 10);
	C_Pin sda = C_Pin(1, 11);
	scl.loadXCfg(GPIO_GP_OD1);
	sda.loadXCfg(GPIO_GP_OD1);
	S_I2C si2c = S_I2C(scl, sda);
	si2c.set_clock(100000);
	S_I2C_Dev dev = S_I2C_Dev(&si2c, Addr_OLED);
	SSD1306 oled = SSD1306(&dev);
	oled.Init();
	oled.fill(0x00);

	C_Pin sck = C_Pin((int)0, 10);
	C_Pin dout = C_Pin((int)0, 9);
	sck.loadXCfg(GPIO_GP_PP0);
	dout.loadXCfg(GPIO_In_Float);
	HX711 hx = HX711(sck, dout);
	hx.Init(HX711_CHA_128);
	while(1){
		for(int i=0;i<8;i++){
			hx_rawv[i] = hx.block_raw();
			//snprintf(str, 20, "%8d", hx_rawv);
			//oled.setVHAddr(Vert_Mode, 0, 127, 0, 0);
			//oled.text_5x7(str);
		}
		usbd->Send_Pack(0x81, &hx_rawv, 4*8);
	}
	return 0;
}

void USBHD_IRQHandler(void)
{
	int endp = usbd->USB_ISR();
}
