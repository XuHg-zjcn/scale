/*************************************************************************
 *  24位ADC抽象层
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
#include "adc24.hpp"
#include "c_pin.hpp"
#include "hx711.hpp"
#include "pins_config.h"
#include "creep.h"

int32_t hx_rawv[256];  //HX711原始数据
volatile int hx_i = 0; //已读取数据个数

HX711 *hx;

void HX_Init()
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	C_Pin sck = C_Pin(ADC24_SCLK_Port, ADC24_SCLK_Pin);
	C_Pin dout = C_Pin(ADC24_DRDO_Port, ADC24_DRDO_Pin);
	sck.loadXCfg(GPIO_GP_PP0);
	dout.loadXCfg(GPIO_In_Float);
	hx = new HX711(sck, dout);
	hx->Init(HX711_CHA_128);

	GPIO_EXTILineConfig(ADC24_DRDO_EXTI_PortSource, ADC24_DRDO_EXTI_PinSource);
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

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line9) != RESET){
		hx_rawv[hx_i&0xff] = hx->block_raw();
		creep_update(hx_rawv[hx_i&0xff]);
		hx_i++;
		//if(hx_i%8 == 0){
		//      usbd->Send_Pack(0x81, &hx_rawv[(hx_i-8)&0xff], 4*8);
		//}
		EXTI_ClearITPendingBit(EXTI_Line9);
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
	}
}

void Wait_ADC24_b(int b)
{
	while(hx_i < b);
}

int Wait_ADC24_n(int n)
{
	int x = hx_i;
	Wait_ADC24_b(x+n);
	return x;
}
