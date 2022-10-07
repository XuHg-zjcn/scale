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

typedef void (*keyfunc)(void);
extern keyfunc kfdown[16];
extern keyfunc kfup[16];

void Keyboard_Init(uint16_t psc, uint16_t arr);

#ifdef __cplusplus
}
#endif

#endif
