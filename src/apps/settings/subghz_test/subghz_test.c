#include "apps/apps.h"
#include "modules/cc1101/cc1101.h"
#include "modules/cc1101/rfdriver.h"

/*
 *
 * Spi communication is not working reliably.
 * Still has many bug, not enough working parts to call the RF implementation done.
 * Todo : Add hardware timers for recording raw rf data and transmission.
 * 
*/

extern RTC_HandleTypeDef hrtc;
extern osThreadId_t defaultTaskHandle;

extern uint8_t upPressed;
extern uint8_t downPressed;
extern uint8_t leftPressed;
extern uint8_t rightPressed;
extern uint8_t actionPressed;
extern uint8_t cancelPressed;

float rssi_dbm = 0;
char part_num_char[8] = {0};
char version_char[8] = {0};
static uint32_t left_lastGetTick = 0;
static uint32_t right_lastGetTick = 0;
float rssi = 0;
uint8_t part_num = 0;
uint8_t version = 0;

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void subghz_test(u8g2_t *u8g2)
{
    uint8_t loop = 129;
    uint8_t curr_screen = 0;

    left_lastGetTick = HAL_GetTick;
    right_lastGetTick = HAL_GetTick;

    uint8_t buffer[32];

    rf_init();

    cc1101_wake();
    subghz_load_preset(SubGhzPreset2FSKDev476Async);

    rf_set_frequency(433850000);

    while (!cancelPressed)
    {
        if (!curr_screen || loop == 128)
        {
            u8g2_ClearBuffer(&u8g2);
        }

        if (downPressed || curr_screen)
        {
            cc1101_switch_to_rx();
            
            for (uint8_t i = 0; i < 64; i++)
            {
                buffer[i] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
                HAL_Delay(1);
            }
        }
        else if (upPressed)
        {
            cc1101_switch_to_tx();
            //subghz_write_packet(buffer, 32);
            for (uint8_t i = 0; i < sizeof(buffer); i++)
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, buffer[i]);
                HAL_Delay(1);
            }
            
            //Transmits something, not sure how to control this for now.
        }
        else
        {
            cc1101_switch_to_idle();
        }

        if (rightPressed)
        {
            // NAVIGATION RIGHT
            if((HAL_GetTick() - right_lastGetTick) >= 200)
            {
                if (!curr_screen)
                {
                    curr_screen = 1;
                    loop = 129;   
                }
                right_lastGetTick = HAL_GetTick();
            }
        }
        else if (leftPressed)
        {
            // NAVIGATION LEFT
            if((HAL_GetTick() - left_lastGetTick) >= 200)
            {
                if (curr_screen)
                {
                    curr_screen = 0;
                }
                left_lastGetTick = HAL_GetTick();
            }
        }

        CC1101Status state = cc1101_get_status();
        if (!state.CHIP_RDYn)
        {
            rssi = rf_get_rssi();
            part_num = cc1101_get_partnumber();
            version = cc1101_get_version();
        }

        cc1101_flush_rx();
        cc1101_flush_tx();

        sprintf(version_char,"%0d", version);
        sprintf(part_num_char,"%0d", part_num);

        if (curr_screen) // RSSI SPECTRUM SCREEN
        {
            u8g2_SetDrawColor(&u8g2,1);
            u8g2_SetFont(&u8g2, u8g2_font_6x12_tr);
            u8g2_DrawStr(&u8g2, 1, 10, "RSSI Spectrum:");
            u8g2_DrawStr(&u8g2, 1, 20, "Back: <=");
            u8g2_SetDisplayRotation(&u8g2, &u8g2_cb_r2);
            u8g2_DrawBox(&u8g2, loop, 0, 1, map(rssi, -120, 0, 0, 80));
            u8g2_SetDisplayRotation(&u8g2, &u8g2_cb_r0);

            if (loop == 0)
            {
                loop = 129;
            }

            loop--;
        }
        else // MAIN SCREEN
        {
            u8g2_SetDrawColor(&u8g2,1);
            u8g2_SetFont(&u8g2, u8g2_font_6x12_tr);
            u8g2_DrawStr(&u8g2, 1, 10, "Part Num:");
            u8g2_DrawStr(&u8g2, 54, 10, part_num_char);
            u8g2_DrawStr(&u8g2, 1, 20, "Version Num:");
            u8g2_DrawStr(&u8g2, 73, 20, version_char);
            u8g2_DrawStr(&u8g2, 1, 30, "Chip State:");
            u8g2_DrawStr(&u8g2, 67, 30, !state.CHIP_RDYn ? "Ready" : "Not Ready");
            u8g2_DrawStr(&u8g2, 1, 40, "Spectrum: =>");

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
                u8g2_DrawBox(&u8g2, 120, 3, 6, map(rssi, -120, 0, 1, 61));
            }
        }

        u8g2_SendBuffer(&u8g2);
    }
    vTaskDelay(50);
    osThreadResume(defaultTaskHandle);
}