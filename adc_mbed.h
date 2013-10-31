#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"

#define ADC_DEV LPC_ADC
#define ADC_RATE 100000

void adc_init(void);

void adc_enchan(uint8_t chan, uint8_t en);

uint16_t adc_getdata(uint8_t chan);
