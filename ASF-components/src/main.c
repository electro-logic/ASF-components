// ASF-components
// http://electro-logic.blogspot.it
// Library of reusable components for ASF
//
// Components:
// ili9320	lcd controller		(tested with HY28A display)
// sst25	serial flash memory (tested with SST25VF080B memory)
//
// Notes:
// Components are written in "ASF style" and tested on XMega (ATxmega32A4U) mcu.
// To reuse component copy component folder to project and configure config file

#include "ili9320/example.h"
#include "sst25/example.h"

int main(void)
{	
	// Uncomment example to try
	example_sst25();
	// example_ili9320();
}