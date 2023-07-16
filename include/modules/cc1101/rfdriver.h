#ifndef __RFDRIVER_H
#define __RFDRIVER_H

#include "modules/cc1101/cc1101.h"
#include "modules/cc1101/cc1101_configs.h"

/** Radio Presets */
typedef enum {
    SubGhzPresetIDLE, /**< default configuration */
    SubGhzPresetOok270Async, /**< OOK, bandwidth 270kHz, asynchronous */
    SubGhzPresetOok650Async, /**< OOK, bandwidth 650kHz, asynchronous */
    SubGhzPreset2FSKDev238Async, /**< FM, deviation 2.380371 kHz, asynchronous */
    SubGhzPreset2FSKDev476Async, /**< FM, deviation 47.60742 kHz, asynchronous */
    SubGhzPresetMSK99_97KbAsync, /**< MSK, deviation 47.60742 kHz, 99.97Kb/s, asynchronous */
    SubGhzPresetGFSK9_99KbAsync, /**< GFSK, deviation 19.042969 kHz, 9.996Kb/s, asynchronous */
    SubGhzPresetCustom, /**Custom Preset*/
} SubGhzPreset;

void rf_init();
float rf_get_rssi();
uint8_t rf_get_lqi();
uint32_t rf_set_frequency(uint32_t value);
bool rf_is_frequency_valid(uint32_t value);
void subghz_load_registers(uint8_t* data);
void subghz_sleep();
void subghz_load_preset(SubGhzPreset preset);
void subghz_write_packet(const uint8_t* data, uint8_t size);

#endif