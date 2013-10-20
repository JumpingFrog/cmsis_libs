#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_i2c.h"
#include "debug_frmwrk.h"
#include "stdlib.h"

#define ASCII_OFFSET 0x80

void serial_init() {
	UART_CFG_Type UARTConfigStruct;	
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;
	PINSEL_CFG_Type PinCfg;	// Pin configuration for UART

	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	// USB serial first
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 2;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 3;
	PINSEL_ConfigPin(&PinCfg);
		
	//default 9600 8N1
	UART_ConfigStructInit(&UARTConfigStruct);
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
	// Built the basic structures, lets start the devices/
	UART_Init((LPC_UART_TypeDef *)LPC_UART0, &UARTConfigStruct);		
	UART_FIFOConfig((LPC_UART_TypeDef *)LPC_UART0, &UARTFIFOConfigStruct); //Enable FIFO
	UART_TxCmd((LPC_UART_TypeDef *)LPC_UART0, ENABLE); //Enable Tx
}

void i2c_init() {
	PINSEL_CFG_Type PinCfg;
	
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Funcnum = 3;
	PinCfg.Portnum = 0;

	PinCfg.Pinnum = 0; //SDA
	PINSEL_ConfigPin(&PinCfg);

	PinCfg.Pinnum = 1; //SCL
	PINSEL_ConfigPin(&PinCfg);

	I2C_Init(LPC_I2C1, 100000);
	I2C_Cmd(LPC_I2C1, ENABLE);	
}

int write_serial(char *buf,int length) {
	return(UART_Send((LPC_UART_TypeDef *)LPC_UART0,(uint8_t *)buf,length, BLOCKING));
}

void led_test() {
	uint8_t data[2];
	data[0] = 0; //access ctrl
	data[1] = 0xBF; //led test word
	uint8_t rx_buf[4];

	I2C_M_SETUP_Type i2cMcfg;
	i2cMcfg.tx_data = data;
	i2cMcfg.tx_length = 2;
	i2cMcfg.rx_data = rx_buf;
	i2cMcfg.rx_length = 4;
	i2cMcfg.retransmissions_max = 3;
	i2cMcfg.callback = NULL;
	
	i2cMcfg.sl_addr7bit = 0x38;
	I2C_MasterTransferData(LPC_I2C1, &i2cMcfg, I2C_TRANSFER_POLLING);
	write_serial("LED TEST.\r\n", 11);
}

void set_ctrl(uint8_t data) {
	uint8_t tx_data[2];
	tx_data[0] = 0;
	tx_data[1] = data; 
	uint8_t rx_buf[4];

			
	I2C_M_SETUP_Type i2cMcfg;
	i2cMcfg.tx_data = tx_data;
	i2cMcfg.tx_length = 2;
	i2cMcfg.rx_data = rx_buf;
	i2cMcfg.rx_length = 4;
	i2cMcfg.retransmissions_max = 3;
	i2cMcfg.callback = NULL;
	
	i2cMcfg.sl_addr7bit = 0x38;
	I2C_MasterTransferData(LPC_I2C1, &i2cMcfg, I2C_TRANSFER_POLLING);
}

