#include "lpc17xx_uart.h"
#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
/* CMSIS UART Wrapper (mostly centred on terminal), makes serial operations easier */
void tty_init(void);

void tty_printf(char *fmt);
