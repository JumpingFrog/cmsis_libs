#include "tty_mbed.h"
/* Debug library for mbed, provides assert etc */

//the main assert macro really important.
#define ASSERT(X, Y) { \
		if (X) {} \
		else { dbg_assert(Y, __FILE__, __LINE__); } }

void dbg_assert(char * msg, char * file, int line);
