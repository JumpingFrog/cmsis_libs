#include "tim_mbed.h"

LPC_TIM_TypeDef * tim_devs[] = { LPC_TIM0, LPC_TIM1, LPC_TIM2, LPC_TIM3 };

TIM_TIMERCFG_Type tim_cfg;

void tim_init(int dev, uint32_t preval) {
	tim_cfg.PrescaleOption = TIM_PRESCALE_USVAL; //use microseconds
	tim_cfg.PrescaleValue = preval;

	
	TIM_Init(tim_devs[dev], TIM_TIMER_MODE, &tim_cfg);//Allow setting of mode one day	
}


void tim_reset(int dev) {
	TIM_ResetCounter(tim_devs[dev]);
}

void tim_setmatch(int dev, uint8_t chan, uint32_t val) {
	TIM_UpdateMatchValue(tim_devs[dev], chan, val);
}

void tim_state(int dev, uint8_t state) {
	TIM_Cmd(tim_devs[dev], state);
}

void tim_clearint(int dev) {
	TIM_ClearIntPending(tim_devs[dev], TIM_MR0_INT);
}

void tim_configm(int dev, int chan, int irq, int stop, int rst, int val) {
	TIM_MATCHCFG_Type mcfg;
	mcfg.MatchChannel = chan;
	// Enable interrupt when MR0 matches the value in TC register
	mcfg.IntOnMatch   = irq;
	//Enable reset on MR0: TIMER will reset if MR0 matches it
	mcfg.ResetOnMatch = rst;
	//Stop on MR0 if MR0 matches it
	mcfg.StopOnMatch  = stop;
	//Toggle MR0.0 pin if MR0 matches it
	mcfg.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
	// Set Match value, count value of 10000 (10000 * 100uS = 1000000us = 1s --> 1 Hz)
	mcfg.MatchValue   = val;

	TIM_ConfigMatch(tim_devs[dev], &mcfg);
}
