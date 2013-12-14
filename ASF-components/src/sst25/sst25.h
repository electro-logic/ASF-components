// SST25 flash memory

#ifndef SST25_H_
#define SST25_H_

#include "conf_sst25.h"

// Commands
#define SST25_READ				0x03	// Read Memory
#define SST25_HS_READ			0x0B	// Read Memory at higher speed
#define SST25_SECTOR_ERASE		0x20	// Erase 4 KByte of memory array
#define SST25_32K_BLOCK_ERASE	0x52	// Erase 32 KByte of memory array
#define SST25_64K_BLOCK_ERASE	0xD8	// Erase 64 KByte of memory array
#define SST25_CHIP_ERASE		0x60	// Erase Full Memory Array
#define SST25_SECTOR_ERASE		0x20	// Erase 4 KByte of memory array
#define SST25_BYTE_PROGRAM		0x02	// Program One Data Byte
#define SST25_AAI_WORD_PROGRAM	0xAD	// Auto Address Increment Programming
#define SST25_RDSR				0x05	// Read-Status-Register
#define SST25_EWSR				0x50	// Enable-Write-Status-Register
#define SST25_WRSR				0x01	// Write-Status-Register
#define SST25_WREN				0x06	// Write-Enable
#define SST25_WRDI				0x04	// Write-Disable
#define SST25_RDID				0x90	// Read-ID
#define SST25_JEDEC_ID			0x9F	// JEDEC ID read
#define SST25_EBSY				0x70	// Enable SO to output RY/BY# status during AAI programming
#define SST25_DBSY				0x80	// Disable SO as RY/BY# status during AAI programming

// Status Register
#define SST25_SREG_BUSY_BM		0		// Internal Write operation is in progress
#define SST25_SREG_WEL_BM		1 << 1	// Memory Write enabled
#define SST25_SREG_BP0_BM		1 << 2
#define SST25_SREG_BP1_BM		1 << 3
#define SST25_SREG_BP2_BM		1 << 4
#define SST25_SREG_BP3_BM		1 << 5
#define SST25_SREG_AAI_BM		1 << 6	// Auto Address Increment Programming status
#define SST25_SREG_BPL_BM		1 << 7	// Block-Protection bits

// Internals
void sst25_send_command(uint8_t command);

// Initialization and Identification
void sst25_init(void);
uint32_t sst25_read_jedec_id(void);
uint32_t sst25_read_id(void);

// Register
void sst25_write_status_enable(void);
void sst25_write_status_register(uint8_t sreg);
uint8_t sst25_read_status_register(void);

// Erase
void sst25_sector_erase(uint32_t address);
void sst25_32k_block_erase(uint32_t address);
void sst25_64k_block_erase(uint32_t address);
void sst25_chip_erase();

// Read
uint8_t sst25_read_byte(uint32_t address);
void sst25_read(uint32_t address, uint8_t *readData, uint32_t count);

// Write
void sst25_write_enable(void);
void sst25_write_disable(void);
void sst25_write_byte(uint32_t address, uint8_t writeData);
void sst25_write(uint32_t address, uint8_t *writeData, uint32_t count);
	
#endif /* SST25_H_ */