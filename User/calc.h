#ifndef CALC_H
#define CALC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

int32_t calc_mg(int32_t x);
void calc_set(int32_t x0);
void calc_init(int32_t x0);
void calc_clear();

#ifdef __cplusplus
}
#endif

#endif
