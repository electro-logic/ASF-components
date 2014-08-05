// NRF24L01+ Single Chip 2.4GHz Transceiver

#include "nrf24l01.h"

// SPI select pin
struct spi_device nrf24l01_spi_device_conf = { .id = CONF_NRF24L01_CSn_PIN };

//// IRQ pin fallen
//ISR(PORTD_INT0_vect)
//{
//}
//
//// AVR1313
//void PORT_ConfigureInterrupt0( PORT_t * port, PORT_INT0LVL_t intLevel, uint8_t pinMask )
//{
	//port->INTCTRL = ( port->INTCTRL & ~PORT_INT0LVL_gm ) | intLevel;
	//port->INT0MASK = pinMask;
//}

uint8_t PTX;	// Sending mode

// Initialize Pin and SPI like specified into config file
void nrf24l01_init()
{
	// Init SPI pins
	ioport_set_pin_dir(CONF_NRF24L01_SS_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(CONF_NRF24L01_SS_PIN, IOPORT_MODE_PULLUP);
	ioport_set_pin_dir(CONF_NRF24L01_MOSI_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_mode(CONF_NRF24L01_MOSI_PIN, IOPORT_MODE_PULLUP);
	ioport_set_pin_high(CONF_NRF24L01_MOSI_PIN);
	ioport_set_pin_dir(CONF_NRF24L01_MISO_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_dir(CONF_NRF24L01_SCK_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_high(CONF_NRF24L01_SCK_PIN);
	
	// Init nrf24l01 pins
	ioport_set_pin_dir(CONF_NRF24L01_CE_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(CONF_NRF24L01_CSn_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(CONF_NRF24L01_IRQ_PIN, IOPORT_DIR_INPUT);

	ioport_set_pin_low(CONF_NRF24L01_CE_PIN);
	spi_deselect_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf);
	
	spi_master_init(&CONF_NRF24L01_SPI);
	spi_master_setup_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf, SPI_MODE_0, CONF_NRF24L01_CLOCK_SPEED, 0);
	spi_enable(&CONF_NRF24L01_SPI);
	
	// Wait nrf24l01 power on reset
	delay_ms(150);
	
	// Config parameters sets in CONF_NRF24L01
	nrf24l01_set_power_amplifier(CONF_NRF24L01_PA);
	nrf24l01_set_data_rate(CONF_NRF24L01_DATA_RATE);
	nrf24l01_set_crc(CONF_NRF24L01_CRC);
	nrf24l01_set_rx_addr(CONF_NRF24L01_RX_ADDR);
	nrf24l01_set_tx_addr(CONF_NRF24L01_TX_ADDR);
	nrf24l01_write_register(NRF24L01_RF_CH_REG, CONF_NRF24L01_RF_CHANNEL);
	nrf24l01_write_register(NRF24L01_RX_PW_P0_REG, CONF_NRF24L01_PAYLOAD);
	nrf24l01_write_register(NRF24L01_RX_PW_P1_REG, CONF_NRF24L01_PAYLOAD);	
}

// Power Up in RX Mode
void nrf24l01_primary_rx()
{
	PTX = 0;
	ioport_set_pin_low(CONF_NRF24L01_CE_PIN);
	
	uint8_t configReg = nrf24l01_read_register(NRF24L01_CONFIG_REG);
	nrf24l01_write_register(NRF24L01_CONFIG_REG, configReg | NRF24L01_PWR_UP_BM | NRF24L01_PRIM_RX_BM);
	//nrf24l01_write_register(NRF24L01_CONFIG_REG, NRF24L01_EN_CRC_BM | NRF24L01_PWR_UP_BM | NRF24L01_PRIM_RX_BM);

	ioport_set_pin_high(CONF_NRF24L01_CE_PIN);
	
	//uint8_t statusReg = nrf24l01_read_register(NRF24L01_STATUS_REG);
	nrf24l01_write_register(NRF24L01_STATUS_REG, NRF24L01_TX_DS_BM | NRF24L01_MAX_RT_BM);
}

// Power Up in TX Mode
void nrf24l01_primary_tx()
{	
	PTX = 1;
	uint8_t config_reg = nrf24l01_read_register(NRF24L01_CONFIG_REG);
	config_reg |= NRF24L01_PWR_UP_BM;			// Power Up
	config_reg &= ~NRF24L01_PRIM_RX_BM;			// PTX mode
	nrf24l01_write_register(NRF24L01_CONFIG_REG, config_reg);		
	//nrf24l01_write_register(NRF24L01_CONFIG_REG,NRF24L01_EN_CRC_BM | NRF24L01_PWR_UP_BM);
}

//void nrf24l01_power_off()
//{
	//// TODO: Wait that nrf finish rx_read, tx_send or tx_wait_ack
	//delay_ms(100); // TODO: delete
//
	//uint8_t config_reg = nrf24l01_read_register(NRF24L01_CONFIG_REG);
	//config_reg &= ~NRF24L01_PWR_UP_BM;			
	//nrf24l01_write_register(NRF24L01_CONFIG_REG, config_reg);
//}

void nrf24l01_set_rf_channel(uint8_t channel)
{
	nrf24l01_write_register(NRF24L01_RF_CH_REG, channel);	
}

void nrf24l01_set_crc(enum nrf24l01_crc crc)
{
	uint8_t config_reg = nrf24l01_read_register(NRF24L01_CONFIG_REG);
	switch(crc)
	{
		case NRF24L01_CRC_DISABLED:
			config_reg &= ~NRF24L01_EN_CRC_BM;
			break;
		case NRF24L01_CRC_1B:
			config_reg |= NRF24L01_EN_CRC_BM;
			config_reg &= ~NRF24L01_CRCO_BM;
			break;
		case NRF24L01_CRC_2B:
			config_reg |= NRF24L01_EN_CRC_BM | NRF24L01_CRCO_BM;
			break;
	}
	nrf24l01_write_register(NRF24L01_CONFIG_REG, config_reg);	
};

void nrf24l01_set_data_rate(enum nrf24l01_data_rate dataRate)
{
	uint8_t rf_setup_reg = nrf24l01_read_register(NRF24L01_RF_SETUP_REG);
	switch (dataRate)
	{
		case NRF24L01_250kbps:
		rf_setup_reg |= NRF24L01_RF_DR_LOW_BM;
		rf_setup_reg &= ~NRF24L01_RF_DR_HIGH_BM;
		break;
		case NRF24L01_1Mbps:
		rf_setup_reg &= (~NRF24L01_RF_DR_HIGH_BM) & (~NRF24L01_RF_DR_HIGH_BM);
		break;
		case NRF24L01_2Mbps:
		rf_setup_reg |= NRF24L01_RF_DR_HIGH_BM;
		rf_setup_reg &= ~NRF24L01_RF_DR_LOW_BM;
		break;
	}
	nrf24l01_write_register(NRF24L01_RF_SETUP_REG, rf_setup_reg);
}

void nrf24l01_send_command(uint8_t command)
{
	spi_select_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf);
	spi_write_packet(&CONF_NRF24L01_SPI, &command, 1);
	spi_deselect_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf);
}

void nrf24l01_flush_tx(void)
{
	nrf24l01_send_command(NRF24L01_FLUSH_TX);
}

void nrf24l01_flush_rx(void)
{
	nrf24l01_send_command(NRF24L01_FLUSH_RX);
}

void nrf24l01_set_power_amplifier(enum nrf24l01_power_amplifier pa)
{
	uint8_t rf_setup_reg = nrf24l01_read_register(NRF24L01_RF_SETUP_REG);
	switch (pa)
	{
		case NRF24L01_PA_0dBm:
			rf_setup_reg |= NRF24L01_RF_PWR_BM;
			break;
		case NRF24L01_PA_less_6dBm:
			rf_setup_reg |= (NRF24L01_RF_PWR_BM & 0x02);
			break;
		case NRF24L01_PA_less_12dBm:
			rf_setup_reg |= (NRF24L01_RF_PWR_BM & 0x01);
			break;
		case NRF24L01_PA_less_18dBm:
			rf_setup_reg &= (~NRF24L01_RF_PWR_BM);
			break;
	}
	nrf24l01_write_register(NRF24L01_RF_SETUP_REG, rf_setup_reg);
};

// reg: 5 bit memory map address (LSB first)
uint8_t nrf24l01_read_register(uint8_t reg)
{
	spi_select_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf);
	uint8_t data = NRF24L01_R_REGISTER | (NRF24L01_REGISTER_MASK & reg);
	spi_write_packet(&CONF_NRF24L01_SPI, &data, 1);
	uint8_t readData;
	spi_read_packet(&CONF_NRF24L01_SPI, &readData, 1);
	spi_deselect_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf);
	return readData;
}

