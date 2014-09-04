// NRF24L01+ Single Chip 2.4GHz Transceiver

#ifndef NRF24L01_H_
#define NRF24L01_H_

#include <asf.h>
#include "nrf24l01_regs.h"
#include "conf_nrf24l01.h"
#include "utils/utils.h"

void nrf24l01_init(void);

void nrf24l01_primary_rx(void);
void nrf24l01_primary_tx(void);
void nrf24l01_power_off(void);

void nrf24l01_send_data(uint8_t* value);
void nrf24l01_receive_data(uint8_t* data);

uint8_t nrf24l01_read_byte(void);
void nrf24l01_write_byte(uint8_t nrfData);

void nrf24l01_set_data_rate(enum nrf24l01_data_rate dataRate);
void nrf24l01_set_crc(enum nrf24l01_crc crc);
void nrf24l01_set_power_amplifier(enum nrf24l01_power_amplifier pa);
void nrf24l01_set_rf_channel(uint8_t channel);
void nrf24l01_set_rx_addr(uint8_t* addr);
void nrf24l01_set_tx_addr(uint8_t* addr);

void nrf24l01_set_addr_len(enum nrf24l01_addrlen addr_len);

void nrf24l01_flush_rx(void);
void nrf24l01_flush_tx(void);

uint8_t nrf24l01_read_register(uint8_t reg);
void nrf24l01_write_register(uint8_t reg, uint8_t writeData);

uint8_t* nrf24l01_read_array_register(uint8_t reg, uint8_t* readData, uint8_t dataLen);
void nrf24l01_write_array_register(uint8_t reg, uint8_t* writeData, uint8_t dataLen);

void nrf24l01_send_command(uint8_t command);

// TODO: Write Interrupt-driven code
bool nrf24l01_dataReady(void);
bool nrf24l01_isSending(void);

#endif /* NRF24L01P_H_ */