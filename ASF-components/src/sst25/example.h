// SST25VF080B SPI Memory communication
// http://electro-logic.blogspot.it
//
// Require ASF modules:
// SPI - Serial Peripheral Interface (Common API) (service)
// Delay routines (service)

#ifndef SST25_EXAMPLE_H_
#define SST25_EXAMPLE_H_

#include <asf.h>
#include "SST25.h"

#define TEST_SIZE	1024

void example_sst25(void)
{
	ioport_init();
	board_init();
	sysclk_init();
	
	// Prepare test data
	uint8_t dataWrite[TEST_SIZE];
	uint8_t dataRead[TEST_SIZE];
	for (int i=0;i<TEST_SIZE;i++)
	{
		dataWrite[i]=i;
	}
	
	// Start writing / reading from address 0
	uint32_t address = 0x00;
	
	// Init component
	sst25_init();
	
	// Erase all memory
	sst25_chip_erase();
		
	// Write test data
	sst25_write(address, dataWrite, TEST_SIZE);
		
	// Read test data
	sst25_read(address, dataRead, TEST_SIZE);
	
	while(true)
	{		
		// Do nothing
		delay_ms(100);
	}
}

#endif