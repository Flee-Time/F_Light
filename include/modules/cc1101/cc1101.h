#pragma once

/*
 *
 * These codes were taken and adapted from the flipper zero cc1101 driver.
 *
*/

#ifndef __CC1101_H
#define __CC1101_H

#include "modules/cc1101/cc1101_regs.h"
#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

/** Strobe command to the device
 *
 * @param      handle  - pointer to the SPI_HandleTypeDef
 * @param      strobe  - command to execute
 *
 * @return     device status
 */
CC1101Status cc1101_strobe(SPI_HandleTypeDef handle, uint8_t strobe);

/** Write device register
 *
 * @param      handle  - pointer to the SPI_HandleTypeDef
 * @param      reg     - register
 * @param      data    - data to write
 *
 * @return     device status
 */
CC1101Status cc1101_write_reg(SPI_HandleTypeDef handle, uint8_t reg, uint8_t data);

/** Read device register
 *
 * @param      handle  - pointer to the SPI_HandleTypeDef
 * @param      reg     - register
 * @param[out] data    - pointer to data
 *
 * @return     device status
 */
CC1101Status cc1101_read_reg(SPI_HandleTypeDef handle, uint8_t reg, uint8_t* data);

/** Set Power Amplifier level table, ramp
 *
 * @param      handle  - pointer to the SPI_HandleTypeDef
 * @param      value   - array of power level values
 */
void cc1101_set_pa_table(SPI_HandleTypeDef handle, const uint8_t value[8]);

/** Write FIFO
 *
 * @param      handle  - pointer to the SPI_HandleTypeDef
 * @param      data    pointer to byte array
 * @param      size    write bytes count
 *
 * @return     size, written bytes count
 */
uint8_t cc1101_write_fifo(SPI_HandleTypeDef handle, const uint8_t* data, uint8_t size);

/** Read FIFO
 *
 * @param      handle  - pointer to the SPI_HandleTypeDef
 * @param      data    pointer to byte array
 * @param      size    bytes to read from fifo
 *
 * @return     size, read bytes count
 */
uint8_t cc1101_read_fifo(SPI_HandleTypeDef handle, uint8_t* data, uint8_t* size);

/* High level API */

/** Reset
 *
 * @param      handle  - pointer to the SPI_HandleTypeDef
 */
void cc1101_reset(SPI_HandleTypeDef handle);

/** Get status
 *
 * @param      handle  - pointer to the SPI_HandleTypeDef
 *
 * @return     CC1101Status structure
 */
CC1101Status cc1101_get_status(SPI_HandleTypeDef handle);

/** Enable shutdown mode
 *
 * @param      handle  - pointer to the SPI_HandleTypeDef
 */
void cc1101_shutdown(SPI_HandleTypeDef handle);

/** Get Partnumber
 *
 * @param      handle  - pointer to the SPI_HandleTypeDef
 *
 * @return     part number id
 */
uint8_t cc1101_get_partnumber(SPI_HandleTypeDef handle);

/** Get Version
 *
 * @param      handle  - pointer to the SPI_HandleTypeDef
 *
 * @return     version
 */
uint8_t cc1101_get_version(SPI_HandleTypeDef handle);

/** Get raw RSSI value
 *
 * @param      handle  - pointer to the SPI_HandleTypeDef
 *
 * @return     rssi value
 */
uint8_t cc1101_get_rssi(SPI_HandleTypeDef handle);

/** Calibrate oscillator
 *
 * @param      handle  - pointer to the SPI_HandleTypeDef
 */
void cc1101_calibrate(SPI_HandleTypeDef handle);

/** Switch to idle
 *
 * @param      handle  - pointer to the SPI_HandleTypeDef
 */
void cc1101_switch_to_idle(SPI_HandleTypeDef handle);

/** Switch to RX
 *
 * @param      handle  - pointer to the SPI_HandleTypeDef
 */
void cc1101_switch_to_rx(SPI_HandleTypeDef handle);

/** Switch to TX
 *
 * @param      handle  - pointer to the SPI_HandleTypeDef
 */
void cc1101_switch_to_tx(SPI_HandleTypeDef handle);

/** Flush RX FIFO
 *
 * @param      handle  - pointer to the SPI_HandleTypeDef
 */
void cc1101_flush_rx(SPI_HandleTypeDef handle);

/** Flush TX FIFO
 *
 * @param      handle  - pointer to the SPI_HandleTypeDef
 */
void cc1101_flush_tx(SPI_HandleTypeDef handle);

/** Set Frequency
 *
 * @param      handle  - pointer to the SPI_HandleTypeDef
 * @param      value   - frequency in herz
 *
 * @return     real frequency that were synthesized
 */
uint32_t cc1101_set_frequency(SPI_HandleTypeDef handle, uint32_t value);

/** Set Intermediate Frequency
 *
 * @param      handle  - pointer to the SPI_HandleTypeDef
 * @param      value   - frequency in herz
 *
 * @return     real inermediate frequency that were synthesized
 */
uint32_t cc1101_set_intermediate_frequency(SPI_HandleTypeDef handle, uint32_t value);

#endif