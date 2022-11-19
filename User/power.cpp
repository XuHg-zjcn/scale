#include "power.h"
#include "c_pin.hpp"
#include "ch32v10x.h"
#include "pins_config.h"

C_Pin *pwrk;
C_Pin *dpwr;
C_Pin *apwr;
int PA0_count;


void Power_Init()
{
        EXTI_InitTypeDef EXTI_InitStructure;
        NVIC_InitTypeDef NVIC_InitStructure;

	//需要先打开AFIO时钟
        GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
        dpwr = new C_Pin(DPWR_Port, DPWR_Pin);
        apwr = new C_Pin(APWR_Port, APWR_Pin);
        dpwr->loadXCfg(GPIO_GP_PP1);
        apwr->loadXCfg(GPIO_GP_PP1);
        dpwr->write_pin(Pin_Set);
        apwr->write_pin(Pin_Set);

	pwrk = new C_Pin(PWRK_Port, PWRK_Pin);
	pwrk->loadXCfg(GPIO_In_Down);

        GPIO_EXTILineConfig(PWRK_EXTI_PortSource, PWRK_EXTI_PinSource);
        EXTI_InitStructure.EXTI_Line = EXTI_Line0;
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure);

        NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
}

void Power_Off()
{
	apwr->write_pin(Pin_Reset);
	dpwr->write_pin(Pin_Reset);
}

void EXTI0_IRQHandler()
{
	EXTI_ClearITPendingBit(EXTI_Line0);
	if(PA0_count){
		Power_Off();
	}
	PA0_count++;
}
