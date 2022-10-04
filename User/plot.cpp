#include "plot.hpp"
#include "ops.h"
#include <limits.h>

extern int32_t filt_data[128];
extern int filt_i;

int plot_i = -1;  //最后已画或准备要画的index
int32_t plot_max=INT_MIN, plot_min=INT_MAX;
int32_t plot_max_i=-1, plot_min_i=-1;
int8_t plot_tmp[128];
uint32_t plot_a;

void update_plot(SSD1306 &oled, S_I2C_Dev &dev)
{
	bool redraw = false;
	bool calcmin = false;
	bool calcmax = false;
	while(filt_i != plot_i){
		int32_t old = filt_data[plot_i];
		plot_i = (plot_i+1)&0x7f;
		int32_t mg = filt_data[plot_i];
		if(plot_i == plot_max_i){  //移出最大值
			calcmax = true;
			redraw = true;
		}
		if(plot_i == plot_min_i){  //移出最小值
			calcmin = true;
			redraw = true;
		}
		if(mg > plot_max){  //超过原最大值
			plot_max = mg;
			plot_max_i = plot_i;
			redraw = true;
		}
		if(mg < plot_min){  //超过原最小值
			plot_min = mg;
			plot_min_i = plot_i;
			redraw = true;
		}
		if(!redraw){
			plot_tmp[plot_i] = value_clip(((uint32_t)(filt_data[plot_i]-plot_min)*plot_a)>>26, 0, 39);
		}
	}
	if(calcmax){
		plot_max = INT_MIN;
		for(int i=0;i<128;i++){
			if(filt_data[i] > plot_max){
				plot_max = filt_data[i];
				plot_max_i = i;
			}
		}
	}
	if(calcmin){
		plot_min = INT_MAX;
		for(int i=0;i<128;i++){
			if(filt_data[i] < plot_min){
				plot_min = filt_data[i];
				plot_min_i = i;
			}
		}
	}
	if(redraw){  //需要重新计算整个视图
		if(plot_max == plot_min){
			return;
		}
		plot_a = (40U<<26)/(plot_max-plot_min);
		for(int i=0;i<128;i++){
			plot_tmp[i] = value_clip((((uint64_t)(filt_data[i]-plot_min))*plot_a)>>26, 0, 39);
		}
	}
	oled.setVHAddr(Vert_Mode, 0, 127, 0, 4);
	for(int i=1;i<=128;i++){
		uint64_t col = 1ULL<<(39-plot_tmp[(plot_i+i)&0x7f]);
		dev.Mem_write(ConByte_Data, (uint8_t *)&col, 5);
	}
}
