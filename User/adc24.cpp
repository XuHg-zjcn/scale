#include "adc24.hpp"
#include "c_pin.hpp"
#include "hx711.hpp"

int32_t hx_rawv[256];  //HX711原始数据
volatile int hx_i = 0; //存放下一次数据的位置

HX711 *hx;

void HX_Init()
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	C_Pin sck = C_Pin((int)0, 10);
	C_Pin dout = C_Pin((int)0, 9);
	sck.loadXCfg(GPIO_GP_PP0);
	dout.loadXCfg(GPIO_In_Float);
	hx = new HX711(sck, dout);
	hx->Init(HX711_CHA_128);

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

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line9) != RESET){
		hx_rawv[hx_i] = hx->block_raw();
		hx_i = (hx_i+1)&0xff;
		//if(hx_i%8 == 0){
		//      usbd->Send_Pack(0x81, &hx_rawv[(hx_i-8)&0xff], 4*8);
		//}
		EXTI_ClearITPendingBit(EXTI_Line9);
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
	}
}

void Wait_ADC24(int a, int b)
{
	if(a < b){
		while(a <= hx_i && hx_i < b);
	}else{
		while(hx_i < b || hx_i >= a);
	}
}

int Wait_ADC24_b(int b)
{
	int x = hx_i;
	Wait_ADC24(x, b);
	return x;
}

int Wait_ADC24_n(int n)
{
	int x = hx_i;
	Wait_ADC24(x, (x+n)&0xff);
	return x;
}
