#include "modules/cc1101/cc1101.h"

void rf_init(SPI_HandleTypeDef rf_spi)
{
    // Reset
    cc1101_reset(rf_spi);
    cc1101_write_reg(rf_spi, CC1101_IOCFG0, CC1101IocfgHighImpedance);

    // GD0 low
    cc1101_write_reg(rf_spi, CC1101_IOCFG0, CC1101IocfgHW);
    while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) != false)
        ;

    // GD0 high
    cc1101_write_reg(rf_spi, CC1101_IOCFG0, CC1101IocfgHW | CC1101_IOCFG_INV);
    while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) != true)
        ;

    // Reset GD0 to floating state
    cc1101_write_reg(rf_spi, CC1101_IOCFG0, CC1101IocfgHighImpedance);

    // Go to sleep
    cc1101_shutdown(rf_spi);
}

float rf_get_rssi(SPI_HandleTypeDef rf_spi) 
{
    int32_t rssi_dec = cc1101_get_rssi(rf_spi);

    float rssi = rssi_dec;
    if(rssi_dec >= 128) {
        rssi = ((rssi - 256.0f) / 2.0f) - 74.0f;
    } else {
        rssi = (rssi / 2.0f) - 74.0f;
    }

    return rssi;
}

uint8_t rf_get_lqi(SPI_HandleTypeDef rf_spi) {
    uint8_t data[1];
    cc1101_read_reg(rf_spi, CC1101_STATUS_LQI | CC1101_BURST, data);
    return data[0] & 0x7F;
}

uint32_t rf_set_frequency(SPI_HandleTypeDef rf_spi, uint32_t value) {
    uint32_t real_frequency = cc1101_set_frequency(rf_spi, value);
    cc1101_calibrate(rf_spi);

    while(true) {
        CC1101Status status = cc1101_get_status(rf_spi);
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