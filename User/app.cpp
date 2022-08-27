#include "app.h"
#include "delay.hpp"
#include "c_pin.hpp"
#include "s_i2c.hpp"
#include "hx711.hpp"
#include "ssd1306.hpp"

char str[20];

int app(void)
{
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
		snprintf(str, 20, "%8d", hx.block_raw());
		oled.setVHAddr(Vert_Mode, 0, 127, 0, 0);
		oled.text_5x7(str);
		//XDelayMs(100);
	}
	return 0;
}
