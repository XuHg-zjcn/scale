#include "app.h"
#include "delay.hpp"
#include "c_pin.hpp"
#include "s_i2c.hpp"
#include "c_usb.hpp"
#include "hx711.hpp"
#include "ssd1306.hpp"
#include "font_show.hpp"
#include "plot.hpp"
#include "ds18b20.hpp"
#include "filter.h"
#include "keys.h"

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


int32_t hx_rawv[256];  //HX711原始数据
volatile int hx_i = 0; //存放下一次数据的位置
SSD1306 *oled;
DS18B20 *ds1;
DS18B20 *ds2;

extern uint32_t keystat;

void HX_EXTI_Init()
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource9);
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

volatile int32_t x0;
int32_t mean;

void clear0()
{
	x0 = mean;
}

int app(void)
{
	usbd = new C_USBD(&R8_USB_CTRL);
	usbd->Init(&MyDeviceDescr,
	           &MyConfigDescr,
	           &MyStringDescrs);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	C_Pin scl = C_Pin(1, 10);
	C_Pin sda = C_Pin(1, 11);
	scl.loadXCfg(GPIO_GP_OD1);
	sda.loadXCfg(GPIO_GP_OD1);
	S_I2C si2c = S_I2C(scl, sda);
	si2c.set_clock(500000);
	S_I2C_Dev dev = S_I2C_Dev(&si2c, Addr_OLED);
	oled = new SSD1306(&dev);
	oled->Init();
	oled->fill(0x00);

	Keyboard_Init(72, 5000);
	kfdown[12] = &clear0;

	C_Pin dt1 = C_Pin(1, 9);  //top
	dt1.loadXCfg(GPIO_GP_OD1);
	ds1 = new DS18B20(dt1);
	C_Pin dt2 = C_Pin(1, 8);  //bottom
	dt2.loadXCfg(GPIO_GP_OD1);
	ds2 = new DS18B20(dt2);

	C_Pin sck = C_Pin((int)0, 10);
	C_Pin dout = C_Pin((int)0, 9);
	sck.loadXCfg(GPIO_GP_PP0);
	dout.loadXCfg(GPIO_In_Float);
	hx = new HX711(sck, dout);
	hx->Init(HX711_CHA_128);
	HX_EXTI_Init();
	while(hx_i < 48);
	x0 = hann_filter(5, hx_i-1);
	int a=32, b=48;
	char str[8];
	while(1){
		if(a < b){
			while(a <= hx_i && hx_i < b);
		}else{
			while(hx_i < b || hx_i >= a);
		}
		mean = hann_filter(5, b);
		int mg = ((int64_t)(mean-x0)*2491180449UL)>>32;
		plot_mg(*oled, dev, mg);
		show_mg10(*oled, dev, mg/10);

		int tmp1 = ds1->read_temp();
		int tmp2 = ds2->read_temp();
		snprintf(str, 6, "%04x", tmp1&0xffff);
		oled->setVHAddr(Vert_Mode, 98, 127, 6, 6);
		oled->text_5x7(str);
		snprintf(str, 6, "%04x", tmp2&0xffff);
		oled->setVHAddr(Vert_Mode, 98, 127, 7, 7);
		oled->text_5x7(str);
		ds1->convert_temp();
		ds2->convert_temp();

		a = (a+16)&0xff;
		b = (b+16)&0xff;
		*(int32_t *)&str[0] = mean;
		*(int16_t *)&str[4] = tmp1;
		*(int16_t *)&str[6] = tmp2;
		usbd->Send_Pack(0x81, str, 8);
	}
	return 0;
}

void USBHD_IRQHandler(void)
{
	int endp = usbd->USB_ISR();
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line9) != RESET){
		hx_rawv[hx_i] = hx->block_raw();
		hx_i = (hx_i+1)&0xff;
		//if(hx_i%8 == 0){
		//      usbd->Send_Pack(0x81, &hx_rawv[(hx_i-8)&0xff], 4*8);
		//}
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
}
