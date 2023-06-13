#include "applications/apps.h"

void test_app(u8g2_t u8g2)
{
    while (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8))
    {
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12))
        {
            // NAVIGATION UP
        }
        else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13))
        {
            // NAVIGATION RIGHT
        }
        else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14))
        {
            // NAVIGATION DOWN
        }
        else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15))
        {
            // NAVIGATION LEFT
        }

        u8g2_ClearBuffer(&u8g2);

        u8g2_SetDrawColor(&u8g2,1);
        u8g2_SetFont(&u8g2, u8g2_font_6x12_tr);
        u8g2_DrawStr(&u8g2, 40, 20, "Hello World!");

        u8g2_SendBuffer(&u8g2);
    }
    HAL_Delay(50);
}