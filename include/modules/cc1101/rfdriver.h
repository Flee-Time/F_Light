#ifndef __RFDRIVER_H
#define __RFDRIVER_H

#include "modules/cc1101/cc1101.h"

void rf_init(SPI_HandleTypeDef rf_spi);
float rf_get_rssi(SPI_HandleTypeDef rf_spi);
uint8_t rf_get_lqi(SPI_HandleTypeDef rf_spi);
uint32_t rf_set_frequency(SPI_HandleTypeDef rf_spi, uint32_t value);
bool rf_is_frequency_valid(uint32_t value);

#endif