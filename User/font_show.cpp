#include "font_show.hpp"
#include "font_12x24.h"

void show_mg10(SSD1306 &oled, S_I2C_Dev &dev, int32_t mg)
{
	uint32_t x = 6*14;
	bool neg;
	bool space = false;
	if(mg >= 1000000 || mg <= -100000){
		return;
	}
	if(mg < 0){
		mg = -mg;
		neg = true;
	}else{
		neg = false;
	}
	for(int i=0;i<6;i++){
		oled.setVHAddr(Vert_Mode, x, 127, 5, 7);
		x -= 14;
		int32_t dig = mg%10;
		mg = mg/10;
		if(i == 5 && neg){
			dig = 11;
		}else if(space){
			dig = 13;
		}
		dev.Mem_write(ConByte_Data, (uint8_t *)font12x24+dig*36, 36);
		if(i >= 2 && mg == 0){
			space = true;
		}
		if(i == 1){
			oled.setVHAddr(Vert_Mode, x, 127, 5, 7);
			x -= 14;
			dev.Mem_write(ConByte_Data, (uint8_t *)font12x24+10*36, 36);
		}
	}
}
