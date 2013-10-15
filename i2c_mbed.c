I2C_M_SETUP_Type i2cCfg;

void i2c_init(void) {
	PINSEL_CFG_Type pcfg; //pin configuration
	pcfg.Funcnum = 3;
	pcfg.OpenDrain = 0;
	pcfg.Portnum = 0;
	pcfg.Pinmode = 0;
	
	pcfg.Pinnum = 0; //SDA
	PINSEL_ConfigPin(&pcfg);
	
	pcfg.Pinnum = 1; //SCL
	
	I2C_Init(I2C_DEV, I2C_CLK);
	I2C_Cmd(I2C_DEV, ENABLE); //ready to go

	i2cCfg.retransmissions_max = I2C_LIM_RETRANSMISSION;
}

uint32_t i2c_transfer(uint8_t * tx_d, uint32_t tx_len, uint8_t * rx_buf, uint32_t rx_len, uint32_t addr) { //do an i2c operation, returns status - polling only
	i2cCfg.tx_data = tx_d;
	i2cCfg.tx_length = tx_len;
	i2cCfg.rx_data = rx_buf;
	i2cCfg.rx_length = rx_len;
	i2cCfg.sl_addr7bit = addr;

	return I2C_MasterTransferData(I2C_DEV, i2cCfg, I2C_TRANSFER_POLLING);
}

void i2c_int_transfer(uint8_t * tx_d, uint32_t tx_len, uint8_t * rx_buf, uint32_t rx_len, uint32_t addr) { //i2c operation by interrupt
	i2cCfg.tx_data = tx_d;
	i2cCfg.tx_length = tx_len;
	i2cCfg.rx_data = rx_buf;
	i2cCfg.rx_length = rx_len;
	i2cCfg.sl_addr7bit = addr;

	I2C_MasterTransferData(I2C_DEV, i2cCfg, I2C_TRANSFER_POLLING);
}

uint8_t * i2c_scan() { //returns 128 byte array 1 if ack'd, 0 otherwise.
	uint8_t ret[128];
	uint8_t buf;
	uint32_t addr = 0; 
	for (; addr < 128; addr++) {
		if (i2c_transfer(null, 0, &buf, 1, addr) == SUCCESS) {
			ret[addr] = 1;		
		}
	}
	return ret;
}

void i2c_callback(void *) {
}

void i2c_irq(Bool state) {
	 I2C_IntCmd(I2C_DEV, state);
}

void i2c_exit(void) {
	I2C_Cmd(I2C_DEV, DISABLE); //disable the I2C device
	I2C_DeInit(IC2_DEV);
}

