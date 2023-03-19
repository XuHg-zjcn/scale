#ifndef CREEP_H
#define CREEP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

void creep_init(int32_t x0);
void creep_update(int32_t x);
int32_t creep_delta(int32_t x);

#ifdef __cplusplus
};
#endif

#endif
