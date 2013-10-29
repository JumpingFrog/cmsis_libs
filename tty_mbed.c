#include "tty_mbed.h"

char tty_buf[TTY_BUF_LEN];

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

void tty_dump_regs() { //http://www.ethernut.de/en/documents/arm-inline-asm.html
	tty_printf("DEBUG: Dumping registers:\n\r");
	int tmp;
	__asm volatile("mov %[res], r0\n\t" : [res] "=r" (tmp));
	tty_printf("r0: 0x%x\n\r", tmp);
	__asm volatile("mov %[res], r1\n\t" : [res] "=r" (tmp));
	tty_printf("r1: 0x%x\n\r", tmp);
	__asm volatile("mov %[res], r2\n\t" : [res] "=r" (tmp));
	tty_printf("r2: 0x%x\n\r", tmp);
	__asm volatile("mov %[res], r3\n\t" : [res] "=r" (tmp));
	tty_printf("r3: 0x%x\n\r", tmp);
	__asm volatile("mov %[res], r4\n\t" : [res] "=r" (tmp));
	tty_printf("r4: 0x%x\n\r", tmp);
	__asm volatile("mov %[res], r5\n\t" : [res] "=r" (tmp));
	tty_printf("r5: 0x%x\n\r", tmp);
	__asm volatile("mov %[res], r6\n\t" : [res] "=r" (tmp));
	tty_printf("r6: 0x%x\n\r", tmp);
	__asm volatile("mov %[res], r7\n\t" : [res] "=r" (tmp));
	tty_printf("r7: 0x%x\n\r", tmp);
	__asm volatile("mov %[res], r8\n\t" : [res] "=r" (tmp));
	tty_printf("r8: 0x%x\n\r", tmp);
	__asm volatile("mov %[res], r9\n\t" : [res] "=r" (tmp));
	tty_printf("r9: 0x%x\n\r", tmp);
	__asm volatile("mov %[res], r10\n\t" : [res] "=r" (tmp));
	tty_printf("r10: 0x%x\n\r", tmp);
	__asm volatile("mov %[res], r11\n\t" : [res] "=r" (tmp));
	tty_printf("r11: 0x%x\n\r", tmp);
	__asm volatile("mov %[res], r12\n\t" : [res] "=r" (tmp));
	tty_printf("r12: 0x%x\n\r", tmp);
	__asm volatile("mov %[res], r13\n\t" : [res] "=r" (tmp));
	tty_printf("r13: 0x%x\n\r", tmp);
	__asm volatile("mov %[res], r14\n\t" : [res] "=r" (tmp));
	tty_printf("r14: 0x%x\n\r", tmp);
	__asm volatile("mov %[res], r15\n\t" : [res] "=r" (tmp));
	tty_printf("r15: 0x%x\n\r", tmp);
	tty_printf("---------------------------------------------\n\r");
	__asm volatile("mov %[res], sp\n\t" : [res] "=r" (tmp));
	tty_printf("sp: 0x%x\n\r", tmp);
	__asm volatile("mov %[res], fp\n\t" : [res] "=r" (tmp));
	tty_printf("fp: 0x%x\n\r", tmp);
	__asm volatile("mov %[res], lr\n\t" : [res] "=r" (tmp));
	tty_printf("lr: 0x%x\n\r", tmp);
	
}

void tty_printf(char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vsprintf(tty_buf, fmt, args);
	va_end(args);
	char * tmp = tty_buf;
	int len = 1;
	while (*tmp++ != '\0') {
		len++;	
	}
	UART_Send((LPC_UART_TypeDef *)LPC_UART0,(uint8_t *)tty_buf,len, BLOCKING);
}
