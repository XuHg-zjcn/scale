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
