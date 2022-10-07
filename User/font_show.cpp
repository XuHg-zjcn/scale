/*************************************************************************
 *  12x24字体显示
 *  Copyright (C) 2022  Xu Ruijun
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *************************************************************************/
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
