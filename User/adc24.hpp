#ifndef ADC24_HPP
#define ADC24_HPP


void HX_Init();
void Wait_ADC24(int a, int b);
int Wait_ADC24_b(int b);
int Wait_ADC24_n(int n);

#ifdef __cplusplus
extern "C"{
#endif
void EXTI9_5_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
#ifdef __cplusplus
}
#endif

#endif
