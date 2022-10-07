/*************************************************************************
 *  示数计算
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
#ifndef CALC_H
#define CALC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

int32_t calc_mg(int32_t x);
int32_t calc_mg_fast(int32_t x);
void calc_set(int32_t x0);
void calc_init(int32_t x0);
void calc_load_x0();
void calc_clear();

#ifdef __cplusplus
}
#endif

#endif
