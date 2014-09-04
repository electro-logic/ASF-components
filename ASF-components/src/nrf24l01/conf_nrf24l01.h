// NRF24L01+ Single Chip 2.4GHz Transceiver

#ifndef CONF_NRF24L01_H_
#define CONF_NRF24L01_H_

#include <asf.h>

// Select a SPI clock speed (Maximum data rate 10MHz)
#define CONF_NRF24L01_CLOCK_SPEED  8000000UL

// Select the correct hardware interface
#define CONF_NRF24L01_SPI			SPIC

// SPI pins
#define CONF_NRF24L01_SS_PIN		IOPORT_CREATE_PIN(PORTC, 4) // Slave Select
#define CONF_NRF24L01_MOSI_PIN		IOPORT_CREATE_PIN(PORTC, 5)
#define CONF_NRF24L01_MISO_PIN		IOPORT_CREATE_PIN(PORTC, 6)
#define CONF_NRF24L01_SCK_PIN		IOPORT_CREATE_PIN(PORTC, 7)

// NRF24L01 pins
#define CONF_NRF24L01_CE_PIN		IOPORT_CREATE_PIN(PORTC, 4) // Chip Enable, activates RX (=1) or TX (=0) mode. Active high
#define CONF_NRF24L01_CSn_PIN		IOPORT_CREATE_PIN(PORTC, 3) // SPI Chip Select
#define CONF_NRF24L01_IRQ_PIN		IOPORT_CREATE_PIN(PORTC, 2)	// Maskable interrupt pin. Active low

enum nrf24l01_data_rate
{
	NRF24L01_250kbps,
	NRF24L01_1Mbps,
	NRF24L01_2Mbps
};

// NRF24L01 data rate
#define CONF_NRF24L01_DATA_RATE		NRF24L01_2Mbps

enum nrf24l01_crc
{
	NRF24L01_CRC_DISABLED,	// No CRC
	NRF24L01_CRC_1B,		// 1 Byte CRC
	NRF24L01_CRC_2B			// 2 Byte CRC
};

// NRF24L01 crc mode
#define CONF_NRF24L01_CRC			NRF24L01_CRC_2B

enum nrf24l01_power_amplifier
{
	NRF24L01_PA_0dBm,		// 11.3	mA
	NRF24L01_PA_less_6dBm,	// 9.0	mA
	NRF24L01_PA_less_12dBm,	// 7.5	mA
	NRF24L01_PA_less_18dBm	// 7.0	mA
};

// NRF24L01 Power Amplifier
#define CONF_NRF24L01_PA			NRF24L01_PA_0dBm

enum nrf24l01_addrlen
{
	NRF24L01_ADDR_LEN_3,
	NRF24L01_ADDR_LEN_4,
	NRF24L01_ADDR_LEN_5
};

#define CONF_NRF24L01_ADDR_LEN		NRF24L01_ADDR_LEN_3
#define CONF_NRF24L01_RX_ADDR		0xA6, 0xA7, 0xA6
#define CONF_NRF24L01_TX_ADDR		0xEC, 0xED, 0xEC
 
// NRF24L01 Payload lenght in bytes (0-32)
#define CONF_NRF24L01_PAYLOAD		32

// NRF24L01 RF channel
// Allowed channels in Italy: 0-83
#define CONF_NRF24L01_RF_CHANNEL	4

#endif /* CONF_NRF24L01_H_ */