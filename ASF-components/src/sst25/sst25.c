// SST25 flash memory

#ifndef sst25_C_
#define sst25_C_

#include "sst25.h"

// SPI select pin
struct spi_device sst25_spi_device_conf = { .id = CONF_SST25_CEn_PIN };

// Initialize Pin and SPI like specified into config file, then disable Block Protection
void sst25_init()
{
	ioport_set_pin_dir(CONF_SST25_SS_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(CONF_SST25_SS_PIN, IOPORT_MODE_PULLUP);
	
	ioport_set_pin_dir(CONF_SST25_MOSI_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_mode(CONF_SST25_MOSI_PIN, IOPORT_MODE_PULLUP);
	ioport_set_pin_high(CONF_SST25_MOSI_PIN);
	
	ioport_set_pin_dir(CONF_SST25_MISO_PIN, IOPORT_DIR_INPUT);
	
	ioport_set_pin_dir(CONF_SST25_SCK_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_high(CONF_SST25_SCK_PIN);
	
	ioport_set_pin_dir(CONF_SST25_CEn_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_high(CONF_SST25_CEn_PIN);
	
	ioport_set_pin_dir(CONF_SST25_WPn_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_high(CONF_SST25_WPn_PIN);
	
	ioport_set_pin_dir(CONF_SST25_HOLDn_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_high(CONF_SST25_HOLDn_PIN);
	
	spi_master_init(&CONF_SST25_SPI);
		
	spi_master_setup_device(&CONF_SST25_SPI, &sst25_spi_device_conf, SPI_MODE_0, CONF_SST25_CLOCK_SPEED, 0);
	spi_enable(&CONF_SST25_SPI);
	
	// Wait Memory is ready (wait recommended System Power-up Timings)
	delay_us(10);
	
	// Disable Block Protection
	sst25_write_status_enable();
	sst25_write_status_register(0);
}
	
uint8_t sst25_read_byte(uint32_t address)
{
	uint8_t readData;
	sst25_read(address, &readData, 1);
	return readData;
}

// JDEC ID: Manufacturer, Memory Type, Memory Capacity
// SST, SPI Serial Flash, SST25VF080B (8MEG)
// 0xBF, 0x25, 0x8E
uint32_t sst25_read_jedec_id()
{
	spi_select_device(&CONF_SST25_SPI, &sst25_spi_device_conf);	
	uint8_t data[1] = { SST25_JEDEC_ID };
	spi_write_packet(&CONF_SST25_SPI, data, 1);	
	uint8_t readData[3];
	spi_read_packet(&CONF_SST25_SPI, readData, 3);
	spi_deselect_device(&CONF_SST25_SPI, &sst25_spi_device_conf);		
	return ((uint32_t)readData[0] << 16) | (readData[1] << 8) | readData[2];
}

// 0xBF 0x8E 0xBF 0x8E
uint32_t sst25_read_id()
{
	spi_select_device(&CONF_SST25_SPI, &sst25_spi_device_conf);
	uint8_t data[4] = { SST25_RDID, 0x00, 0x00, 0x00 };
	spi_write_packet(&CONF_SST25_SPI, data, 4);
	uint8_t readData[4];
	spi_read_packet(&CONF_SST25_SPI, readData, 4);
	spi_deselect_device(&CONF_SST25_SPI, &sst25_spi_device_conf);
	return ((uint32_t)readData[0] << 24) | ((uint32_t)readData[0] << 16) | (readData[1] << 8) | readData[2];	
}

// Read Memory (25/33 MHz)
// Notes: Once the highest memory address is reached, the address pointer will automatically
// increment to the beginning (wrap-around) of the address space
void sst25_read(uint32_t address, uint8_t *readData, uint32_t count)
{
	spi_select_device(&CONF_SST25_SPI, &sst25_spi_device_conf);
	
	// Send Read Command (8 bit)  and Address (24 bit, MSB first)
	uint8_t data[4] = {SST25_READ, address >> 16, ((address >> 8) & 0xFF), (address & 0xFF)};
	spi_write_packet(&CONF_SST25_SPI, data, 4);
	
	// Read Memory
	spi_read_packet(&CONF_SST25_SPI, readData, count);		
	
	spi_deselect_device(&CONF_SST25_SPI, &sst25_spi_device_conf);
}

void sst25_write_byte(uint32_t address, uint8_t writeData)
{	
	sst25_write_enable();
	
	spi_select_device(&CONF_SST25_SPI, &sst25_spi_device_conf);
	
	// Send Write Command and Address
	uint8_t data[4] = { SST25_BYTE_PROGRAM, address >> 16, (address >> 8) & 0xFF, address & 0xFF};
	spi_write_packet(&CONF_SST25_SPI, data, 4);
	spi_write_packet(&CONF_SST25_SPI, &writeData, 1);	// Write Memory
	
	spi_deselect_device(&CONF_SST25_SPI, &sst25_spi_device_conf);
	
	delay_us(10);
}

// Note: The selected address range must be in the erased state (FFH) when initiating an AAI Word Program operation
void sst25_write(uint32_t address, uint8_t *writeData, uint32_t count)
{
	sst25_write_enable();	
	spi_select_device(&CONF_SST25_SPI, &sst25_spi_device_conf);
	uint8_t data[4] = { SST25_AAI_WORD_PROGRAM, address >> 16, (address >> 8) & 0xFF, address & 0xFF};
	spi_write_packet(&CONF_SST25_SPI, data, 4);					
	uint8_t aaiOp = SST25_AAI_WORD_PROGRAM;
	// Send AAI and 2 bytes of data, wait, AAI and others 2 bytes of data, wait, and so on..
	for (int i=0;i<count;i++)
	{		
		if((i!=0)&&((i % 2)==0))
		{
			spi_deselect_device(&CONF_SST25_SPI, &sst25_spi_device_conf);
			// Wait Tbp
			delay_us(10);
			spi_select_device(&CONF_SST25_SPI, &sst25_spi_device_conf);
			// Send AAI instruction
			spi_write_packet(&CONF_SST25_SPI, &aaiOp, 1);
		}		
		spi_write_packet(&CONF_SST25_SPI, writeData++, 1);
	}			
	spi_deselect_device(&CONF_SST25_SPI, &sst25_spi_device_conf);
	// Wait Tbp
	delay_us(10);
	spi_select_device(&CONF_SST25_SPI, &sst25_spi_device_conf);
	// Exit AAI mode
	uint8_t wrdiOp = SST25_WRDI;
	spi_write_packet(&CONF_SST25_SPI, &wrdiOp, 1);			
	spi_deselect_device(&CONF_SST25_SPI, &sst25_spi_device_conf);
	delay_us(10); // ??
	spi_select_device(&CONF_SST25_SPI, &sst25_spi_device_conf);		
	uint8_t rdsrOp = SST25_RDSR;
	spi_write_packet(&CONF_SST25_SPI, &rdsrOp, 1);
	uint8_t sreg;
	spi_read_packet(&CONF_SST25_SPI, &sreg, 1);	
	spi_deselect_device(&CONF_SST25_SPI, &sst25_spi_device_conf);
}

// Clears (set to 0xFF) all bits in the selected 4 KBytes sector
// Sector to clear is identified by A23-A12 bit in address
void sst25_sector_erase(uint32_t address)
{
	sst25_write_enable();	
	spi_select_device(&CONF_SST25_SPI, &sst25_spi_device_conf);			
	// Send Erase Memory Command and Address
	uint8_t data[4] = { SST25_SECTOR_ERASE, address >> 16, (address >> 8) & 0xFF, address & 0xFF};
	spi_write_packet(&CONF_SST25_SPI, data, 4);	
	spi_deselect_device(&CONF_SST25_SPI, &sst25_spi_device_conf);	
	// Wait Tse
	delay_ms(25);
}

// Clears (set to 0xFF) all bits in the selected 32 KBytes block
// Sector to clear is identified by A23-A15 bit in address
void sst25_32k_block_erase(uint32_t address)
{
	sst25_write_enable();	
	spi_select_device(&CONF_SST25_SPI, &sst25_spi_device_conf);	
	// Send Erase Memory Command and Address
	uint8_t data[4] = { SST25_32K_BLOCK_ERASE, address >> 16, (address >> 8) & 0xFF, address & 0xFF};
	spi_write_packet(&CONF_SST25_SPI, data, 4);	
	spi_deselect_device(&CONF_SST25_SPI, &sst25_spi_device_conf);	
	// Wait Tbe
	delay_ms(25);
}

// Clears (set to 0xFF) all bits in the selected 64 KBytes block
// Sector to clear is identified by A23-A16 bit in address
void sst25_64k_block_erase(uint32_t address)
{
	sst25_write_enable();	
	spi_select_device(&CONF_SST25_SPI, &sst25_spi_device_conf);	
	// Send Erase Memory Command and Address
	uint8_t data[4] = { SST25_64K_BLOCK_ERASE, address >> 16, (address >> 8) & 0xFF, address & 0xFF};
	spi_write_packet(&CONF_SST25_SPI, data, 4);	
	spi_deselect_device(&CONF_SST25_SPI, &sst25_spi_device_conf);	
	// Wait Tbe
	delay_ms(25);
}

// Clears (set to 0xFF) all bits in the selected 64 KBytes block
// Sector to clear is identified by A23-A16 bit in address
void sst25_chip_erase()
{
	sst25_write_enable();	
	sst25_send_command(SST25_CHIP_ERASE);	
	// Wait TSce
	delay_ms(50);
}

uint8_t sst25_read_status_register()
{	
	spi_select_device(&CONF_SST25_SPI, &sst25_spi_device_conf);
	uint8_t data[1] = { SST25_RDSR };
	spi_write_packet(&CONF_SST25_SPI, data, 1);
	spi_read_packet(&CONF_SST25_SPI, data, 1);
	spi_deselect_device(&CONF_SST25_SPI, &sst25_spi_device_conf);
	return data[0];
}

void sst25_write_status_register(uint8_t sreg)
{
	spi_select_device(&CONF_SST25_SPI, &sst25_spi_device_conf);
	uint8_t data[2] = { SST25_WRSR, sreg };
	spi_write_packet(&CONF_SST25_SPI, data, 2);	
	spi_deselect_device(&CONF_SST25_SPI, &sst25_spi_device_conf);	
}

void sst25_write_status_enable()
{
	sst25_send_command(SST25_EWSR);	
}

void sst25_write_enable()
{
	sst25_send_command(SST25_WREN);	
}

void sst25_write_disable()
{
	sst25_send_command(SST25_WRDI);
}

void sst25_send_command(uint8_t command)
{
	spi_select_device(&CONF_SST25_SPI, &sst25_spi_device_conf);
	uint8_t data[1] = { command };
	spi_write_packet(&CONF_SST25_SPI, data, 1);
	spi_deselect_device(&CONF_SST25_SPI, &sst25_spi_device_conf);
}

//void sst25_debug()
//{
	//uint32_t id = sst25_read_id();
	//uint32_t jdec_id = sst25_read_jedec_id();
	//uint8_t sreg = sst25_read_status_register();
	//bool busy = sreg & SST25_SREG_BUSY_BM;
	//bool wel = sreg & SST25_SREG_WEL_BM;
	//bool bp0 = sreg & SST25_SREG_BP0_BM;	// default 1
	//bool bp1 = sreg & SST25_SREG_BP1_BM;	// default 1
	//bool bp2 = sreg & SST25_SREG_BP2_BM;	// default 1
	//bool bp3 = sreg & SST25_SREG_BP3_BM;
	//bool aai = sreg & SST25_SREG_AAI_BM;
	//bool bpl = sreg & SST25_SREG_BPL_BM;
//}

#endif /* sst25_H_ */