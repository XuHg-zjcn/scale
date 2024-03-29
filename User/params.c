#include "params.h"
#include "ch32v10x_flash.h"

const Params *params;
const Params p_default = {
	.mgLSB = 2544360965,
	.creep_A1 = 257698,
	.creep_c1 = 373229581,
	.creep_A2 = 442381,
	.creep_c2 = 17145555,
};

void Params_Init()
{
	params = (Params *)0x0800fc00;
	if(params->mgLSB == 0xffffffff){
		//还没有写入过
		Params_Write(&p_default);
	}
	while(params->mgLSB == 0 && (uint32_t)params < 0x0800fff0){
		//已被覆盖
		params++;
	}
	if((uint32_t)params >= 0x0800fff0){
		//到Page尾部，所有都被填充0
		Params_Write(&p_default);
	}
}

void Params_Write(const Params* p)
{
	FLASH_Unlock();
	params = (Params *)0x0800fc00;
	//寻找写入位置
	while(!(params->mgLSB) && (uint32_t)params < 0x0800fff0){
		params++;
		if(params->mgLSB != 0){
			//覆盖为0
			FLASH_ProgramWord((uint32_t)&params->mgLSB, 0);
		}
	}
	if(params >= 0x0800fff0){
		//到Page尾部了
		params = (Params *)0x0800fc00;
		FLASH_ErasePage((uint32_t)params);
	}
	FLASH_ProgramWord((uint32_t)&params->mgLSB, p->mgLSB);
	FLASH_ProgramWord((uint32_t)&params->creep_A1, p->creep_A1);
	FLASH_ProgramWord((uint32_t)&params->creep_c1, p->creep_c1);
	FLASH_ProgramWord((uint32_t)&params->creep_A2, p->creep_A2);
	FLASH_ProgramWord((uint32_t)&params->creep_c2, p->creep_c2);
	FLASH_Lock();
}
