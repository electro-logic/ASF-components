// ASF-components
// http://electro-logic.blogspot.it
// Library of reusable components for ASF
//
// Components:
// ili9320	lcd controller		(tested with HY28A display)
// sst25	serial flash memory (tested with SST25VF080B memory)
// ad9834	dds
//
// Notes:
// Components are written in "ASF style" and tested on XMega (ATxmega32A4U) mcu.
// To reuse component copy component folder to project and configure config file (ex. conf_sst25.h)
// In component main header file (ex. sst25.h) there are methods prototype to call to use component

#include "ili9320/example.h"
#include "sst25/example.h"
#include "ad9834/example.h"

int main(void)
{	
	// Uncomment example to try
	example_ad9834();
	// example_sst25();
	// example_ili9320();
}