// NRF24L01+ Single Chip 2.4GHz Transceiver

#ifndef NRF24L01_H_
#define NRF24L01_H_

#include <asf.h>
#include "nrf24l01_regs.h"
#include "conf_nrf24l01.h"

void nrf24l01_init(void);

void nrf24l01_set_data_rate(enum nrf24l01_data_rate dataRate);
void nrf24l01_set_crc(enum nrf24l01_crc crc);
void nrf24l01_set_power_amplifier(enum nrf24l01_power_amplifier pa);

void nrf24l01_write_tx_payload(uint32_t payload);
uint32_t nrf24l01_read_rx_payload(void);

void nrf24l01_flush_rx(void);
void nrf24l01_flush_tx(void);

void nrf24l01_write_register(uint8_t reg, uint8_t writeData);
uint8_t nrf24l01_read_register(uint8_t reg);
void nrf24l01_send_command(uint8_t command);

#endif /* NRF24L01P_H_ */