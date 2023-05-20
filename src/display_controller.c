#include "display_controller.h"
#include "static/loading_screen/l_screen.h"
#include "static/main_menu/m_back.h"

static u8g2_t u8g2;
uint8_t *buf;
uint8_t selected_menu = 0;
uint8_t m_sel = 0;

static uint32_t up_lastGetTick = 0;
static uint32_t down_lastGetTick = 0;
static uint32_t confirm_lastGetTick = 0;
static uint32_t back_lastGetTick = 0;

extern uint8_t u8x8_stm32_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
extern uint8_t u8x8_byte_stm32_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

// TODO : Fix bug when coming out of apps the menu goes one back.

typedef void (*MenuAction)(u8g2_t u8g2);

typedef struct {
    const char* label;
    const char* menuIcon;
    MenuAction action;
} MenuItem;

typedef struct {
    const char* title;
    const MenuItem* items;
    size_t num_items;
} Menu;

void displayMenu(const Menu* menu, uint8_t selection);
uint8_t handleInput(const Menu* menu);
void goto_settings(void);
void goto_mainmenu(void);

// Main Menu
MenuItem menuItems[] = {
    {"Option 1", NULL, NULL},
    {"Option 2", NULL, NULL},
    {"System Settings", settings_icon, goto_settings},
    {"Option 4", NULL, NULL},
    {"Option 5", NULL, test_app}
};

Menu mainMenu = {
    "Main Menu",
    menuItems,
    sizeof(menuItems) / sizeof(MenuItem)
};

// Settings Submenu
MenuItem settingsItems[] = {
    {"Set DATE&TIME", clock_icon, datetime_app},
    {"Exit", NULL, goto_mainmenu},
    {"Test 1", NULL, NULL},
    {"Test 2", NULL, NULL}
};

Menu settingsMenu = {
    "Settings",
    settingsItems,
    sizeof(settingsItems) / sizeof(MenuItem)
};

void initDisplay()
{
    u8g2_Setup_sh1106_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_stm32_hw_i2c, u8x8_stm32_gpio_and_delay);
    buf = (uint8_t *)malloc(u8g2_GetBufferSize(&u8g2));
    u8g2_SetBufferPtr(&u8g2, buf);
    u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);
}

void loadingScreen()
{
    for (int i = 0; i < 85; i++)
    {
	    u8g2_ClearBuffer(&u8g2);
	    u8g2_SetDrawColor(&u8g2,1);
        u8g2_DrawXBM(&u8g2, 0, 0, screen_width, screen_height, l_screen);
        u8g2_DrawBox(&u8g2, 22, 52, i, 6);
        u8g2_SendBuffer(&u8g2);
    }
}

void drawScreen(char time[7], uint8_t battery_level)
{
    // Think of a better submenu system, one which has context of which menu was the previous one
    Menu *menu[] = {&mainMenu, &settingsMenu};
    
	u8g2_ClearBuffer(&u8g2);

    u8g2_SetBitmapMode(&u8g2, 1);
	u8g2_SetDrawColor(&u8g2,1);
    u8g2_DrawXBM(&u8g2, 0, 0, screen_width, screen_height, m_back);
    u8g2_DrawXBM(&u8g2, 45, 1, bicon_width, bicon_height, battery[battery_level]);
    u8g2_SetFont(&u8g2, u8g2_font_4x6_mf);
    u8g2_DrawStr(&u8g2, 1, 7, time);

    uint8_t selection = handleInput(menu[selected_menu]);
    displayMenu(menu[selected_menu], selection);

    u8g2_SendBuffer(&u8g2);
}

void displayMenu(const Menu* menu, uint8_t selection) {
    uint8_t y_offset = 0;

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

uint8_t handleInput(const Menu* menu)
{
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) && m_sel < (menu->num_items - 1))
    {
        if((HAL_GetTick() - up_lastGetTick) >= 200)
        {
            m_sel++;
            up_lastGetTick = HAL_GetTick();
        }
    }
    else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) && m_sel > 0)
    {
        if((HAL_GetTick() - down_lastGetTick) >= 200)
        {
            m_sel--;
            down_lastGetTick = HAL_GetTick();
        }
    }

    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9))
    {
        if (menu->items[m_sel].action != NULL)
        {
            if((HAL_GetTick() - confirm_lastGetTick) >= 200)
            {
                menu->items[m_sel].action(u8g2);
                confirm_lastGetTick = HAL_GetTick();
            }
        }
    }

    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) && selected_menu > 0)
    {
        if((HAL_GetTick() - back_lastGetTick) >= 200)
        {
            selected_menu--;
            back_lastGetTick = HAL_GetTick();
        }
    }

    return m_sel;
}

void goto_settings()
{
    selected_menu = 1;
    m_sel = 0;
}

void goto_mainmenu()
{
    selected_menu = 0;
    m_sel = 0;
}