#include "dac_mbed.h"

void dac_init(void) {
	PINSEL_CFG_Type pcfg;
	pcfg.Pinmode = 0;
	pcfg.Funcnum = 2;
	pcfg.Pinnum = 26;
	pcfg.Portnum = 0;
	PINSEL_ConfigPin(&pcfg);
	DAC_Init(DAC_DEV);
}

void dac_bias(uint32_t bias) {
	DAC_SetBias(DAC_DEV, bias);
}

void dac_setval(uint32_t val) {
	DAC_UpdateValue(DAC_DEV, val);
}


