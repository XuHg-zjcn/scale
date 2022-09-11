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


const uint32_t win_hann256[256] = {
           641758,    2566651,    5773526,   10260468,   16024795,
         23063062,   31371061,   40943828,   51775640,   63860024,
         77189758,   91756873,  107552664,  124567690,  142791781,
        162214045,  182822874,  204605949,  227550252,  251642069,
        276867001,  303209971,  330655234,  359186388,  388786378,
        419437514,  451121477,  483819328,  517511526,  552177932,
        587797827,  624349922,  661812371,  700162781,  739378233,
        779435287,  820310003,  861977949,  904414222,  947593457,
        991489849, 1036077160, 1081328741, 1127217546, 1173716148,
       1220796755, 1268431229, 1316591099, 1365247580, 1414371591,
       1463933772, 1513904500, 1564253909, 1614951904, 1665968186,
       1717272261, 1768833468, 1820620987, 1872603867, 1924751039,
       1977031334, 2029413506, 2081866247, 2134358206, 2186858010,
       2239334281, 2291755654, 2344090798, 2396308432, 2448377349,
       2500266425, 2551944649, 2603381133, 2654545134, 2705406073,
       2755933550, 2806097366, 2855867539, 2905214322, 2954108221,
       3002520014, 3050420764, 3097781843, 3144574944, 3190772099,
       3236345697, 3281268500, 3325513656, 3369054723, 3411865676,
       3453920927, 3495195342, 3535664250, 3575303465, 3614089294,
       3651998556, 3689008593, 3725097284, 3760243061, 3794424917,
       3827622421, 3859815734, 3890985612, 3921113426, 3950181170,
       3978171469, 4005067595, 4030853473, 4055513690, 4079033507,
       4101398867, 4122596403, 4142613444, 4161438028, 4179058903,
       4195465538, 4210648126, 4224597592, 4237305600, 4248764555,
       4258967607, 4267908658, 4275582364, 4281984139, 4287110156,
       4290957353, 4293523428, 4294806850, 4294806850, 4293523428,
       4290957353, 4287110156, 4281984139, 4275582364, 4267908658,
       4258967607, 4248764555, 4237305600, 4224597592, 4210648126,
       4195465538, 4179058903, 4161438028, 4142613444, 4122596403,
       4101398867, 4079033507, 4055513690, 4030853473, 4005067595,
       3978171469, 3950181170, 3921113426, 3890985612, 3859815734,
       3827622421, 3794424917, 3760243061, 3725097284, 3689008593,
       3651998556, 3614089294, 3575303465, 3535664250, 3495195342,
       3453920927, 3411865676, 3369054723, 3325513656, 3281268500,
       3236345697, 3190772099, 3144574944, 3097781843, 3050420764,
       3002520014, 2954108221, 2905214322, 2855867539, 2806097366,
       2755933550, 2705406073, 2654545134, 2603381133, 2551944649,
       2500266425, 2448377349, 2396308432, 2344090798, 2291755654,
       2239334281, 2186858010, 2134358206, 2081866247, 2029413506,
       1977031334, 1924751039, 1872603867, 1820620987, 1768833468,
       1717272261, 1665968186, 1614951904, 1564253909, 1513904500,
       1463933772, 1414371591, 1365247580, 1316591099, 1268431229,
       1220796755, 1173716148, 1127217546, 1081328741, 1036077160,
        991489849,  947593457,  904414222,  861977949,  820310003,
        779435287,  739378233,  700162781,  661812371,  624349922,
        587797827,  552177932,  517511526,  483819328,  451121477,
        419437514,  388786378,  359186388,  330655234,  303209971,
        276867001,  251642069,  227550252,  204605949,  182822874,
        162214045,  142791781,  124567690,  107552664,   91756873,
         77189758,   63860024,   51775640,   40943828,   31371061,
         23063062,   16024795,   10260468,    5773526,    2566651,
           641758
};

int32_t hx_rawv[256];  //HX711原始数据
volatile int hx_i = 0; //存放下一次数据的位置
HX711 *hx;
DS18B20 *ds;

int sum256(int32_t *p, int32_t offset)
{
	int sum_x = 0;
	for(int j=0;j<256;j++){
		sum_x += ((int64_t)p[(j+offset)&0xff]*win_hann256[j])>>32;
	}
	return sum_x;
}

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
	SSD1306 oled = SSD1306(&dev);
	oled.Init();
	oled.fill(0x00);

	C_Pin dt = C_Pin((int)0, 8);
	dt.loadXCfg(GPIO_GP_OD1);
	ds = new DS18B20(dt);

	C_Pin sck = C_Pin((int)0, 10);
	C_Pin dout = C_Pin((int)0, 9);
	sck.loadXCfg(GPIO_GP_PP0);
	dout.loadXCfg(GPIO_In_Float);
	hx = new HX711(sck, dout);
	hx->Init(HX711_CHA_128);
	HX_EXTI_Init();
	while(hx_i < 128);
	while(hx_i >= 128);
	int32_t x0 = sum256(hx_rawv, hx_i);
	bool stat = false;
	while(1){
		if(stat){
			//在记录前半段时阻塞，已采样到127（下一个为128）时跳出
			while(hx_i < 128);
		}else{
			//在记录后半段时阻塞，已采样到255（下一个为0）时跳出
			while(hx_i >= 128);
		}
		int sum_x = sum256(hx_rawv, hx_i) - x0;
		int mg = ((int64_t)sum_x*26575397)>>32;
		plot_mg(oled, dev, mg);
		show_mg10(oled, dev, mg/10);
		stat = not stat;  //下一次阻塞另一段
		if(stat){
			int16_t temp = ds->read_temp();
			char str[6];
			snprintf(str, 6, "%5d", temp);
			oled.setVHAddr(Vert_Mode, 98, 127, 7, 7);
			oled.text_5x7(str);
			ds->convert_temp();
			*(int32_t *)&str[0] = sum_x;
			*(int16_t *)&str[4] = temp;
			usbd->Send_Pack(0x81, str, 6);
		}
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
		if(hx_i%8 == 0){
			//usbd->Send_Pack(0x81, &hx_rawv[(hx_i-8)&0xff], 4*8);
		}
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
}
