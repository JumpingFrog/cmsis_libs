#include "i2c_mbed.h"
#define LED_ADDR 0x38

void led_ctrl(uint8_t dyn, uint8_t en13, uint8_t ent24);

void led_test();

void led_digit(uint8_t val, uint8_t dig, uint8_t dp);
