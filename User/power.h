#ifndef POWER_H
#define POWER_H

#ifdef __cplusplus
extern "C"{
#endif

void Power_Init();
void Update_AutoPowerOff();
void EXTI0_IRQHandler() __attribute__((interrupt("WCH-Interrupt-fast")));
void SysTick_Handler() __attribute__((interrupt("WCH-Interrupt-fast")));

#ifdef __cplusplus
}
#endif

#endif