void set_led(uint8_t digit, uint8_t data, uint8_t dp) {
	uint8_t tx_data[2];
	tx_data[0] = digit; //led test word
	uint8_t rx_buf[4];

	switch (data) {
		case 0:
			tx_data[1] = 0x01 | 0x02 | 0x04 | 0x08 | 0x10 | 0x20;
			break;
		case 1:
			tx_data[1] = 0x02 | 0x04;
			break;
		case 2:
			tx_data[1] = 0x01 | 0x02 | 0x40 | 0x10 | 0x08;
			break;
		case 3:
			tx_data[1] = 0x01 | 0x02 | 0x40 | 0x04 | 0x08;
			break;
		case 4:
			tx_data[1] = 0x20 | 0x40 | 0x02 | 0x04;
			break;
		case 5:
			tx_data[1] = 0x01 | 0x20 | 0x40 | 0x04 | 0x08;
			break;
		case 6:
			tx_data[1] = 0x01 | 0x20 | 0x40 | 0x10 | 0x08 | 0x04;
			break;
		case 7:
			tx_data[1] = 0x01 | 0x02 | 0x04;
			break;
		case 8:
			tx_data[1] = 0x01 | 0x20 | 0x40 | 0x10 | 0x08 | 0x04 | 0x02;
			break;
		case 9:
			tx_data[1] = 0x01 | 0x20 | 0x40 | 0x08 | 0x04 | 0x02;
			break;
		case 0x0A:
			tx_data[1] = 0x01 | 0x20 | 0x40 | 0x10 | 0x04 | 0x02;
			break;
		case 0x0B:
			tx_data[1] = 0x20 | 0x40 | 0x10 | 0x08 | 0x04;
			break;
		case 0x0C:
			tx_data[1] = 0x01 | 0x20 | 0x10 | 0x08;
			break;
		case 0x0D:
			tx_data[1] = 0x02 | 0x40 | 0x10 | 0x08 | 0x04;
			break;
		case 0x0E:
			tx_data[1] = 0x01 | 0x20 | 0x40 | 0x10 | 0x08;
			break;
		case 0x0F:
			tx_data[1] = 0x01 | 0x20 | 0x40 | 0x10;
			break;
	}
	
	if (dp) {
		tx_data[1] |= 0x80;	
	}
			
	I2C_M_SETUP_Type i2cMcfg;
	i2cMcfg.tx_data = tx_data;
	i2cMcfg.tx_length = 2;
	i2cMcfg.rx_data = rx_buf;
	i2cMcfg.rx_length = 4;
	i2cMcfg.retransmissions_max = 3;
	i2cMcfg.callback = NULL;
	
	i2cMcfg.sl_addr7bit = 0x38;
	I2C_MasterTransferData(LPC_I2C1, &i2cMcfg, I2C_TRANSFER_POLLING);
}

void delay(void) {
	unsigned long i;
	for (i = 0; i < 100000; i++) {
	}
}

void lcd_busy(void) {
	uint8_t rx_buf[1];			
	I2C_M_SETUP_Type i2cMcfg;
	i2cMcfg.tx_data = NULL;
	i2cMcfg.tx_length = 0;
	i2cMcfg.rx_data = rx_buf;
	i2cMcfg.rx_length = 1;
	i2cMcfg.retransmissions_max = 3;
	i2cMcfg.callback = NULL;
	
	i2cMcfg.sl_addr7bit = 0x3b; //write address for lcd 

	I2C_MasterTransferData(LPC_I2C1, &i2cMcfg, I2C_TRANSFER_POLLING);

	while (rx_buf[0] & 0x80) {
		I2C_MasterTransferData(LPC_I2C1, &i2cMcfg, I2C_TRANSFER_POLLING);
		write_serial("Busy\r\n", 6);
	}
}

void lcd_home(void) {
	lcd_busy();
	uint8_t buf[4];
	buf[0] = 0x00;
	buf[1] = 0x34;
	buf[2] = 0x80;
	buf[3] = 0x02; 
	uint8_t rx_buf[4];
			
	I2C_M_SETUP_Type i2cMcfg;
	i2cMcfg.tx_data = buf;
	i2cMcfg.tx_length = 4;
	i2cMcfg.rx_data = rx_buf;
	i2cMcfg.rx_length = 0;
	i2cMcfg.retransmissions_max = 3;
	i2cMcfg.callback = NULL;
	I2C_MasterTransferData(LPC_I2C1, &i2cMcfg, I2C_TRANSFER_POLLING);

}