uint8_t* nrf24l01_read_array_register(uint8_t reg, uint8_t* readData, uint8_t dataLen)
{
	spi_select_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf);
	uint8_t data = NRF24L01_R_REGISTER | (NRF24L01_REGISTER_MASK & reg);
	spi_write_packet(&CONF_NRF24L01_SPI, &data, 1);	
	spi_read_packet(&CONF_NRF24L01_SPI, readData, dataLen);
	spi_deselect_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf);
	return readData;
}

// reg: 5 bit memory map address (LSB first)
// Note: Executable in power down or standby modes only
void nrf24l01_write_register(uint8_t reg, uint8_t writeData)
{
	spi_select_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf);
	uint8_t data[2] = {NRF24L01_W_REGISTER | (NRF24L01_REGISTER_MASK & reg), writeData };
	spi_write_packet(&CONF_NRF24L01_SPI, data, 2);
	spi_deselect_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf);	
}

// reg: 5 bit memory map address (LSB first)
// Note: Executable in power down or standby modes only
void nrf24l01_write_array_register(uint8_t reg, uint8_t* writeData, uint8_t dataLen)
{
	spi_select_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf);
	uint8_t data[1] = {NRF24L01_W_REGISTER | reg };
	spi_write_packet(&CONF_NRF24L01_SPI, data, 1);
	spi_write_packet(&CONF_NRF24L01_SPI, writeData, dataLen);
	spi_deselect_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf);
}

