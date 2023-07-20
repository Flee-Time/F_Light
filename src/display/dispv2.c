#include "display/dispv2.h"
#include "display/static/m_back.h"

static u8g2_t u8g2;
uint8_t *buf;

extern uint8_t u8x8_stm32_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
extern uint8_t u8x8_byte_stm32_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

//uint8_t getBatteryLevel(void);

// Main Menu
MenuItem menuItems[] = {
    {"Subghz Test", NULL, NULL},
    {"Option 2", NULL, NULL},
    {"System Settings", settings_icon, NULL},
    {"Option 4", NULL, NULL},
    {"Option 5", NULL, NULL}
};

Menu mainMenu = {
    "Main Menu",
    menuItems,
    sizeof(menuItems) / sizeof(MenuItem)
};

void initDisplay()
{
    u8g2_Setup_sh1106_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_stm32_hw_i2c, u8x8_stm32_gpio_and_delay);
    buf = (uint8_t *)malloc(u8g2_GetBufferSize(&u8g2));
    u8g2_SetBufferPtr(&u8g2, buf);
    u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);
}

extern uint8_t testVar;

void drawScreen()
{   
    char showtime[8];
    char pinno[8];
    extern RTC_HandleTypeDef hrtc;
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};

    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
    sprintf(showtime,"%0.2d:%0.2d:%0.2d", sTime.Hours, sTime.Minutes, sTime.Seconds);

	u8g2_ClearBuffer(&u8g2);

    u8g2_SetBitmapMode(&u8g2, 1);
	u8g2_SetDrawColor(&u8g2,1);
    u8g2_DrawXBM(&u8g2, 0, 0, screen_width, screen_height, m_back);
    u8g2_DrawXBM(&u8g2, 45, 1, bicon_width, bicon_height, battery[3]);
    u8g2_SetFont(&u8g2, u8g2_font_4x6_mf);
    u8g2_DrawStr(&u8g2, 1, 7, showtime);

    /*
    *
    *   DRAW MENU OPTIONS AND HANDLE INPUT HERE
    *
    */

    sprintf(pinno,"%d", testVar);

    u8g2_DrawStr(&u8g2, 30, 30, pinno);

    u8g2_SendBuffer(&u8g2);
}