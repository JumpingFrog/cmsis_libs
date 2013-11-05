#include "lpc17xx_timer.h"

void tim_init(int dev, uint32_t preval);

//prototypes
void tim_reset(int dev);
void tim_setmatch(int dev, uint8_t chan, uint32_t val);
void tim_state(int dev, uint8_t state);
void tim_clearint(int dev);
void tim_configm(int dev,int chan, int irq, int stop, int rst, int val);
