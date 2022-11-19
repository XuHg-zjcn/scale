#ifndef PINS_CONFIG_H
#define PINS_CONFIG_H

//ADC芯片
#define ADC24_SCLK_Port  1
#define ADC24_SCLK_Pin   8
#define ADC24_DRDO_Port  1
#define ADC24_DRDO_Pin   9
#define ADC24_DRDO_EXTI_PortSource  GPIO_PortSourceGPIOB
#define ADC24_DRDO_EXTI_PinSource   GPIO_PinSource9

//OLED I2C
#define OLED_SCL_Port (int)0
#define OLED_SCL_Pin  5
#define OLED_SDA_Port (int)0
#define OLED_SDA_Pin  7

//电源键
#define PWRK_Port     (int)0
#define PWRK_Pin      0
#define PWRK_EXTI_PortSource  GPIO_PortSourceGPIOA
#define PWRK_EXTI_PinSource   GPIO_PinSource0


//电源控制引脚
#define DPWR_Port     (int)0
#define DPWR_Pin      14
#define APWR_Port     1
#define APWR_Pin      7

#endif
