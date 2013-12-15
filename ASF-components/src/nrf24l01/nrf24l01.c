// NRF24L01+ Single Chip 2.4GHz Transceiver

#include "nrf24l01.h"

// SPI select pin
struct spi_device nrf24l01_spi_device_conf = { .id = CONF_NRF24L01_CSn_PIN };

// Initialize Pin and SPI like specified into config file, then disable Block Protection
void nrf24l01_init()
{
	ioport_set_pin_dir(CONF_NRF24L01_SS_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(CONF_NRF24L01_SS_PIN, IOPORT_MODE_PULLUP);
	
	ioport_set_pin_dir(CONF_NRF24L01_MOSI_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_mode(CONF_NRF24L01_MOSI_PIN, IOPORT_MODE_PULLUP);
	ioport_set_pin_high(CONF_NRF24L01_MOSI_PIN);
	
	ioport_set_pin_dir(CONF_NRF24L01_MISO_PIN, IOPORT_DIR_INPUT);
	
	ioport_set_pin_dir(CONF_NRF24L01_SCK_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_high(CONF_NRF24L01_SCK_PIN);
	
	ioport_set_pin_dir(CONF_NRF24L01_CE_PIN, IOPORT_DIR_OUTPUT);			
	ioport_set_pin_dir(CONF_NRF24L01_CSn_PIN, IOPORT_DIR_OUTPUT);	
	ioport_set_pin_dir(CONF_NRF24L01_IRQ_PIN, IOPORT_DIR_INPUT);
	
	spi_master_init(&CONF_NRF24L01_SPI);
	
	spi_master_setup_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf, SPI_MODE_0, CONF_NRF24L01_CLOCK_SPEED, 0);
	spi_enable(&CONF_NRF24L01_SPI);
	
	// Power on reset
	delay_ms(100);
	
	nrf24l01_set_data_rate(CONF_NRF24L01_DATA_RATE);
	nrf24l01_set_crc(CONF_NRF24L01_CRC);
	nrf24l01_set_power_amplifier(CONF_NRF24L01_PA);
	
	// TX Mode
	uint8_t config_reg = nrf24l01_read(NRF24L01_CONFIG_REG);
	config_reg |= NRF24L01_PWR_UP_BM & (~NRF24L01_PRIM_RX_BM);	
	nrf24l01_write(NRF24L01_CONFIG_REG, config_reg);	
	ioport_set_pin_high(CONF_NRF24L01_CE_PIN);
	delay_us(10);
	ioport_set_pin_low(CONF_NRF24L01_CE_PIN);
	
	// RX Mode
	config_reg = nrf24l01_read(NRF24L01_CONFIG_REG);
	config_reg |= NRF24L01_PWR_UP_BM | NRF24L01_PRIM_RX_BM;		
	ioport_set_pin_high(CONF_NRF24L01_CE_PIN);	
}

void nrf24l01_set_crc(enum nrf24l01_crc crc)
{
	uint8_t config_reg = nrf24l01_read(NRF24L01_CONFIG_REG);
	switch(crc)
	{
		case NRF24L01_CRC_DISABLED:
			config_reg &= ~NRF24L01_EN_CRC_BM;
			break;
		case NRF24L01_CRC_1B:
			config_reg |= NRF24L01_EN_CRC_BM & (~NRF24L01_CRCO_BM);
			break;
		case NRF24L01_CRC_2B:
			config_reg |= NRF24L01_EN_CRC_BM | NRF24L01_CRCO_BM;
			break;
	}	
	nrf24l01_write(NRF24L01_CONFIG_REG, config_reg);
};

void nrf24l01_set_data_rate(enum nrf24l01_data_rate dataRate)
{
	uint8_t rf_setup_reg = nrf24l01_read(NRF24L01_RF_SETUP_REG);		
	switch (dataRate)
	{
		case NRF24L01_250kbps:
			rf_setup_reg |= NRF24L01_RF_DR_LOW_BM & (~NRF24L01_RF_DR_HIGH_BM);			
			break;
		case NRF24L01_1Mbps:
			rf_setup_reg &= (~NRF24L01_RF_DR_HIGH_BM) & (~NRF24L01_RF_DR_HIGH_BM);		
			break;
		case NRF24L01_2Mbps:				
			rf_setup_reg |= NRF24L01_RF_DR_HIGH_BM & (~NRF24L01_RF_DR_LOW_BM);			
			break;
	}	
	nrf24l01_write(NRF24L01_RF_SETUP_REG, rf_setup_reg);
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
	uint8_t rf_setup_reg = nrf24l01_read(NRF24L01_RF_SETUP_REG);
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
	nrf24l01_write(NRF24L01_RF_SETUP_REG, rf_setup_reg);	
};


void nrf24l01_write_tx_payload(uint32_t payload)
{		
	spi_select_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf);
	uint8_t data[5] = {NRF24L01_W_TX_PAYLOAD, payload >> 24, (payload >> 16) & 0xFF, (payload >> 8) & 0xFF, (payload & 0xFF)};						
	spi_write_packet(&CONF_NRF24L01_SPI, data, 5);
	spi_deselect_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf);
}

uint32_t nrf24l01_read_rx_payload()
{
	spi_select_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf);
	uint8_t data[1] = {NRF24L01_R_RX_PAYLOAD};
	spi_write_packet(&CONF_NRF24L01_SPI, data, 1);
	uint8_t readData[4];
	spi_read_packet(&CONF_NRF24L01_SPI, readData, 4);
	spi_deselect_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf);	
	return ((uint32_t)readData[0] << 24) | ((uint32_t)readData[0] << 16) | (readData[1] << 8) | readData[2];
}

uint8_t nrf24l01_read_register(uint8_t reg)
{		
	spi_select_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf);	
	uint8_t data = NRF24L01_R_REGISTER | reg;
	spi_write_packet(&CONF_NRF24L01_SPI, &data, 1);			
	uint8_t readData;
	spi_read_packet(&CONF_NRF24L01_SPI, &readData, 1);	
	spi_deselect_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf);
	return readData;
}

void nrf24l01_write_register(uint8_t reg, uint8_t writeData)
{
	spi_select_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf);
	uint8_t data[2] = {NRF24L01_W_REGISTER | reg, writeData };
	spi_write_packet(&CONF_NRF24L01_SPI, data, 2);	
	spi_deselect_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf);	
}