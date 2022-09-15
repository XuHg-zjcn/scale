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
