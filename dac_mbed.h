#include "lpc17xx_dac.h"
#include "lpc17xx_pinsel.h"

#define DAC_DEV LPC_DAC

void dac_init(void);

void dac_bias(uint32_t bias);

void dac_setval(uint32_t val);
