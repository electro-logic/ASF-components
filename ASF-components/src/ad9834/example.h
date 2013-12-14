// AD9834 DDS
// http://electro-logic.blogspot.it
//
// Require ASF modules:
// SPI - Serial Peripheral Interface (Common API) (service)
// DAC - Digital to Analog (driver)
// Delay routines (service)

#ifndef AD9834_EXAMPLE_H_
#define AD9834_EXAMPLE_H_

#include <asf.h>
#include "ad9834.h"

void example_ad9834(void)
{
	ioport_init();
	board_init();
	sysclk_init();
	
	// Init component
	ad9834_init();
	
	// Set Waveform, Output voltage and Frequency
	ad9834_set_waveform(AD9834_SINE);	
	ad9834_set_output_voltage(CONF_AD9834_MAX_VOUT);
	ad9834_set_frequency(1000.0);	
		
	while(true)
	{
		// Do nothing
		delay_ms(100);
	}
}

#endif