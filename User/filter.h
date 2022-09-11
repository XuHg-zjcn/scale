#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

int32_t hann_filter(uint32_t win2N, int curr);

#ifdef __cplusplus
}
#endif

#endif
