#include "display/dispv2.h"
#include "display/static/m_back.h"

extern osThreadId_t defaultTaskHandle;

static u8g2_t u8g2;
uint8_t *buf;

extern uint8_t u8x8_stm32_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
extern uint8_t u8x8_byte_stm32_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

static uint32_t up_lastGetTick = 0;
static uint32_t down_lastGetTick = 0;
static uint32_t confirm_lastGetTick = 0;
static uint32_t back_lastGetTick = 0;

osThreadId_t appTaskHandle;
const osThreadAttr_t appTask_attributes = {
  .name = "appThread",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

//uint8_t getBatteryLevel(void);

// Main Menu
MenuItem menuItems[] = {
    {"Subghz Test", NULL, subghz_test},
    {"Date/Time App", NULL, datetime_app},
    {"Icon Test", settings_icon, NULL},
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

extern uint8_t upPressed;
extern uint8_t downPressed;
extern uint8_t leftPressed;
extern uint8_t rightPressed;
extern uint8_t actionPressed;
extern uint8_t cancelPressed;

void drawScreen()
{   
    char showtime[8];
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

    drawMainMenu(&mainMenu);

    /*
    u8g2_DrawXBM(&u8g2, 60, 16, button_icon_width, button_icon_height, upPressed ? button_pressed : button_released);
    u8g2_DrawXBM(&u8g2, 60, 48, button_icon_width, button_icon_height, downPressed ? button_pressed : button_released);
    u8g2_DrawXBM(&u8g2, 44, 32, button_icon_width, button_icon_height, leftPressed ? button_pressed : button_released);
    u8g2_DrawXBM(&u8g2, 76, 32, button_icon_width, button_icon_height, rightPressed ? button_pressed : button_released);
    */

    u8g2_SendBuffer(&u8g2);
}

uint8_t m_sel = 0;

uint8_t menuSelectionHandler(const Menu* menu)
{
    if (downPressed && m_sel < (menu->num_items - 1))
    {
        if((HAL_GetTick() - up_lastGetTick) >= 300)
        {
            m_sel++;
            up_lastGetTick = HAL_GetTick();
        }
    }
    else if (upPressed && m_sel > 0)
    {
        if((HAL_GetTick() - down_lastGetTick) >= 300)
        {
            m_sel--;
            down_lastGetTick = HAL_GetTick();
        }
    }

    if (actionPressed)
    {
        if (menu->items[m_sel].action != NULL)
        {
            if((HAL_GetTick() - confirm_lastGetTick) >= 300)
            {
                appTaskHandle = osThreadNew(menu->items[m_sel].action, &u8g2, &appTask_attributes);
                vTaskDelay(15);
                //osThreadSuspend(defaultTaskHandle);
                confirm_lastGetTick = HAL_GetTick();
            }
        }
    }

    /*
    if (cancelPressed && selected_menu > 0)
    {
        if((HAL_GetTick() - back_lastGetTick) >= 300)
        {
            selected_menu--;
            back_lastGetTick = HAL_GetTick();
        }
    }
    */

    return m_sel;
}

void drawMainMenu(const Menu* menu)
{
    uint8_t y_offset = 0;   
    uint8_t selection = menuSelectionHandler(menu); 
    u8g2_SetDrawColor(&u8g2,1);
    u8g2_SetFont(&u8g2, u8g2_font_6x10_tr);
    u8g2_DrawStr(&u8g2, 65, 9, menu->title);
    u8g2_DrawBox(&u8g2, 124, 5 + 54.5/menu->num_items * selection, 3, 54.5/menu->num_items);    
    for (int i = (selection < (menu->num_items - 1) ? (selection < 1 ? 0 : (selection - 1)) : (selection - 2)); i < menu->num_items; i++)
    {
        u8g2_SetDrawColor(&u8g2, 1);
        u8g2_DrawXBM(&u8g2, 1, 12 + y_offset, menu_item_width, menu_item_height, selection == i ? menu_item_highlighted : menu_item);   
        if (menu->items[i].menuIcon != NULL)
        {
            u8g2_SetDrawColor(&u8g2, selection == i ? 0 : 1);
            u8g2_DrawXBM(&u8g2, 5, 14 + y_offset, menu_icon_width, menu_icon_height, menu->items[i].menuIcon);
        }   
        u8g2_SetDrawColor(&u8g2, selection == i ? 0 : 1);
        u8g2_SetFont(&u8g2, u8g2_font_6x10_tr);
        u8g2_DrawStr(&u8g2, 20, 23 + y_offset, menu->items[i].label);
        u8g2_SetDrawColor(&u8g2,0);
        y_offset += 18;
    }
}