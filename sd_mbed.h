/* SD Mass storage driver for MBED
   David Wotherspoon - 2013 */

#include "lpc17xx_ssp.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc_types.h"
#include "tty_mbed.h"
#include "dbg.h"

#define UoY

/* Pins */

#ifdef UoY //UoY Host board
	#define SD_PORT 0
	#define SD_FUNC 2
	
	#define SD_DEV LPC_SSP1
	#define SD_SDO 9
	#define SD_SDI 8
	#define SD_CLK 7

	#define SD_CS  11 //chip sel GPIO
#else //LPC1768 Mini DK2
	#define SD_PORT 1
	#define SD_FUNC 3

	#define SD_DEV LPC_SSP0
	#define SD_SDO 23
	#define SD_SDI 24
	#define SD_CLK 20

	#define SD_CS 21 //chip sel GPIO
	#define SD_CD 25 //detect - GPIO
#endif


/* SD SPI Command Set from http://elm-chan.org/docs/mmc/mmc_e.html */

#define CMD0   0 //Software reset.
#define CMD1   1 //Initiate init.
#define CMD8   8 //SDCv2; Voltage range check.
#define CMD9   9 //Read CSD register.
#define CMD10 10 //Read CID register.
#define CMD12 12 //Stop transmission.
#define CMD16 16 //Set block size.
#define CMD17 17 //Read a block.
#define CMD18 18 //Read multiple blocks.
#define CMD23 23 //MMC; Set blocks for next multiblock (r/w) command.
#define CMD24 24 //Write a block.
#define CMD25 25 //Write multiple blocks.
#define CMD55 55 //APP Cmd (leader for ACMDs)
	#define ACMD23 23 //SDC; Define block number for next multiblock write cmd.
	#define ACMD41 41 //SDC; Initiate init.
#define CMD58 58 //Read OCR

/* SD Card types */

#define CARDTYPE_MMC    0x00
#define CARDTYPE_SDV1   0x01
#define CARDTYPE_SDV2   0x02
#define CARDTYPE_SDV2HC 0x04

/* Misc */
#define SD_BLOCK 512


/* SD card info  - http://www.mikroe.com/download/eng/documents/development-tools/accessory-boards/storage/microsd-card-1gb/microsd_card_spec.pdf */
typedef struct {
	uint8_t mid; //manufacturer
	char oid[2]; //oem
	char pnm[5]; //product name
	uint8_t prv; //revision
	uint32_t psn; //serial num
	uint16_t mdt; //manufacturer date
} sd_cid;

typedef struct { //
	uint8_t csd_struct; //csd structure
	uint8_t taac; //date read access time-1
	uint8_t nsac; //access time in clock cycles.
	uint8_t tran_speed; //max transfer rate
	uint16_t ccc; //card command classes
	uint8_t read_bl_len; //max read block len
	uint8_t read_bl_partial; //partial blocks allowed?
	uint8_t write_misalign; //write block misalignment
	uint8_t read_misalign; //read block misalignment
	uint8_t dsr; //dsr implemented?
	uint16_t size; //device size
	uint8_t r_curr_min; //read current min
	uint8_t r_curr_max; //read current max
	uint8_t w_curr_min; //write current min
	uint8_t w_curr_max; //write current max
	uint8_t s_mult; //size multiplier
	uint8_t erase_en; //single block erase?
	uint8_t sector_size; //erase sector size
	uint8_t wp_size; //write protect size
	uint8_t wp_en; //write protect enable
	uint8_t r2w_factor; //write speed factor
	uint8_t write_bl_len; //max write block len
	uint8_t write_bl_partial; //partial block writes allowed?
	uint8_t ff_grp; //file format group
	uint8_t copy; //copy flag
	uint8_t perm_wp; //permanent write protect
	uint8_t tmp_wp; //temporary write protect
	uint8_t ff; //file format
} sd_csd;

typedef struct {
	sd_cid cid;
	sd_csd csd;
	uint32_t blocksize;
	uint32_t capacity;
	uint32_t space_total;
	uint32_t block_count;
} sd_info;

/* Public prototypes */
uint8_t sd_init(void); //initialise sd card

uint8_t sd_readblock(uint8_t * buf, uint32_t lba);
uint8_t sd_readblocks(uint8_t * buf, uint32_t lba, uint32_t num);
uint8_t sd_writeblock(uint8_t *buf, uint32_t lba);
uint8_t sd_writeblocks(uint8_t * buf, uint32_t lba, uint32_t num);
void sd_cs(int state);
void sd_wl(int state);
void sd_rl(int state);
void ssp_init(void);
uint16_t spi_readwrite(uint8_t in);
sd_info* sd_readinfo(void);

