// ILI9320 display controller component driver default config header 

#ifndef CONF_ILI9320_H_INCLUDED
#define CONF_ILI9320_H_INCLUDED

#include <compiler.h>

// Select a SPI clock speed
#define CONF_ILI9320_CLOCK_SPEED   8000000UL

// Select the correct hardware interface
#define CONF_ILI9320_SPI           &SPIC

// Define what MCU pin the ILI9320 chip select pin is connected to
#define CONF_ILI9320_CS_PIN        IOPORT_CREATE_PIN(PORTC, 2)

// Define what MCU pin the ILI9320 back light pin is connected to (NOT USED)
#define CONF_ILI9320_BACKLIGHT_PIN IOPORT_CREATE_PIN(PORTC, 0)

// Define what MCU pin the ILI9320 reset is connected to
#define CONF_ILI9320_RESET_PIN     IOPORT_CREATE_PIN(PORTC, 1)

// SPI pins
#define CONF_ILI9320_SS_PIN			IOPORT_CREATE_PIN(PORTC, 4)
#define CONF_ILI9320_MOSI_PIN		IOPORT_CREATE_PIN(PORTC, 5)
#define CONF_ILI9320_MISO_PIN		IOPORT_CREATE_PIN(PORTC, 6)
#define CONF_ILI9320_SCK_PIN		IOPORT_CREATE_PIN(PORTC, 7)

#endif /* CONF_ILI9320_H_INCLUDED */