void lcd_clear(void) {
	uint8_t buf[17];
	buf[0] = 0x40; //writing
	int i = 1;
	for (; i < 17; i++) {
		buf[i] = 0xA0; //white space	
	} 

	uint8_t rx_buf[4];
			
	I2C_M_SETUP_Type i2cMcfg;
	i2cMcfg.tx_data = buf;
	i2cMcfg.tx_length = 17;
	i2cMcfg.rx_data = rx_buf;
	i2cMcfg.rx_length = 0;
	i2cMcfg.retransmissions_max = 3;
	i2cMcfg.callback = NULL;
	
	i2cMcfg.sl_addr7bit = 0x3b; //write address for lcd 
	I2C_MasterTransferData(LPC_I2C1, &i2cMcfg, I2C_TRANSFER_POLLING);
	lcd_busy();
	//clear line 2 now

	uint8_t buf2[2];
	buf2[0] = 0x00;
	buf2[1] = 0x80 + 0x40; //line 2 set
	i2cMcfg.tx_data = buf2;
	i2cMcfg.tx_length = 2;
	I2C_MasterTransferData(LPC_I2C1, &i2cMcfg, I2C_TRANSFER_POLLING);
	
	uint8_t buf3[16];
	buf3[0] = 0x40; //writing
	for (i=1; i < 17; i++) {
		buf3[i] = 0xA0; //white space	
	} 
	i2cMcfg.tx_data = buf3;
	i2cMcfg.tx_length = 17;
	I2C_MasterTransferData(LPC_I2C1, &i2cMcfg, I2C_TRANSFER_POLLING);
	
}

void lcd_test(void) {
	
	uint8_t tx_data[4];
	tx_data[0] = 0x00; 
	tx_data[1] = 0x20; 
	tx_data[2] = 0x0E;
	tx_data[3] = 0x06;
	uint8_t rx_buf[4];
		
	I2C_M_SETUP_Type i2cMcfg;
	i2cMcfg.tx_data = tx_data;
	i2cMcfg.tx_length = 4;
	i2cMcfg.rx_data = rx_buf;
	i2cMcfg.rx_length = 0;
	i2cMcfg.retransmissions_max = 3;
	i2cMcfg.callback = NULL;
	
	i2cMcfg.sl_addr7bit = 0x3b; //write address for lcd
	/*
	if (I2C_MasterTransferData(LPC_I2C1, &i2cMcfg, I2C_TRANSFER_POLLING) == SUCCESS) {
		write_serial("LCD says hi!\r\n", 14);	
	} //send setup  */

	uint8_t tx_data2[14];
	tx_data2[0] = 0x40;
	tx_data2[1] = (uint8_t)'H' + ASCII_OFFSET;//H
	tx_data2[2] = (uint8_t)'e' + ASCII_OFFSET;//e
	tx_data2[3] = (uint8_t)'l' + ASCII_OFFSET;//l
	tx_data2[4] = (uint8_t)'l' + ASCII_OFFSET;//l
	tx_data2[5] = (uint8_t)'o' + ASCII_OFFSET;//o
	tx_data2[6] = (uint8_t)',' + ASCII_OFFSET;//o
	tx_data2[7] = (uint8_t)' ' + ASCII_OFFSET;//o
	tx_data2[8] = (uint8_t)'w' + ASCII_OFFSET;//o
	tx_data2[9] = (uint8_t)'o' + ASCII_OFFSET;//o
	tx_data2[10] = (uint8_t)'r' + ASCII_OFFSET;//o
	tx_data2[11] = (uint8_t)'l' + ASCII_OFFSET;//o
	tx_data2[12] = (uint8_t)'d' + ASCII_OFFSET;//o
	tx_data2[13] = (uint8_t)'!' + ASCII_OFFSET;//o
	
	i2cMcfg.tx_data = tx_data2;
	i2cMcfg.tx_length = 14;
	i2cMcfg.sl_addr7bit = 0x3b; //write address for lcd (3b for read
	if (I2C_MasterTransferData(LPC_I2C1, &i2cMcfg, I2C_TRANSFER_POLLING) == SUCCESS) {
		write_serial("LCD likes data\r\n", 16);	
	} //send data 
}

