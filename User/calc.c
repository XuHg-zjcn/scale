#include "calc.h"
#include <stdlib.h>
#include <ops.h>

volatile int32_t x0_display;
volatile int32_t x0_lastclr;
int32_t last;
const uint32_t mgLSB = 2491180449;

int az_count = 0;
int32_t x0_buffer[4];

int32_t filt_data[128];
int filt_i = -1;


void calc_clear()
{
	x0_lastclr = last;
	x0_display = last;
	az_count = 1;
	x0_buffer[0] = last;
}

void calc_set(int32_t x0)
{
	x0_lastclr = x0;
	x0_display = x0;
	az_count = 1;
	x0_buffer[0] = x0;
}

void calc_init(int32_t x0)
{
	for(int i=0;i<128;i++){
		filt_data[i] = x0;
	}
	calc_set(x0);
}

int compInc(const void *a, const void *b)
{
    return *(int32_t *)a - *(int32_t *)b;
}

int last_midd(int n)
{
	int32_t buff[n];
	for(int i=1;i<=n;i++){
		buff[i-1] = filt_data[(filt_i-i)&0x7f];
	}
	qsort(buff, n, sizeof(buff[0]), compInc);
	if(n%2 == 0){
		return (buff[n/2-1]+buff[n/2])/2;
	}else{
		return buff[n/2];
	}
}

int32_t last_sum(int n)
{
	int32_t s = 0;
	for(int i=1;i<=n;i++){
		s += filt_data[(filt_i-i)&0x7f];
	}
	return s;
}

int64_t last_nvar(int n, int32_t s)
{
	int64_t s2 = 0;
	for(int i=1;i<=n;i++){
		int32_t data = filt_data[(filt_i-i)&0x7f];
		s2 += (int64_t)data*data;
	}
	return s2 - (int64_t)s*s/n;
}


int isSoftClear(int32_t x, int32_t mg_lclr, int32_t mg_disp)
{
	if(az_count){
		if(abs(x - last_midd(min(az_count, 5))) < 40){
			return 1;
		}else{
			x0_display = x0_buffer[max(az_count-3, 0)&0x03];
			az_count = 0;
			return 0;
		}
	}else{
		if(abs(mg_lclr) > 500){
			return 0;
		}
		if(abs(mg_disp) > 50){
			return 0;
		}
		int sum = last_sum(5);
		if(abs(sum-x*5) > 40*5){
			return 0;
		}
		int var = last_nvar(5, sum);
		if(var > 500*5){
			return 0;
		}
		int midd = last_midd(5);
		if(abs(midd-x) > 40){
			return 0;
		}
		return 1;
	}
}

/*
 * 计算用于显示的毫克数
 */
int32_t calc_mg(int32_t x)
{
	last = x;
	filt_data[(filt_i+1)&0x7f] = x;
	filt_i = (filt_i+1)&0x7f;
	int32_t mg_lclr = (((int64_t)(x-x0_lastclr))*mgLSB)>>32;
	int32_t mg_disp = (((int64_t)(x-x0_display))*mgLSB)>>32;
	if(isSoftClear(x, mg_lclr, mg_disp)){
		az_count++;
		x0_buffer[(az_count-1)&0x03] = x;
		return 0;
	}else{
		return mg_disp;
	}
}