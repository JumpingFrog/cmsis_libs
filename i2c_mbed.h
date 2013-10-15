#define IC2_DEV LPC_I2C1
#define I2C_CLK 100000
#define I2C_LIM_RETRANSMISSION 3
#define

#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "lpc17xx_i2c.h"

/* CMSIS I2C wrapper, makes I2C less painful.
   Bus master using one device only.
 */

void i2c_init(void);

uint32_t i2c_transfer(uint8_t * tx_d, uint32_t tx_len, uint8_t * rx_buf, uint32_t rx_len, uint32_t addr);

int * i2c_scan(void);

void i2c_exit(void);