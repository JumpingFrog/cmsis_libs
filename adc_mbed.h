#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"

#define ADC_DEV LPC_ADC
#define ADC_RATE 40000

void adc_init(void);

void adc_enchan(uint8_t chan, uint8_t en);

void adc_start(uint8_t mode);

uint16_t adc_getdata(uint8_t chan);

uint8_t adc_getstatus(uint8_t chan);

void adc_burst(uint8_t mode);

void adc_int(uint8_t chan, uint8_t en);
