#ifndef POWER_H
#define POWER_H

#ifdef __cplusplus
extern "C"{
#endif

void Power_Init();
void EXTI0_IRQHandler() __attribute__((interrupt("WCH-Interrupt-fast")));

#ifdef __cplusplus
}
#endif

#endif