// Sets the receiving address
void nrf24l01_set_rx_addr(uint8_t* addr)
{
	ioport_set_pin_low(CONF_NRF24L01_CE_PIN);
	nrf24l01_write_array_register(NRF24L01_RX_ADDR_P1_REG, addr, NRF24L01_ADDR_LEN);
	ioport_set_pin_high(CONF_NRF24L01_CE_PIN);;
}

// Sets the transmitting address
void nrf24l01_set_tx_addr(uint8_t* addr)
{
	// RX_ADDR_P0 must be set to the sending addr for auto ack to work.	 
	nrf24l01_write_array_register(NRF24L01_RX_ADDR_P0_REG, addr, NRF24L01_ADDR_LEN);
	nrf24l01_write_array_register(NRF24L01_TX_ADDR_REG, addr, NRF24L01_ADDR_LEN);
}

// Checks if data is available for reading
bool nrf24l01_dataReady() 
{
    // See note in getData() function - just checking RX_DR isn't good enough
	uint8_t status = nrf24l01_read_register(NRF24L01_STATUS_REG);

    // We can short circuit on RX_DR, but if it's not set, we still need
    // to check the FIFO for any pending packets
    if (status & NRF24L01_RX_DR_BM) 
		return true;
		
	// return true if rx fifo is not empty
	uint8_t fifoStatus = nrf24l01_read_register(NRF24L01_FIFO_STATUS_REG);
	return !(fifoStatus & NRF24L01_RX_EMPTY_BM);
}

