/*************************************************************************
 *  键盘驱动
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
#include "keys.h"
#include "ch32v10x.h"
#include "ops.h"
#include "power.h"

/*
 * |  键盘样式  | keystat |
 * | 1 2 3 设置 | 3 2 1 0 | --row1
 * | 4 5 6 模式 | 7 6 5 4 | --row2
 * | 7 8 9 OK   | B A 9 8 | --row3
 * | ± 0 . 清零 | F E D C | --row4
 *                | | | |
 *            col 1 2 3 4
 */

#define GPIO_Port_Row     GPIOB
#define GPIO_Pin_Row1     GPIO_Pin_14
#define GPIO_Pin_Row2     GPIO_Pin_13
#define GPIO_Pin_Row3     GPIO_Pin_12
#define GPIO_Pin_Row4     GPIO_Pin_11
#define GPIO_Pin_Row_All  (GPIO_Pin_Row1 | GPIO_Pin_Row2 | GPIO_Pin_Row3 | GPIO_Pin_Row4)

#define GPIO_Port_Col1    GPIOB
#define GPIO_Pin_Col1     GPIO_Pin_15

#define GPIO_Port_Col234  GPIOA
#define GPIO_Pin_Col2     GPIO_Pin_8
#define GPIO_Pin_Col3     GPIO_Pin_9
#define GPIO_Pin_Col4     GPIO_Pin_10
#define GPIO_Pin_Col234   (GPIO_Pin_Col2 | GPIO_Pin_Col3 | GPIO_Pin_Col4)


//高电平(弹起)为0，低电平(按下)为1
uint32_t Read_Cols()
{
        return ((((~GPIO_ReadInputData(GPIOA))>>7)&0x0e) | (((~GPIO_ReadInputData(GPIOB))>>15)&0x01));
}



#define TIM_Keyboard      TIM3
#define Key_Nchrg         3

void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

volatile uint32_t keystat = 0;  //当前状态
static uint32_t keychrg = 0;  //变化检测中
static uint8_t keycount[16];  //消抖计数
keyfunc kfdown[16];
keyfunc kfup[16];

void Keyboard_Init(uint16_t psc, uint16_t arr)
{
    for(int i=0;i<16;i++){
        kfdown[i] = NULL;
        kfup[i] = NULL;
    }
    GPIO_InitTypeDef        GPIO_InitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};
    TIM_OCInitTypeDef       TIM_OCInitStructure = {0};
    NVIC_InitTypeDef        NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Row_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIO_Port_Row, &GPIO_InitStructure);
    GPIO_ResetBits(GPIO_Port_Row, GPIO_Pin_Row_All);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Col1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIO_Port_Col1, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Col234;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIO_Port_Col234, &GPIO_InitStructure);

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM_Keyboard, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = arr/2;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM1->INTFR = 0;
    TIM_ITConfig(TIM_Keyboard, TIM_IT_Update, ENABLE);
    TIM_ARRPreloadConfig(TIM_Keyboard, ENABLE);
    TIM_Cmd(TIM_Keyboard, ENABLE);
}

void TIM3_IRQHandler(void)
{
    if(TIM_GetFlagStatus(TIM_Keyboard, TIM_FLAG_Update)){
        TIM_ClearFlag(TIM_Keyboard, TIM_FLAG_Update);
        if(keystat || keychrg || Read_Cols()){
            for(int i=0;i<4;i++){
                GPIO_SetBits(GPIO_Port_Row, GPIO_Pin_Row_All);
                GPIO_ResetBits(GPIO_Port_Row, GPIO_Pin_14>>i);
                Delay_Us(1);
                uint32_t old = (keystat>>(i*4))&0x0f;
                uint32_t curr = Read_Cols();
                uint32_t kxor = curr^old;
                if(!kxor && !keychrg){
                    continue;  //不需要检查每个按钮
                }
                for(int j=0;j<4;j++){
                    int key_i = i*4+j;
                    if(kxor & 1){  //有变化
                        if(++keycount[key_i] >= Key_Nchrg){  //消抖延时够了
                            keycount[key_i] = 0;
                            if(curr&1){  //现在是按下
                                keystat |= 1<<key_i;
                                if(kfdown[key_i]){
                                    kfdown[key_i]();
                                }
                            }else{       //现在是弹起
                                keystat &= ~(1<<key_i);
                                if(kfup[key_i]){
                                    kfup[key_i]();
                                }
                                Update_AutoPowerOff();
                            }
                            keychrg &= ~(1<<key_i);
                        }else{
                            keychrg |= 1<<key_i;
                        }
                    }else{  //没有变化
                        keycount[key_i] = 0;
                        keychrg &= ~(1<<key_i);
                    }
                    kxor >>= 1;
                    curr >>= 1;
                }
            }
        }
        if(!keystat){
            GPIO_ResetBits(GPIO_Port_Row, GPIO_Pin_Row_All);
        }
    }
}

/*
 * 等待到key中的任何一个键按下退出
 */
void WaitKey_Down(uint16_t key)
{
    while(!(keystat&key));
}

/*
 * 等待到key中的所有键弹起后退出
 */
void WaitKey_Up(uint16_t key)
{
    while(keystat&key);
}

/*
 * 等待到key中的任何键出现变化
 */
void WaitKey_Change(uint16_t key)
{
    uint16_t old = keystat;
    while(old == keystat);
}
