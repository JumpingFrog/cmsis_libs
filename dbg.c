void dbg_assert(char * msg, char * file, int line) {
	tty_printf("Error assertion failed: %s:%d - %s\n\r", file, line, msg)
	tty_dump_regs();
	while (1) {} //lock up
}
