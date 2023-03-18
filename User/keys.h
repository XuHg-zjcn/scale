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
#ifndef KEYS_H
#define KEYS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif


#define KEY_1     (1<<0)
#define KEY_2     (1<<1)
#define KEY_3     (1<<2)
#define KEY_SET   (1<<3)

#define KEY_4     (1<<4)
#define KEY_5     (1<<5)
#define KEY_6     (1<<6)
#define KEY_MOD   (1<<7)

#define KEY_7     (1<<8)
#define KEY_8     (1<<9)
#define KEY_9     (1<<10)
#define KEY_OK    (1<<11)

#define KEY_PM    (1<<12)
#define KEY_0     (1<<13)
#define KEY_PT    (1<<14)
#define KEY_CLR   (1<<15)


typedef void (*keyfunc)(void);
extern keyfunc kfdown[16];
extern keyfunc kfup[16];

void Keyboard_Init(uint16_t psc, uint16_t arr);
void WaitKey_Down(uint16_t key);
void WaitKey_Up(uint16_t key);
void WaitKey_Change(uint16_t key);

#ifdef __cplusplus
}
#endif

#endif
