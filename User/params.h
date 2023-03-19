#ifndef PARAMS_H
#define PARAMS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

// Y_pred[t] = Y_adc[t] + delta_1[t] + delta_2[t]
// delta_i[t] = A_i*(Y_adc[t] - stat_i[t])
// stat_i[t] = (1-c_i)*stat_i[t-1] + c_i*Y_adc[t]
typedef struct{
        uint32_t mgLSB;
        uint32_t creep_A1;
        uint32_t creep_c1;
        uint32_t creep_A2;
        uint32_t creep_c2;
}Params;

void Params_Init();
void Params_Write(const Params *p);
void Params_Read(Params *p);

#ifdef __cplusplus
}
#endif

#endif
