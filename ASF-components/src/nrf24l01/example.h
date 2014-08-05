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

void example_nrf24l01(void);

void example_nrf24l01(void)
{
	ioport_init();
	board_init();
	sysclk_init();
	
	/*
	// Enable USB
	irq_initialize_vectors();	// Enables all interrupt levels
	cpu_irq_enable();
	stdio_usb_init();
	*/
	
	nrf24l01_init();
	
	// Start receiver
	nrf24l01_primary_rx();
	nrf24l01_flush_rx();
	
	unsigned long nrfData = 0;
	
	loop:
	while(true)
	{
		nrfData++;
		nrf24l01_send_data((uint8_t*)&nrfData);
		// Check if sent or max retr, if not sending powerUpRx()
		while(nrf24l01_isSending()){ }
		delay_ms(10);
		while(!nrf24l01_dataReady()){
			//printf("Waiting..\r\n");
			delay_s(1);
			goto loop;
		}
		nrf24l01_receive_data((uint8_t*)&nrfData);
		//printf("Ping received: %lu\r\n", nrfData);
		delay_ms(200);
	}
}

#endif