#include "tty_mbed.h"

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

void tty_init() {
	serial_init();
	tty_printf("Console started:\r\n");
}

void tty_printf(char * c) {
	char * tmp = c;
	int len = 1;
	while (*tmp++ != '\0') {
		len++;	
	}
	UART_Send((LPC_UART_TypeDef *)LPC_UART0,(uint8_t *)c,len, BLOCKING);
}
