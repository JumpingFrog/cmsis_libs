#include "lpc17xx_spi.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"

#define SPI_DEV LPC_SPI

void spi_init(void);

void spi_setclk(uint32_t clk);

uint16_t * spi_rx(void);

void spi_tx(uint16_t txd);
