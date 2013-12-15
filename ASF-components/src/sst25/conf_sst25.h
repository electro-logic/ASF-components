// SST25 flash memory config header 
 
#ifndef CONF_SST25_H_
#define CONF_SST25_H_

#include <asf.h>

// Select a SPI clock speed
#define CONF_SST25_CLOCK_SPEED  8000000UL

// Select the correct hardware interface
#define CONF_SST25_SPI			SPIC

// SPI pins
#define CONF_SST25_SS_PIN		IOPORT_CREATE_PIN(PORTC, 4) // Slave Select
#define CONF_SST25_MOSI_PIN		IOPORT_CREATE_PIN(PORTC, 5)
#define CONF_SST25_MISO_PIN		IOPORT_CREATE_PIN(PORTC, 6)
#define CONF_SST25_SCK_PIN		IOPORT_CREATE_PIN(PORTC, 7)

// SST25 pins
#define CONF_SST25_CEn_PIN		IOPORT_CREATE_PIN(PORTC, 2) // Chip Enable
#define CONF_SST25_WPn_PIN		IOPORT_CREATE_PIN(PORTD, 0) // Write Protect
#define CONF_SST25_HOLDn_PIN	IOPORT_CREATE_PIN(PORTD, 1)	// Hold - to temporarily stop communication without resetting the device

#endif /* CONF_SST25_H_ */