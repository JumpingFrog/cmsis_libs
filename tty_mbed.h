#include "lpc17xx_uart.h"
#include "lpc_types.h"
#include "lpc17xx_pinsel.h"
#include <stdarg.h>
#include <stdio.h>

#define TTY_BUF_LEN 256 //size of the buffer

/* CMSIS UART Wrapper (mostly centred on terminal), makes serial operations easier */
void tty_init(void);

void tty_dump_regs(void);

void tty_printf(char *fmt, ...);
