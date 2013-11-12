#include "sd_mbed.h"

SSP_DATA_SETUP_Type sd_data;
int const SD_RETRY = 200;
int sd_type;

void ssp_init() { //initalise ssp for the SD
	sd_cs(1); //disable the card. (active low)
	PINSEL_CFG_Type pcfg; //pin configuration
	SSP_CFG_Type sspcfg;

	pcfg.Portnum = SD_PORT;
	pcfg.OpenDrain = 0;
	pcfg.Pinmode = 0;
	pcfg.Funcnum = SD_FUNC;
	
	//SD_SDO
	pcfg.Pinnum = SD_SDO;
	PINSEL_ConfigPin(&pcfg);

	//SD_SDI
	pcfg.Pinnum = SD_SDI;
	PINSEL_ConfigPin(&pcfg);

	//SD_CLK
	pcfg.Pinnum = SD_CLK;
	PINSEL_ConfigPin(&pcfg);

	//SD_CS
	pcfg.Funcnum = 0; //GPIO
	pcfg.Pinnum = SD_CS;
	PINSEL_ConfigPin(&pcfg);

	GPIO_SetDir(SD_PORT, (1<<SD_CS), 1); //GPIO for CS

	//SD_CD - Not on UoY Host board, just hope SD is inserted.
	

	SSP_ConfigStructInit(&sspcfg); //Configure struct
	sspcfg.ClockRate = 400000; //must be 400KHz

	SSP_Init(SD_DEV, &sspcfg);
	SSP_Cmd(SD_DEV, ENABLE);
}

void spi_tx(uint16_t data) { //macro would be better
	SSP_SendData(SD_DEV, data);
}

uint16_t spi_rx(void) { //macro would be better.
	return SSP_ReceiveData(SD_DEV);
}

uint32_t spi_rxtx(uint32_t * txd, uint32_t * rxd, uint32_t len) { //returns data length transferred
	sd_data.length = len;
	sd_data.rx_cnt = 0; //may not be needed
	sd_data.rx_data = &rxd;
	sd_data.tx_cnt = 0; //may not be needed
	sd_data.tx_data = &txd;

	return SSP_ReadWrite(SD_DEV, &sd_data, SSP_TRANSFER_POLLING);
}

uint8_t sd_cmd(uint8_t cmd, uint32_t arg, uint8_t crc) {
	int c;
	uint8_t r1;
	uint8_t buf[] = { cmd | 0x40, arg>>24, arg>>16, arg>>8, arg, crc }; //always 6
	spi_readwrite(0xFF);

	for (c = 0; c < 6; c++) { //use rxtx?
			spi_tx(buf[c]);
	}
	//read

	for (c = 0; c < SD_RETRY; c++) {
		if ((r1 = spi_readwrite(0xFF)) != 0xFF) {
			break;		
		}	
	}		
	return r1; //spi_rx();
}

uint16_t spi_readwrite(uint8_t in) {
	SSP_SendData(SD_DEV, in);
	return SSP_ReceiveData(SD_DEV);
}


void sd_init(void) { //http://elm-chan.org/docs/mmc/mmc_e.html
	int i;
	int r1;
	ssp_init();
	tty_printf("SPI bus initialised.\n\r");
	sd_cs(1); //toggling this makes it work more often.
	sd_cs(0);
	
	for (i = 0; i < 10; i++) { //send 74 clocks (80)
		spi_readwrite(0xFF);
	}
	for (i = 0; i <= SD_RETRY; i++) {
		r1 = sd_cmd(CMD0, 0, 0x95); // soft reset
		if (r1 == 0x01) {
			tty_printf("SD card reset to IDLE\n\r");
			break;
		}
		else {
			tty_printf("Response %x to CMD0\n\r", r1);
		}
	}
	ASSERT((r1 == 0x01), "Failed to reset SD.");

	r1 = sd_cmd(CMD8, 0x1AA, 0x87); //Try to determine SD card type.
	if (r1 == 0x05) {
		tty_puts("\n\rCard is SDCv1/MMC\n\r");	
		//*Do Init Stuff*//
	}
	else if (r1 == 0x01) {//SDC v2 Mode
		tty_puts("Card is SDC v2\n\r");
		uint8_t buf[] = { spi_readwrite(0xFF), spi_readwrite(0xFF), spi_readwrite(0xFF), spi_readwrite(0xFF)};
		if (buf[2] == 0x01 && buf[3] == 0xAA) {
			for (i = 0; i <= SD_RETRY; i++) {
				r1 = sd_cmd(CMD55, 0x00, 0x56);
				if (r1 == 0x05) {
					THROW("Card thinks CMD55 is illegal :/");
				}
				r1 = sd_cmd(ACMD41, 0x40000000, 0x01);
				if (r1 == 0x01) {
					tty_printf("Card initalised (%d retries)..\n\r", i);
					break;
				}
				spi_tx(0xFF);
			}
			
			r1 = sd_cmd(CMD58, 0, 0x01);
			if (r1 == 0x01) {
				buf[0] = spi_rx();
				buf[1] = spi_rx();
				buf[2] = spi_rx();
				buf[3] = spi_rx();

				tty_printf("CMD58 Ret: %x, %x, %x, %x\n\r", buf[0], buf[1], buf[2], buf[3]);
				if (buf[0] & 0x40) {
					sd_type = CARDTYPE_SDV2HC;
					tty_printf("Card is SDv2HC\n\r");
				}
				else {
					sd_type = CARDTYPE_SDV2;
					tty_printf("Card is SDv2\n\r");
				}
				return 0;
			}
			else {
				THROW("CMD58 error.");
			}	
		} 
		else {
			THROW("Card rejected on CMD8.");
		}
			
	}
	else {
		THROW("Unknown response to CMD8.");
	}
}

void sd_cs(int state) { //chipselect
	if (state) {
		GPIO_SetValue(SD_PORT, (1<<SD_CS));
	}
	else {
		GPIO_ClearValue(SD_PORT, (1<<SD_CS));
	}
}

void sd_readblock(uint8_t * buf, uint32_t lba) {
	uint8_t r1;
	int i;
	sd_cs(1);
	sd_cs(0);
	spi_tx(0xFF);
	
	if (sd_type == CARDTYPE_SDV2HC) {
		lba <<= 9; //shift lba up 512 (block size)
	}

	for (i = 0; i < SD_RETRY; i++) {
		r1 = sd_cmd(CMD17, lba, 0x01);
		if (r1 == 0x00) {
			break;
		}
	}
	
	ASSERT(r1 == 0x00, "CMD17 read timeout.");
	//tty_printf("ACCEPTED\n\r");
	for (i = 0; i < 0xFFFFFFFF; i++) {
		r1 = spi_readwrite(0xFF);
		(r1 != 0xFF) ? tty_printf("CMD17 ret: %x\n\r", r1) : NULL;
		if (r1 == 0xFE) {
			break;
		}
	}
	ASSERT(r1 == 0xFE, "Read timeout.");
	
	for (i=0; i< (SD_BLOCK); i++) {
		*(buf+i) = spi_readwrite(0xFF);
		tty_printf("Read byte: %d, Value was %x\n\r", i, *(buf+i));
	}
	//IGNORE CRC (2 bytes)
	spi_readwrite(0xFF);
	spi_readwrite(0xFF);
	//DONE
	sd_cmd(CMD12, 0, 0x01);
}