// Reads payload bytes into data array
void nrf24l01_receive_data(uint8_t* data) 
{
    spi_select_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf); 
	spi_write_single(&CONF_NRF24L01_SPI, NRF24L01_R_RX_PAYLOAD);	
	spi_read_packet(&CONF_NRF24L01_SPI, data, CONF_NRF24L01_PAYLOAD);
    spi_deselect_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf);                              
	
    // NVI: per product spec, p 67, note c:
    //  "The RX_DR IRQ is asserted by a new packet arrival event. The procedure
    //  for handling this interrupt should be: 1) read payload through SPI,
    //  2) clear RX_DR IRQ, 3) read FIFO_STATUS to check if there are more 
    //  payloads available in RX FIFO, 4) if there are more data in RX FIFO,
    //  repeat from step 1)."
    // So if we're going to clear RX_DR here, we need to check the RX FIFO
    // in the dataReady() function
    nrf24l01_write_register(NRF24L01_STATUS_REG, NRF24L01_RX_DR_BM);   // Reset status register
}

// Sends a data package to the default address. Be sure to send the correct
// amount of bytes as configured as payload on the receiver.
void nrf24l01_send_data(uint8_t* value) 
{    
	// Wait until last packet is send if in TX mode
    while (PTX) {
	    uint8_t status = nrf24l01_read_register(NRF24L01_STATUS_REG);
		// Packet transmitted or Max retransmission
	    if((status & (NRF24L01_TX_DS_BM  | NRF24L01_MAX_RT_BM))){
		    PTX = 0;
		    break;
	    }
    } 

    ioport_set_pin_low(CONF_NRF24L01_CE_PIN);;
    
	nrf24l01_primary_tx();    
	
	delay_ms(1);
  	nrf24l01_flush_tx();
		
    spi_select_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf);  
	spi_write_single(&CONF_NRF24L01_SPI, NRF24L01_W_TX_PAYLOAD);
	spi_write_packet(&CONF_NRF24L01_SPI, value, CONF_NRF24L01_PAYLOAD);
    spi_deselect_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf); 
    
	ioport_set_pin_high(CONF_NRF24L01_CE_PIN);                     // Start transmission
}

// Test if chip is still sending. When sending has finished return chip to listening.
bool nrf24l01_isSending(){
	if(PTX){
	    uint8_t status = nrf24l01_read_register(NRF24L01_STATUS_REG);
		
		
		//#define NRF24L01_TX_DS_BM		(1 << 5)
		//#define NRF24L01_MAX_RT_BM		(1 << 4)

		// if sending successful (TX_DS) or max retries exceded (MAX_RT).
		if((status & (NRF24L01_TX_DS_BM  | NRF24L01_MAX_RT_BM))){
			nrf24l01_primary_rx();			
			return false; 
		}

		return true;
	}
	return false;
}

