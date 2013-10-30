#include "spi_mbed.h"

SPI_CFG_Type spicfg;
SPI_DATA_SETUP_Type spidcfg;

uint16_t spi_rxBuf;

enum spi_status { //bindings for status
	abort = SPI_STAT_ABRT, //Slave abort.
    mode = SPI_STAT_MODF, //Mode fault.
    rovr = SPI_STAT_ROVR, //Read overrun.
    wcol = SPI_STAT_WCOL, //Write collision.
    done = SPI_STAT_SPIF //transfer complete.
};

void spi_init(void) {
	PINSEL_CFG_Type pcfg; //pin configuration
	pcfg.Funcnum = 2;
	pcfg.OpenDrain = 0;
	pcfg.Portnum = 0;
	pcfg.Pinmode = 0;
	
	pcfg.Pinnum = 7; //SCLK
	PINSEL_ConfigPin(&pcfg);
	
	pcfg.Pinnum = 8; //MISO
	PINSEL_ConfigPin(&pcfg);

	pcfg.Pinnum = 9; //MOSI
	PINSEL_ConfigPin(&pcfg);
	
	pcfg.Funcnum = 0;
	pcfg.Pinnum = 11; //SDCS
	PINSEL_ConfigPin(&pcfg);


	SPI_Config_StructInit(&spicfg); //fill spicfg to default vals
	SPI_Init(SPI_DEV, &spicfg);

	
}

void spi_setclk(uint32_t clk) {
	SPI_SetClock(SPI_DEV, clk);
}

void spi_setint(uint8_t en) {
	SPI_IntCmd(SPI_DEV, (en) ? ENABLE : DISABLE);
}

void spi_clearint(void) { //clear any interrupts
	SPI_ClearIntPending(SPI_DEV);
}


uint16_t spi_rx(void) {
	return SPI_ReceiveData(SPI_DEV);
}

uint8_t spi_datasize() {
	return SPI_GetDataSize(SPI_DEV);
}

void spi_tx(uint16_t txd) {
	SPI_SendData(SPI_DEV, txd);
}
