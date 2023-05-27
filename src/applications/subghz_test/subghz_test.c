#include "applications/apps.h"
#include "modules/cc1101/cc1101.h"

extern SPI_HandleTypeDef hspi1;

/*
 *
 * Spi communication is not working reliably.
 * Still has many bug, not enough working parts to call the RF implementation done.
 * Will fix in future, hopefully.
 * 
*/

void subghz_test(u8g2_t u8g2)
{
    cc1101_reset(hspi1);

    while (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8))
    {
        u8g2_ClearBuffer(&u8g2);

        CC1101Status state = cc1101_get_status(hspi1);
        uint8_t rssi = cc1101_get_rssi(hspi1);
        uint8_t part_num = cc1101_get_partnumber(hspi1);
        uint8_t version = cc1101_get_version(hspi1);

        u8g2_SetDrawColor(&u8g2,1);
        u8g2_SetFont(&u8g2, u8g2_font_6x12_tr);
        u8g2_DrawStr(&u8g2, 2, 10, part_num);
        u8g2_DrawStr(&u8g2, 2, 25, version);
        u8g2_DrawStr(&u8g2, 2, 40, state.CHIP_RDYn ? "Chip ready" : "Chip not ready");
        u8g2_DrawStr(&u8g2, 2, 55, rssi);

        u8g2_SendBuffer(&u8g2);
    }
}