/*
void Debug()
{
	uint8_t config = nrf24l01_read_register(NRF24L01_CONFIG_REG);
	uint8_t en_aa = nrf24l01_read_register(NRF24L01_EN_AA_REG);
	uint8_t en_rxaddr = nrf24l01_read_register(NRF24L01_EN_RXADDR_REG);
	uint8_t setup_aw = nrf24l01_read_register(NRF24L01_SETUP_AW_REG);
	uint8_t setup_retr = nrf24l01_read_register(NRF24L01_SETUP_RETR_REG);
	uint8_t rf_ch = nrf24l01_read_register(NRF24L01_RF_CH_REG);
	uint8_t rf_setup = nrf24l01_read_register(NRF24L01_RF_SETUP_REG);
	uint8_t status = nrf24l01_read_register(NRF24L01_STATUS_REG);
	uint8_t observe_tx = nrf24l01_read_register(NRF24L01_OBSERVE_TX_REG);
	uint8_t rpd = nrf24l01_read_register(NRF24L01_RPD_REG);
	uint8_t fifo_status = nrf24l01_read_register(NRF24L01_FIFO_STATUS_REG);
	uint8_t dynpd = nrf24l01_read_register(NRF24L01_DYNPD_REG);
	uint8_t feature = nrf24l01_read_register(NRF24L01_FEATURE_REG);
	uint64_t rx_addr_p0 = 0; nrf24l01_read_array_register(NRF24L01_RX_ADDR_P0_REG, (uint8_t*) &rx_addr_p0, 40);
	uint64_t rx_addr_p1 = 0; nrf24l01_read_array_register(NRF24L01_RX_ADDR_P1_REG, (uint8_t*) &rx_addr_p1, 40);
	uint64_t tx_addr = 0; nrf24l01_read_array_register(NRF24L01_TX_ADDR_REG, (uint8_t*) &tx_addr, 40);
	uint8_t rx_pw_p0 = nrf24l01_read_register(NRF24L01_RX_PW_P0_REG);
	uint8_t rx_pw_p1 = nrf24l01_read_register(NRF24L01_RX_PW_P1_REG);

	printf("\r\nREGISTER	(DEFAULT): (VALUE)\r\n");
	printf("CONFIG		(0x08): %x\r\n", config);
	printf("\tMASK_RX_DR	: %x\r\n", config & NRF24L01_MASK_RX_DR_BM);
	printf("\tMASK_TX_DS	: %x\r\n", config & NRF24L01_MASK_TX_DS_BM);
	printf("\tMASK_MAX_RT	: %x\r\n", config & NRF24L01_MASK_MAX_RT_BM);
	printf("\tEN_CRC		: %x\r\n", config & NRF24L01_EN_CRC_BM);
	printf("\tCRCO			: %x\r\n", config & NRF24L01_CRCO_BM);
	printf("\tPWR_UP		: %x\r\n", config & NRF24L01_PWR_UP_BM);
	printf("\tPRIM_RX		: %x\r\n", config & NRF24L01_PRIM_RX_BM);
	printf("EN_AA		(0x3F): %x\r\n", en_aa);
	printf("EN_RXADDR	(0x03): %x\r\n", en_rxaddr);
	printf("SETUP_AW	(0x03): %x\r\n", setup_aw);
	printf("SETUP_RETR	(0x03): %x\r\n", setup_retr);
	printf("RF_CH		(0x02): %x\r\n", rf_ch);
	printf("RF_SETUP	(0x0E): %x\r\n", rf_setup);
	printf("\tCONT_WAVE		: %x\r\n", rf_setup & NRF24L01_CONT_WAVE_BM);
	printf("\tRF_DR_LOW		: %x\r\n", rf_setup & NRF24L01_RF_DR_LOW_BM);
	printf("\tPLL_LOCK		: %x\r\n", rf_setup & NRF24L01_PLL_LOCK_BM);
	printf("\tRF_DR_HIGH	: %x\r\n", rf_setup & NRF24L01_RF_DR_HIGH_BM);
	printf("\tRF_PWR		: %x\r\n", rf_setup & NRF24L01_RF_PWR_BM);
	printf("STATUS		(0x0E): %x\r\n", status);
	printf("OBSERVE_TX	(0x00): %x\r\n", observe_tx);
	printf("RPD			(0x00): %x\r\n", rpd);
	printf("FIFO_STATUS	(0x11): %x\r\n", fifo_status);
	printf("\tTX_REUSE		: %x\r\n", fifo_status & NRF24L01_TX_REUSE_BM);
	printf("\tTX_FULL		: %x\r\n", fifo_status & NRF24L01_FIFO_STATUS_TX_FULL_BM);
	printf("\tTX_EMPTY		: %x\r\n", fifo_status & NRF24L01_TX_EMPTY_BM);
	printf("\tRX_FULL		: %x\r\n", fifo_status & NRF24L01_RX_FULL_BM);
	printf("\tRX_EMPTY		: %x\r\n", fifo_status & NRF24L01_RX_EMPTY_BM);
	printf("DYNPD		(0x00): %x\r\n", dynpd);
	printf("FEATURE		(0x00): %x\r\n", feature);
	//printf("RX_ADDR_P0	(0xE7E7E7E7E7): %llu\r\n", rx_addr_p0);
	//printf("RX_ADDR_P1	(0xC2C2C2C2C2): %llu\r\n", rx_addr_p1);
	//printf("TX_ADDR  	(0xE7E7E7E7E7): %llu\r\n", tx_addr);
	printf("RX_PW_P0  	(0x00): %x\r\n", rx_pw_p0);
	printf("RX_PW_P1  	(0x00): %x\r\n", rx_pw_p1);
}
*/