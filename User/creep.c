#include "creep.h"
#include "params.h"
#include <stdint.h>

extern const Params *params;
int32_t creep_s1, creep_s2;

void creep_init(int32_t x0)
{
	creep_s1 = x0;
	creep_s2 = x0;
}

void creep_update(int32_t x)
{
	creep_s1 = (((int64_t)creep_s1*((-params->creep_c1)&0xffffffff))>>32) + \
	           (((int64_t)x*params->creep_c1)>>32);
	creep_s2 = (((int64_t)creep_s2*((-params->creep_c2)&0xffffffff))>>32) + \
	           (((int64_t)x*params->creep_c2)>>32);
}

int32_t creep_delta(int32_t x)
{
	return (((int64_t)(x-creep_s1)*params->creep_A1)>>32) + (((int64_t)(x-creep_s2)*params->creep_A2)>>32);
}
