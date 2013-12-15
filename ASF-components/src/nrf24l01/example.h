// NRF24L01+ Single Chip 2.4GHz Transceiver
// http://electro-logic.blogspot.it
//
// Require ASF modules:
// SPI - Serial Peripheral Interface (Common API) (service)
// Delay routines (service)

#ifndef NRF24L01_EXAMPLE_H_
#define NRF24L01_EXAMPLE_H_

#include <asf.h>
#include "nrf24l01.h"

void example_nrf24l01(void)
{
	ioport_init();
	board_init();
	sysclk_init();
	
	nrf24l01_init();	
	
	uint8_t sreg = nrf24l01_read_register(NRF24L01_STATUS_REG);
	
	// TX Addr 0xE7 E7 E7 E7 E7

	// SETUP_AW to change address lenght
	
	// RX Addr	
	spi_select_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf);
	uint8_t data[6] = {NRF24L01_W_REGISTER | NRF24L01_RX_ADDR_P0_REG, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7 };
	spi_write_packet(&CONF_NRF24L01_SPI, data, 6);
	spi_deselect_device(&CONF_NRF24L01_SPI, &nrf24l01_spi_device_conf);		
	
	uint8_t rx_reg = nrf24l01_read_register(NRF24L01_RX_PW_P0_REG);
	
	while(true)
	{
		nrf24l01_write_tx_payload(123);
		nrf24l01_flush_tx();
		
		uint32_t received = nrf24l01_read_rx_payload();
		nrf24l01_flush_rx();
		
		uint8_t sreg = nrf24l01_read_register(NRF24L01_STATUS_REG);				
		
		delay_ms(100);
	}
}

#endif