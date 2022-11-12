#ifndef PARAMS_H
#define PARAMS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct{
        uint32_t mgLSB;
        uint32_t creep_change;
        uint32_t creep_coeff;
}Params;

void Params_Init();
void Params_Write(const Params *p);
void Params_Read(Params *p);

#ifdef __cplusplus
}
#endif

#endif
