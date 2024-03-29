/*************************************************************************
 *  C++程序
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
#include "app.h"
#include "delay.hpp"
#include "c_pin.hpp"
#include "s_i2c.hpp"
#include "c_usb.hpp"
#include "adc24.hpp"
#include "ssd1306.hpp"
#include "font_show.hpp"
#include "plot.hpp"
#include "filter.h"
#include "keys.h"
#include "calc.h"
#include "ops.h"
#include "usbcmd.h"
#include "params.h"
#include "pins_config.h"
#include "power.h"
#include "creep.h"


extern C_USBD *usbd;
extern int32_t adc_creepcorr[256];
extern volatile int adc_i; //存放下一次数据的位置
SSD1306 *oled;

extern volatile uint32_t keystat;
extern int az_count;
extern int32_t filt_data[128];
extern int filt_i;
extern int32_t last;
extern int32_t creep_s1, creep_s2;
extern volatile int32_t x0_lastclr;
extern uint8_t USB_EP1_buffer[128];
extern USBCMD_DataReq usb_auto_report;


int app(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	Power_Init();
	USBHD_Init();
	Params_Init();
	
	C_Pin scl = C_Pin(OLED_SCL_Port, OLED_SCL_Pin);
	C_Pin sda = C_Pin(OLED_SDA_Port, OLED_SDA_Pin);
	scl.loadXCfg(GPIO_GP_OD1);
	sda.loadXCfg(GPIO_GP_OD1);
	S_I2C si2c = S_I2C(scl, sda);
	si2c.set_clock(500000);
	S_I2C_Dev dev = S_I2C_Dev(&si2c, Addr_OLED);
	oled = new SSD1306(&dev);
	oled->Init();
	oled->fill(0xf0);
	oled->commd_bytes(OutScan_Inv);
	oled->commd_bytes(Seg_Remap1);

	Keyboard_Init(72, 5000);
	kfdown[15] = &calc_clear;

	HX_Init();
	Wait_ADC24_b(32);
	calc_init(hann_filter(5, adc_i-1));
	int a = 16;
	char str[8];
	int filt_level = 3;  //滤波器等级
	int last_disp_i = 32;
	int last_mean = x0_lastclr;
	int last_f8 = x0_lastclr;
	int f32 = x0_lastclr;
	int mg32 = 0;
	uint8_t *pTx = &USB_EP1_buffer[64];
	int32_t usb_bytes = 0;
	oled->fill(0x00);
	while(1){
		for(int i=0;i<4;i++){
			int f8 = hann_filter(3, a);
			if(usb_auto_report.raw_ad){
				*pTx++ = 0x00; //raw_ad
				int x = (a-4)&0xff;
				for(int j=0;j<4;j++){
					uint32_t value = adc_creepcorr[(x+j)&0xff];
					*pTx++ = value&0xff;
					*pTx++ = (value>>8)&0xff;
					*pTx++ = (value>>16)&0xff;
				}
				usb_bytes += 1+3*4;
			}
			if(usb_auto_report.short_filter){
				*pTx++ = 0x01;
				*pTx++ = f8&0xff;
				*pTx++ = (f8>>8)&0xff;
				*pTx++ = (f8>>16)&0xff;
				usb_bytes += 1+3;
			}
			if(abs(f8 - last_f8) > 200){
				filt_level = 3;
				calc_load_x0();
			}
			last_f8 = f8;
			if(a >= last_disp_i + (1UL<<(filt_level-1))){
				int mean_d, mg_disp;
				if(filt_level == 5){
					last_disp_i = a&(~0xf);
				}else{
					last_disp_i = a;
				}
				if(filt_level == 5 and i == 0){
					mean_d = f32;
					mg_disp = mg32;
				}else{
					if(filt_level == 3){
						mean_d = f8;
					}else{
						mean_d = hann_filter(filt_level, a);
					}
					mg_disp = calc_mg_fast(mean_d);
				}
				last = mean_d;
				if(usb_auto_report.display){
					*pTx++ = 5;
					*pTx++ = mg_disp&0xff;
					*pTx++ = (mg_disp>>8)&0xff;
					*pTx++ = (mg_disp>>16)&0xff;
					usb_bytes += 1+3;
				}
				show_mg10(*oled, dev, DIV_ROUND(mg_disp, 10));
				snprintf(str, 6, "%4d", filt_level);
				oled->setVHAddr(Vert_Mode, 98, 127, 5, 5);
				oled->text_5x7(str);
				if(filt_level == 3){
					if(abs(mean_d-last_mean) < 200){
						filt_level = 4;
					}
				}else if(filt_level == 4){
					if(abs(mean_d-last_mean) < 100){
						filt_level = 5;
					}
				}
				last_mean = mean_d;
			}
			if(usb_bytes != 0 && i != 3){
				usbd->Send_Pack(0x81, usb_bytes);
				usb_bytes = 0;
				pTx = &USB_EP1_buffer[64];
			}
			a += 4;
			Wait_ADC24_b(a);
		}
		f32 = hann_filter(5, a);
		if(usb_auto_report.long_filter){
			*pTx++ = 0x02;
			*pTx++ = f32&0xff;
			*pTx++ = (f32>>8)&0xff;
			*pTx++ = (f32>>16)&0xff;
			usb_bytes += 1+3;
		}
		mg32 = calc_mg(f32);
		if(usb_auto_report.creep_corr){
			*pTx++ = 0x03;
			*pTx++ = creep_s2&0xff;
			*pTx++ = (creep_s2>>8)&0xff;
			*pTx++ = (creep_s2>>16)&0xff;
			usb_bytes += 1+3;
		}
		if(usb_bytes != 0){
			usbd->Send_Pack(0x81, usb_bytes);
			usb_bytes = 0;
			pTx = &USB_EP1_buffer[64];
		}

		update_plot(*oled, dev);
		snprintf(str, 6, "%4d", az_count);
		oled->setVHAddr(Vert_Mode, 98, 127, 6, 6);
		oled->text_5x7(str);

		snprintf(str, 6, "%5d", creep_delta(f32));
		oled->setVHAddr(Vert_Mode, 98, 127, 7, 7);
		oled->text_5x7(str);
	}
	return 0;
}
