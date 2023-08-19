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

// Deprecated -- See cc1101.h for explanation

/*
void GD0_Input();
void GD0_Output();
void GD0_Analog();
*/

/** Strobe command to the device
 *
 * @param      strobe  - command to execute
 *
 * @return     device status
 */
CC1101Status cc1101_strobe(uint8_t strobe);

/** Write device register
 *
 * @param      reg     - register
 * @param      data    - data to write
 *
 * @return     device status
 */
CC1101Status cc1101_write_reg(uint8_t reg, uint8_t data);

/** Read device register
 *
 * @param      reg     - register
 * @param[out] data    - pointer to data
 *
 * @return     device status
 */
CC1101Status cc1101_read_reg(uint8_t reg, uint8_t* data);

/** Set Power Amplifier level table, ramp
 *
 * @param      value   - array of power level values
 */
void cc1101_set_pa_table(const uint8_t value[8]);

/** Write FIFO
 *
 * @param      data    pointer to byte array
 * @param      size    write bytes count
 *
 * @return     size, written bytes count
 */
uint8_t cc1101_write_fifo(const uint8_t* data, uint8_t size);

/** Read FIFO
 *
 * @param      data    pointer to byte array
 * @param      size    bytes to read from fifo
 *
 * @return     size, read bytes count
 */
uint8_t cc1101_read_fifo(uint8_t* data, uint8_t* size);

/* High level API */

/** Reset
 */
void cc1101_reset();

/** Get status
 *
 * @return     CC1101Status structure
 */
CC1101Status cc1101_get_status();

/** Enable shutdown mode
 */
void cc1101_shutdown();

/** Get Partnumber
 *
 * @return     part number id
 */
uint8_t cc1101_get_partnumber();

/** Get Version
 *
 * @return     version
 */
uint8_t cc1101_get_version();

/** Get raw RSSI value
 *
 * @return     rssi value
 */
uint8_t cc1101_get_rssi();

/** Calibrate oscillator
 */
void cc1101_calibrate();

/** Switch to idle
 */
void cc1101_switch_to_idle();

/** Switch to RX
 */
void cc1101_switch_to_rx();

/** Switch to TX
 */
void cc1101_switch_to_tx();

/** Flush RX FIFO
 */
void cc1101_flush_rx();

/** Flush TX FIFO
 */
void cc1101_flush_tx();

/** Set Frequency
 *
 * @param      value   - frequency in hertz
 *
 * @return     real frequency that were synthesized
 */
uint32_t cc1101_set_frequency(uint32_t value);

/** Set Intermediate Frequency
 *
 * @param      value   - frequency in hertz
 *
 * @return     real inermediate frequency that were synthesized
 */
uint32_t cc1101_set_intermediate_frequency(uint32_t value);

/** Wakes up the cc1101 from sleep
 */
void cc1101_wake();

#endif