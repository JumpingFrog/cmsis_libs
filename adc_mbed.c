#include "adc_mbed.h"

PINSEL_CFG_Type adc_pcfg;

void adc_init(void) {
	adc_pcfg.OpenDrain = 0;
	adc_pcfg.Pinmode = 0;
	adc_pcfg.Funcnum = 1;
	ADC_Init(ADC_DEV, ADC_RATE);
}

void adc_enchan(uint8_t chan, uint8_t en) {
	//first we set the pin (p15 to 20 on mbed)
	switch(chan) {
		case 0:
			adc_pcfg.Pinnum = 23;
			adc_pcfg.Portnum =	0;
			adc_pcfg.Funcnum = 1;
			break;
		case 1:
			adc_pcfg.Pinnum = 24;
			adc_pcfg.Portnum = 0;
			adc_pcfg.Funcnum = 1;
			break;
		case 2:
			adc_pcfg.Pinnum = 25;
			adc_pcfg.Portnum =	0;
			adc_pcfg.Funcnum = 1;
			break;
		case 3: //also DAC (func 2)(MUTEX).
			adc_pcfg.Pinnum = 26;
			adc_pcfg.Portnum = 0;
			adc_pcfg.Funcnum = 1;
			break;
		case 4: //actual audio socket on UoY host board
			adc_pcfg.Pinnum = 30;
			adc_pcfg.Portnum =	1;
			adc_pcfg.Funcnum = 2; //PCC says 3?
			break;
		case 5:
			adc_pcfg.Pinnum = 31;
			adc_pcfg.Portnum =	1;
			adc_pcfg.Funcnum = 2;
			break;
		default:
			return;
	}
	PINSEL_ConfigPin(adc_pcfg);
	ADC_ChannelCmd(chan, (en) ? Enable : Disable);
}

uint16_t adc_getdata(uint8_t chan) {
	return ADC_ChannelGetData(ADC_DEV, chan);
}
