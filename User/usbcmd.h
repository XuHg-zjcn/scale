#ifndef USBCMD_H
#define USBCMD_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef enum{
	USBCMD_ReadOnce_Data = 0,
	USBCMD_ReadRepeat_Data = 1,
	USBCMD_Read_Stat = 2,
	USBCMD_Read_Params = 3,
	USBCMD_Write_Params = 4,
}USBCMD_Type;

//主要数据
typedef struct{
	bool raw_ad:1;        //ADC原始数据
	bool short_filter:1;  //短期滤波
	bool long_filter:1;   //长期滤波
	bool creep_corr:1;    //蠕变补偿
	bool zero_point:1;    //零点值
	bool display:1;       //显示值
}USBCMD_DataReq;


//设备状态
typedef struct{
	bool battary:1;
	bool datetime:1;
	bool MCU_temp:1;
	bool ADC_temp:1;
	bool OLED_bghtn:1;
	bool illume:1;
}USBCMD_StatReq;

void USBHD_Init();
void USBHD_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

#ifdef __cplusplus
};
#endif

#endif
