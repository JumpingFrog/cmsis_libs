#include "adc_mbed.h"

PINSEL_CFG_Type adc_pcfg;

void adc_init(void) {
	adc_pcfg.OpenDrain = 0;
	adc_pcfg.Pinmode = 0;
	adc_pcfg.Funcnum = 1;
	ADC_Init(ADC_DEV, ADC_RATE);
}

void adc_enchan(uint8_t chan, uint8_t en) { //enable a channel
	//first we set the pin (p15 to 20 on mbed)
	switch(chan) {
		case 0:
			adc_pcfg.Pinnum = 23;
			adc_pcfg.Portnum = 0;
			adc_pcfg.Funcnum = 1;
			ADC_IntConfig(ADC_DEV, ADC_ADINTEN0, RESET); 
			break;
		case 1:
			adc_pcfg.Pinnum = 24;
			adc_pcfg.Portnum = 0;
			adc_pcfg.Funcnum = 1;
			ADC_IntConfig(ADC_DEV, ADC_ADINTEN1, RESET); 
			break;
		case 2:
			adc_pcfg.Pinnum = 25;
			adc_pcfg.Portnum = 0;
			adc_pcfg.Funcnum = 1;
			ADC_IntConfig(ADC_DEV, ADC_ADINTEN2, RESET); 
			break; 
		case 3: //also DAC (func 2)(MUTEX).
			adc_pcfg.Pinnum = 26;
			adc_pcfg.Portnum = 0;
			adc_pcfg.Funcnum = 1;
			ADC_IntConfig(ADC_DEV, ADC_ADINTEN3, RESET); 
			break;
		case 4: //actual audio socket on UoY host board
			adc_pcfg.Pinnum = 30;
			adc_pcfg.Portnum = 1;
			adc_pcfg.Funcnum = 3;
			ADC_IntConfig(ADC_DEV, ADC_ADINTEN4, RESET); 
			break;
		case 5:
			adc_pcfg.Pinnum = 31;
			adc_pcfg.Portnum =	1;
			adc_pcfg.Funcnum = 3;
			ADC_IntConfig(ADC_DEV, ADC_ADINTEN5, RESET); 
			break;
		default:
			return; //nothing done, invalid chan
	}
	PINSEL_ConfigPin(&adc_pcfg);
	ADC_ChannelCmd(ADC_DEV, chan, (en) ? ENABLE : DISABLE);
}

void adc_int(uint8_t chan, uint8_t en) { //set interrupt config for a channel
	switch(chan) {
		case 0:
			ADC_IntConfig(ADC_DEV, ADC_ADINTEN0, en); 
			break;
		case 1:
			ADC_IntConfig(ADC_DEV, ADC_ADINTEN1, en); 
			break;
		case 2:
			ADC_IntConfig(ADC_DEV, ADC_ADINTEN2, en); 
			break;
		case 3:
			ADC_IntConfig(ADC_DEV, ADC_ADINTEN3, en); 
			break;
		case 4:
			ADC_IntConfig(ADC_DEV, ADC_ADINTEN4, en); 
			break;
		case 5:
			ADC_IntConfig(ADC_DEV, ADC_ADINTEN5, en); 
			break;
		default:
			return;	
	}
}

void adc_start(uint8_t mode) { //n.b. allow modes other than cont
	ADC_StartCmd(ADC_DEV, ADC_START_CONTINUOUS);
}

uint16_t adc_getdata(uint8_t chan) { //read back data for a channel
	return ADC_ChannelGetData(ADC_DEV, chan);
}

void adc_burst(uint8_t mode) { //enable/disable burst mode
	ADC_BurstCmd(ADC_DEV, mode);
}

uint8_t adc_getstatus(uint8_t chan) { //get the status (for polling)
	if (ADC_ChannelGetStatus(ADC_DEV, chan, 1) == SET) {
		return 1;
	}
	return 0;
}
