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
	bool temp_corr:1;     //温度补偿
	bool buoyancy_corr:1; //空气浮力补偿
	bool all_corr:1;      //总补偿
	bool corr_res:1;      //补偿结果
	bool zero_point:1;    //零点值
	bool tare_point:1;    //去皮值
	bool display:1;       //显示值
}USBCMD_Data1Req;

//辅助数据
typedef struct{
	bool air_temp1:1;     //湿度传感器的温度
	bool air_hum:1;       //湿度
	bool air_temp2:1;     //气压传感器的温度
	bool air_press:1;     //气压
	bool air_temp_raw:1;  //气压传感器的温度原始数据
	bool air_press_raw:1; //气压原始数据
	bool air_dentsity:1;  //空气密度(计算)
	bool volume:1;        //物品体积
}USBCMD_Data2Req;


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
