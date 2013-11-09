#include "dbg.h"

void dbg_assert(char * msg, char * file, int line) {
	tty_printf("\x1b[31mAssert error: \x1b[93m%s:%d \x1b[0m - %s\n\r", file, line, msg);
	tty_dump_regs();
	while (1) {} //lock up
}

void dbg_warn(char * msg, char * file, int line) {
	tty_printf("\x1b[96mWarning: \x1b[93m%s:%d \x1b[0m - %s\n\r", file, line, msg);
}
