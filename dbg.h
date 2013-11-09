#include "tty_mbed.h"
/* Debug library for mbed, provides assert etc */
void dbg_assert(char * msg, char * file, int line);
void dbg_warn(char * msg, char* file, int line);
//the main assert macro really important.
#define ASSERT(X, Y)  { if (X) {} else { dbg_assert(Y, __FILE__, __LINE__); } } 
#define WARN(X, Y)    { if (X) {} else { dbg_warn(Y, __FILE__, __LINE__);   } }