int lcd_init(void) { //https://bitbucket.org/furyfire/raspi/src/6c8ca895cf0071496037ef376df4b6f44170cca3/hw/pcf2119x/main.c?at=default
	//M = 1 H0

	uint8_t tx_data[1];
	tx_data[0] = 0x30; 
	/*
	uint8_t tx_data4[10];
	tx_data4[0] = 0x00;
	tx_data4[1] = 0x34;
	tx_data4[2] = 0x0E;
	tx_data4[3] = 0x06;
	tx_data4[4] = 0x35;//8 bits, 2 line, normal instruction set
	tx_data4[5] = 0x04; //Left to right, top to bottom
	tx_data4[6] = 0x10; //set Temp
	tx_data4[7] = 0x42;
	tx_data4[8] = 0x80; //ram to start
	tx_data4[9] = 0x02; //home */
	uint8_t buf[12];
	buf[0] = 0x00; // Control byte for Instruction
        buf[1] = 0x34; // DL: 8 bits, M:  16 by two line  display, SL: 1:18, H: normal instruction set
        buf[2] = 0x0E; // D: Display on, C: cursor on, B: blink off

        buf[3] = 0x06; // I/D: increment, S: no shift
        buf[4] = 0x35; // DL: 8 bits, M: 16 by two line, SL: 1:18, H: extended instruction set
        buf[5] = 0x04; // P: left to right, Q: top to bottom
        buf[6] = 0x10; // TC1: 0, TC2: 0
        buf[7] = 0x9f; // set Vlcd, store to VA
        buf[8] = 0x34; // DL: 8 bits, M:  two line, SL: 1:18, H: normal instruction set
        buf[9] = 0x80; // DDRAM Address set to 00hex

        buf[10] = 0x02; // return home

	uint8_t rx_buf[4];
			
	I2C_M_SETUP_Type i2cMcfg;
	i2cMcfg.tx_data = buf;
	i2cMcfg.tx_length = 11;
	i2cMcfg.rx_data = rx_buf;
	i2cMcfg.rx_length = 0;
	i2cMcfg.retransmissions_max = 3;
	i2cMcfg.callback = NULL;
	
	i2cMcfg.sl_addr7bit = 0x3b; //write address for lcd 

	I2C_MasterTransferData(LPC_I2C1, &i2cMcfg, I2C_TRANSFER_POLLING);	
}


int main(void) {
	serial_init();
	i2c_init();
	write_serial("Hello, World!\n\r", 15); 
	uint32_t cur_adr = 0;
	uint8_t rx_buf[64];
	uint8_t dev_count = 0;
	char tmp[30];
	I2C_M_SETUP_Type i2cMcfg;
	i2cMcfg.tx_data = NULL;
	i2cMcfg.tx_length = 0;
	i2cMcfg.rx_data = rx_buf;
	i2cMcfg.rx_length = 1;
	i2cMcfg.retransmissions_max = 3;
	i2cMcfg.callback = NULL;
	for ( ; cur_adr < 128; cur_adr++) {
		i2cMcfg.sl_addr7bit = cur_adr;
		if (I2C_MasterTransferData(LPC_I2C1, &i2cMcfg, I2C_TRANSFER_POLLING) == SUCCESS) {
			sprintf(tmp, "%x, ", cur_adr<<1);
			write_serial(tmp, 5);
			dev_count++;
		}
	}
	sprintf(tmp, "\r\n%d i2c devices found.\r\n", dev_count);
	write_serial(tmp, 26);
	/*
	set_ctrl(0xB7); 
	uint8_t c;
	while(1){
	for (c=0; c<16; c++) {
		set_led(1, c, 0);
		set_led(2, c, 0);
		set_led(3, c, 0);
		set_led(4, c, 0);
		delay();
	}
	}*/
	lcd_init();
	lcd_clear();
	lcd_busy();
	lcd_init();
	lcd_test();
}
