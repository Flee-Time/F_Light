#include "modules/cc1101/rfdriver.h"

// Needs complete rewrite from flipper zero source code to work with FreeRTOS.

void rf_init()
{
    // Reset
    cc1101_reset();
    cc1101_write_reg(CC1101_IOCFG0, CC1101IocfgHighImpedance);

    // GD0 low
    //GD0_Input();
    cc1101_write_reg(CC1101_IOCFG0, CC1101IocfgHW);
    while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) != false)
        ;

    // GD0 high
    cc1101_write_reg(CC1101_IOCFG0, CC1101IocfgHW | CC1101_IOCFG_INV);
    while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) != true)
        ;

    // Reset GD0 to floating state
    cc1101_write_reg(CC1101_IOCFG0, CC1101IocfgHighImpedance);

    // Go to sleep
    cc1101_shutdown();
}

void subghz_load_registers(uint8_t* data) {
    cc1101_reset();
    uint32_t i = 0;
    while(data[i]) {
        cc1101_write_reg(data[i], data[i + 1]);
        i += 2;
    }
}

void subghz_write_packet(const uint8_t* data, uint8_t size) {
    cc1101_flush_tx();
    cc1101_write_reg(CC1101_FIFO, size);
    cc1101_write_fifo(data, size);
}

float rf_get_rssi() 
{
    int32_t rssi_dec = cc1101_get_rssi();

    float rssi = rssi_dec;
    if(rssi_dec >= 128) {
        rssi = ((rssi - 256.0f) / 2.0f) - 74.0f;
    } else {
        rssi = (rssi / 2.0f) - 74.0f;
    }

    return rssi;
}

uint8_t rf_get_lqi() {
    uint8_t data[1];
    cc1101_read_reg(CC1101_STATUS_LQI | CC1101_BURST, data);
    return data[0] & 0x7F;
}

uint32_t rf_set_frequency(uint32_t value) {
    uint32_t real_frequency = cc1101_set_frequency(value);
    cc1101_calibrate();

    while(true) {
        CC1101Status status = cc1101_get_status();
        if(status.STATE == CC1101StateIDLE) break;
    }

    return real_frequency;
}

bool rf_is_frequency_valid(uint32_t value) {
    if(!(value >= 299999755 && value <= 348000335) &&
       !(value >= 386999938 && value <= 464000000) &&
       !(value >= 778999847 && value <= 928000000)) {
        return false;
    }

    return true;
}

void subghz_sleep() {
    cc1101_switch_to_idle();
    cc1101_write_reg(CC1101_IOCFG0, CC1101IocfgHighImpedance);
    //GD0_Analog();
    cc1101_shutdown();
}

bool subghz_rx_pipe_not_empty() {
    CC1101RxBytes status[1];
    cc1101_read_reg((CC1101_STATUS_RXBYTES) | CC1101_BURST, (uint8_t*)status);
    // TODO: you can add a buffer overflow flag if needed
    if(status->NUM_RXBYTES > 0) {
        return true;
    } else {
        return false;
    }
}

bool subghz_is_rx_data_crc_valid() {
    uint8_t data[1];
    cc1101_read_reg(CC1101_STATUS_LQI | CC1101_BURST, data);

    if(((data[0] >> 7) & 0x01)) {
        return true;
    } else {
        return false;
    }
}


void subghz_load_preset(SubGhzPreset preset) {
    if(preset == SubGhzPresetOok650Async) {
        subghz_load_registers((uint8_t*)subghz_preset_ook_650khz_async_regs);
        cc1101_set_pa_table(subghz_preset_ook_async_patable);
    } else if(preset == SubGhzPresetOok270Async) {
        subghz_load_registers((uint8_t*)subghz_preset_ook_270khz_async_regs);
        cc1101_set_pa_table(subghz_preset_ook_async_patable);
    } else if(preset == SubGhzPreset2FSKDev238Async) {
        subghz_load_registers(
            (uint8_t*)subghz_preset_2fsk_dev2_38khz_async_regs);
        cc1101_set_pa_table(subghz_preset_2fsk_async_patable);
    } else if(preset == SubGhzPreset2FSKDev476Async) {
        subghz_load_registers(
            (uint8_t*)subghz_preset_2fsk_dev47_6khz_async_regs);
        cc1101_set_pa_table(subghz_preset_2fsk_async_patable);
    } else if(preset == SubGhzPresetMSK99_97KbAsync) {
        subghz_load_registers((uint8_t*)subghz_preset_msk_99_97kb_async_regs);
        cc1101_set_pa_table(subghz_preset_msk_async_patable);
    } else if(preset == SubGhzPresetGFSK9_99KbAsync) {
        subghz_load_registers((uint8_t*)subghz_preset_gfsk_9_99kb_async_regs);
        cc1101_set_pa_table(subghz_preset_gfsk_async_patable);
    }
}