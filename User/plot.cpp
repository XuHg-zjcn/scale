#include "plot.hpp"
#include "ops.h"
#include <limits.h>

int32_t plot_data[128];
int32_t plot_max=1, plot_min=0;
int8_t plot_tmp[128];
int plot_i = -1;
uint32_t plot_a;

void plot_mg(SSD1306 &oled, S_I2C_Dev &dev, int32_t mg)
{
	plot_i++;
	if(plot_i >= 128){
		plot_i = 0;
	}
	bool redraw = false;
	int32_t old = plot_data[plot_i];
	plot_data[plot_i] = mg;
	if(old == plot_max){
		plot_max = INT_MIN;
		for(int i=0;i<128;i++){
			if(plot_data[i] > plot_max){
				plot_max = plot_data[i];
			}
		}
		redraw = true;
	}
	if(old == plot_min){
		plot_min = INT_MAX;
		for(int i=0;i<128;i++){
			if(plot_data[i] < plot_min){
				plot_min = plot_data[i];
			}
		}
		redraw = true;
	}
	if(mg > plot_max){
		plot_max = mg;
		redraw = true;
	}
	if(mg < plot_min){
		plot_min = mg;
		redraw = true;
	}
	if(redraw){
		if(plot_max == plot_min){
			return;
		}
		plot_a = (40U<<26)/(plot_max-plot_min);
		for(int i=0;i<128;i++){
			plot_tmp[i] = value_clip(((uint32_t)(plot_data[i]-plot_min)*plot_a)>>26, 0, 39);
		}
	}else{
		plot_tmp[plot_i] = value_clip(((uint32_t)(plot_data[plot_i]-plot_min)*plot_a)>>26, 0, 39);
	}
	oled.setVHAddr(Vert_Mode, 0, 127, 0, 4);
	for(int i=1;i<=128;i++){
		uint64_t col = 1ULL<<(39-plot_tmp[(plot_i+i)&0x7f]);
		dev.Mem_write(ConByte_Data, (uint8_t *)&col, 5);
	}
}
