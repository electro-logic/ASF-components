// NRF24L01+ Single Chip 2.4GHz Transceiver

#ifndef NRF24L01_REGS_H_
#define NRF24L01_REGS_H_

// PTX = Primary TX
// PRX = Primary RX

// Commands
#define NRF24L01_R_REGISTER		0		// Read command and status registers
#define NRF24L01_W_REGISTER		0x20	// Write command and status registers
#define NRF24L01_R_RX_PAYLOAD	0x61	// Read RX-payload
#define NRF24L01_W_TX_PAYLOAD	0xA0	// Read RX-payload
#define NRF24L01_FLUSH_TX		0xE1	// Flush TX FIFO, used in TX mode
#define NRF24L01_FLUSH_RX		0xE2	// Flush RX FIFO, used in RX mode
#define NRF24L01_REUSE_TX_PL	0xE3	// Used for a PTX device. Reuse last transmitted payload.
#define NRF24L01_R_RX_PL_WID	0x60	// Read RX payload width for the top R_RX_PAYLOAD in the RX FIFO
#define NRF24L01_W_ACK_PAYLOAD	0xA8	// Used in RX mode. Write Payload to be transmitted together with ACK packet on PIPE PPP
#define NRF24L01_W_TX_PAYLOAD_NOACK	0xB0	// Used in TX mode. Disables AUTOACK on this specific packet
#define NRF24L01_NOP			0xFF	// No Operation. Might be used to read the STATUS register

// Register Map Table
#define NRF24L01_CONFIG_REG		0x00	// Configuration Register
#define NRF24L01_EN_AA_REG		0x01	// Enable ‘Auto Acknowledgment’ Function Disable, this functionality to be compatible with nRF2401
#define NRF24L01_EN_RXADDR_REG	0x02	// Enabled RX Addresses
#define NRF24L01_SETUP_AW_REG	0x03	// Setup of Address Widths
#define NRF24L01_SETUP_RETR_REG	0x04	// Setup of Automatic Retransmission
#define NRF24L01_RF_CH_REG		0x05	// RF Channel
#define NRF24L01_RF_SETUP_REG	0x06	// RF Setup Register
#define NRF24L01_STATUS_REG		0x07	// Status Register (In parallel to the SPI command word applied on the MOSI pin, the STATUS register is shifted serially out on the MISO pin)
#define NRF24L01_OBSERVE_TX_REG	0x08	// Transmit observe
#define NRF24L01_RPD_REG		0x09	// Received Power Detector
#define NRF24L01_RX_ADDR_P0_REG	0x0A	// Receive address data pipe 0 40 bit
#define NRF24L01_RX_ADDR_P1_REG	0x0B	// Receive address data pipe 1 8 bit
#define NRF24L01_RX_ADDR_P2_REG	0x0C	// Receive address data pipe 2
#define NRF24L01_RX_ADDR_P3_REG	0x0D	// Receive address data pipe 3
#define NRF24L01_RX_ADDR_P4_REG	0x0E	// Receive address data pipe 4
#define NRF24L01_RX_ADDR_P5_REG	0x0F	// Receive address data pipe 5
#define NRF24L01_TX_ADDR_REG	0x10	// Transmit address 40 bit
#define NRF24L01_RX_PW_P0_REG	0x11	// Number of bytes in RX payload in data pipe 0
#define NRF24L01_RX_PW_P1_REG	0x12	// Number of bytes in RX payload in data pipe 1
#define NRF24L01_RX_PW_P2_REG	0x13	// Number of bytes in RX payload in data pipe 2
#define NRF24L01_RX_PW_P3_REG	0x14	// Number of bytes in RX payload in data pipe 3
#define NRF24L01_RX_PW_P4_REG	0x15	// Number of bytes in RX payload in data pipe 4
#define NRF24L01_RX_PW_P5_REG	0x16	// Number of bytes in RX payload in data pipe 5
#define NRF24L01_FIFO_STATUS_REG	0x17	// FIFO Status Register
#define NRF24L01_DYNPD_REG		0x1C	// Enable dynamic payload length
#define NRF24L01_FEATURE_REG	0x1D	// Feature Register

// Configuration Register
#define NRF24L01_MASK_RX_DR_BM	(1 << 6)
#define NRF24L01_MASK_TX_DS_BM	(1 << 5)
#define NRF24L01_MASK_MAX_RT_BM	(1 << 4)
#define NRF24L01_EN_CRC_BM		(1 << 3)			// Default 1
#define NRF24L01_CRCO_BM		(1 << 2)
#define NRF24L01_PWR_UP_BM		(1 << 1)
#define NRF24L01_PRIM_RX_BM		1

// Status Register
#define NRF24L01_RX_DR_BM		(1 << 6)
#define NRF24L01_TX_DS_BM		(1 << 5)
#define NRF24L01_MAX_RT_BM		(1 << 4)
#define NRF24L01_RX_P_NO_BM		(1 << 1)|(1 << 2)|(1 << 3)	// // Default 111
#define NRF24L01_TX_FULL_BM		1

// RF Setup Register
#define NRF24L01_CONT_WAVE_BM	(1 << 7)
#define NRF24L01_RF_DR_LOW_BM	(1 << 5)
#define NRF24L01_PLL_LOCK_BM	(1 << 4)
#define NRF24L01_RF_DR_HIGH_BM	(1 << 3)			// Default 1
#define NRF24L01_RF_PWR_BM		(1 << 1)|(1 << 2)	// Default 11

// Transmit observe Register
#define NRF24L01_PLOS_CNT_BM	(1 << 4)|(1 << 5)|(1 << 6)|(1 << 7)	// Count lost packets
#define NRF24L01_ARC_CNT_BM		(1 << 1)|(1 << 2)|(1 << 3)			// Count retransmitted packets

// Received Power Detector Register
#define NRF24L01_RPD_BM			0

// TODO: Add other bit mask

#endif /* NRF24L01_REGS_H_ */