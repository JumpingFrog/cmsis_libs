#include "keypad_mbed.h"
//#include "tty_mbed.h"


char keymap[4][4] = { {'D', '#', '0', '*'}, {'C', '9', '8', '7'}, {'B', '6', '5', '4'}, {'A', '3', '2', '1'}};//used for returning chars

uint8_t send(uint8_t txd) {
		uint8_t ret;
		i2c_transfer(&txd, 1, &ret, 1, EXPAND_ADDR);	
		return ret;
	}

char keypad_scan(void) { //polls keypad till button pressed then returns.
	uint8_t tx = 0x00;
	uint8_t rx = 0x00;

	int col;
	int row;
	while (1) {
		for (col = 0; col < 4; col++) {
			tx = 0xFF ^ (1<<(4+col));
			rx = send(tx) & 0x0F;
			if (rx != 0x0F) { //a key is pressed
				rx = (~rx) & 0x0F;
				for (row = 0; row < 4; row++) {
					if ((1<<row) & rx) {
						//tty_printf("Col: %d, Row: %d, Key: %c\n\r",col, row, keymap[row][col]);
						return keymap[row][col]; //return from keymap
					}					
				}
			}
		}
	}	
}
