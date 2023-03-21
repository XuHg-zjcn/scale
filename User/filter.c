/*************************************************************************
 *  ADC数据滤波
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
#include "filter.h"
#include "limits.h"

extern const uint32_t win_hann8[4];
extern const uint32_t win_hann16[8];
extern const uint32_t win_hann32[16];
extern const uint32_t win_hann64[32];
extern const uint32_t win_hann128[64];
extern const uint32_t win_hann256[128];

const uint32_t const *pWins[6] = {
	win_hann8, win_hann16, win_hann32,
	win_hann64, win_hann128, win_hann256
};

extern int32_t adc_creepcorr[256];

int32_t hann_filter(uint32_t win2N, int curr){
	if(win2N < 3 || win2N > 8){
		return INT_MIN;
	}
	int32_t sum_x = 0;
	uint32_t hwin = 1UL<<(win2N-1); //半窗大小
	const uint32_t *pwin = pWins[win2N-3]; 
	curr -= 1UL<<win2N;
	//先计算前半窗口，减少被后续数据覆盖可能
	for(int j=0;j<hwin;j++){
		sum_x += ((int64_t)adc_creepcorr[(curr+j)&0xff]*pwin[j])>>32;
	}
	curr += (1UL<<win2N) - 1;
        for(int j=hwin-1;j>=0;j--){
		sum_x += ((int64_t)adc_creepcorr[(curr-j)&0xff]*pwin[j])>>32;
	}
	return sum_x>>(win2N-1);
}
