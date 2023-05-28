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

float rssi_dbm = 0;
char part_num_char[8] = {0};
char version_char[8] = {0};
uint8_t rssi_dec = 0;
uint8_t part_num = 0;
uint8_t version = 0;

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void subghz_test(u8g2_t u8g2)
{
    cc1101_reset(hspi1);
    cc1101_calibrate(hspi1);

    while (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8))
    {
        u8g2_ClearBuffer(&u8g2);

        cc1101_set_frequency(hspi1, 433800000);

        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12))
        {
            cc1101_switch_to_rx(hspi1);
        }
        else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14))
        {
            cc1101_switch_to_tx(hspi1);
            //does not transmit at this stage, just puts the cc1101 in transmit mode
        }
        else
        {
            cc1101_switch_to_idle(hspi1);
        }

        CC1101Status state = cc1101_get_status(hspi1);
        if (!state.CHIP_RDYn)
        {
            rssi_dec = cc1101_get_rssi(hspi1);
            part_num = cc1101_get_partnumber(hspi1);
            version = cc1101_get_version(hspi1);
        }

        cc1101_flush_rx(hspi1);
        cc1101_flush_tx(hspi1);

        sprintf(version_char,"%0d", version);
        sprintf(part_num_char,"%0d", part_num);

        if (rssi_dec >= 128 )
        {
            rssi_dbm = (rssi_dec - 256)/2 - 74;
        }
        else if (rssi_dec < 128)
        {
            rssi_dbm = (rssi_dec)/2 - 74;
        }

        u8g2_SetDrawColor(&u8g2,1);
        u8g2_SetFont(&u8g2, u8g2_font_6x12_tr);
        u8g2_DrawStr(&u8g2, 1, 10, "Part Num:");
        u8g2_DrawStr(&u8g2, 54, 10, part_num_char);
        u8g2_DrawStr(&u8g2, 1, 20, "Version Num:");
        u8g2_DrawStr(&u8g2, 73, 20, version_char);
        u8g2_DrawStr(&u8g2, 1, 30, "Chip State:");
        u8g2_DrawStr(&u8g2, 67, 30, !state.CHIP_RDYn ? "Ready" : "Not Ready");
        
        if (state.STATE == 0b001)
        {
            u8g2_DrawStr(&u8g2, 1, 64, "RX Mode");
        }
        else if (state.STATE == 0b010)
        {
            u8g2_DrawStr(&u8g2, 1, 64, "TX Mode");    
        }
        else if (state.STATE == 0b000)
        {
            u8g2_DrawStr(&u8g2, 1, 64, "Idle");
        }
        else if (state.STATE == 0b011)
        {
            u8g2_DrawStr(&u8g2, 1, 64, "Fast TX Mode");
        }
        else if (state.STATE == 0b100)
        {
            u8g2_DrawStr(&u8g2, 1, 64, "Calibration");
        }
        else if (state.STATE == 0b101)
        {
            u8g2_DrawStr(&u8g2, 1, 64, "PLL Settling");
        }
        else if (state.STATE == 0b110)
        {
            u8g2_DrawStr(&u8g2, 1, 64, "RX FIFO Overflow");
        }
        else if (state.STATE == 0b111)
        {
            u8g2_DrawStr(&u8g2, 1, 64, "TX FIFO Underflow");
        }

        if (!state.CHIP_RDYn)
        {
            u8g2_DrawStr(&u8g2, 88, 10, "RSSI:");
            u8g2_DrawFrame(&u8g2, 118, 1, 10, 63);
            u8g2_DrawBox(&u8g2, 120, 3, 6, map(rssi_dbm, -120, 0, 1, 61));
        }

        u8g2_SendBuffer(&u8g2);
    }
}