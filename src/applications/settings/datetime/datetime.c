#include "applications/apps.h"
#include "d_images.h"

extern RTC_HandleTypeDef hrtc;

static RTC_TimeTypeDef appTime = {0};
static RTC_TimeTypeDef setTime = {0};
static RTC_DateTypeDef appDate = {0};
static RTC_DateTypeDef setDate = {0};
char time[4];
uint8_t offset = 19;

static uint32_t left_lastGetTick = 0;
static uint32_t right_lastGetTick = 0;
static uint32_t up_lastGetTick = 0;
static uint32_t down_lastGetTick = 0;

static uint32_t anim_lastGetTick = 0;

void get_time(char* time_str);
void set_time(RTC_TimeTypeDef time);
void reset_time(void);
void reset_date(void);

void datetime_app(u8g2_t u8g2)
{
    uint8_t selected_time_frame = 0;

    HAL_RTC_GetTime(&hrtc, &setTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &setDate, RTC_FORMAT_BIN);

    while (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8))
    {
        uint8_t preview_time[] = {
            (setTime.Hours / 10) % 10,
            setTime.Hours % 10,
            (setTime.Minutes / 10) % 10,
            setTime.Minutes % 10
        };

        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12))
        {
            // NAVIGATION UP
            if((HAL_GetTick() - up_lastGetTick) >= 200)
            {
                switch (selected_time_frame)
                {
                case 0:
                    if (setTime.Hours < 14)
                    {
                        setTime.Hours += 10;
                    }
                    else if (setTime.Hours > 13)
                    {
                        setTime.Hours = 23;
                    }
                    break;
                case 1:
                    if (setTime.Hours < 23)
                    {
                        setTime.Hours++;
                    }
                    break;
                case 2:
                    if (setTime.Minutes < 50)
                    {
                        setTime.Minutes += 10;
                    }
                    break;
                case 3:
                    if (setTime.Minutes < 59)
                    {
                        setTime.Minutes++;
                    }
                    break;
                }
                up_lastGetTick = HAL_GetTick();
            }
        }
        else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13))
        {
            // NAVIGATION RIGHT
            if((HAL_GetTick() - right_lastGetTick) >= 200)
            {
                if (selected_time_frame < 3)
                {
                    selected_time_frame++;
                }
                right_lastGetTick = HAL_GetTick();
            }
        }
        else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14))
        {
            // NAVIGATION DOWN
            if((HAL_GetTick() - down_lastGetTick) >= 200)
            {
                switch (selected_time_frame)
                {
                case 0:
                    if (setTime.Hours > 9)
                    {
                        setTime.Hours -= 10;
                    }
                    break;
                case 1:
                    if (setTime.Hours > 00)
                    {
                        setTime.Hours--;
                    }
                    break;
                case 2:
                    if (setTime.Minutes > 9)
                    {
                        setTime.Minutes -= 10;
                    }
                    break;
                case 3:
                    if (setTime.Minutes > 0)
                    {
                        setTime.Minutes--;
                    }
                    break;
                }
                down_lastGetTick = HAL_GetTick();
            }
            
        }
        else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15))
        {
            // NAVIGATION LEFT
            if((HAL_GetTick() - left_lastGetTick) >= 200)
            {
                if (selected_time_frame > 0)
                {
                    selected_time_frame--;
                }
                left_lastGetTick = HAL_GetTick();
            }
        }

        u8g2_ClearBuffer(&u8g2);

        get_time(time);

        u8g2_SetDrawColor(&u8g2,1);
        u8g2_SetFont(&u8g2, u8g2_font_6x12_tr);
        u8g2_DrawStr(&u8g2, 39, 8, time);

        u8g2_SetFont(&u8g2, u8g2_font_9x18_tr);
        u8g2_DrawStr(&u8g2, 58, 33, ":");
        u8g2_SetFont(&u8g2, u8g2_font_9x18B_tr);


        uint8_t a_offset;
        if((HAL_GetTick() - anim_lastGetTick) >= 250)
        {
            a_offset++;
            anim_lastGetTick = HAL_GetTick();
        }

        if (a_offset > 3)
        {
            a_offset = 0;
        }
        
        u8g2_DrawXBM(&u8g2, 115 + a_offset, 25, arrow_width, arrow_height, right_arrow);

        for (uint8_t i = 0; i < 4; i++)
        {
            // ik i could have done offset = 20 * i, but it didnt work when having a space in the middle, dont judge me this seemed easier and its staying that way
            switch (i)
            {
                case 0:
                    offset = 0;
                    break;
                case 1:
                    offset = 20;
                    break;
                case 2:
                    offset = 44;
                    break;
                case 3:
                    offset = 64;
                    break;
            }
            
            if (i == selected_time_frame)
            {
                u8g2_DrawXBM(&u8g2, 22 + offset, 15, time_frame_width, time_frame_height, time_frame_highlighted);
                u8g2_DrawXBM(&u8g2, 28 + offset, 10, cursor_width, cursor_height, cursor_up);
                u8g2_DrawXBM(&u8g2, 28 + offset, 45, cursor_width, cursor_height, cursor_down);
                u8g2_SetDrawColor(&u8g2, 0);
                char time_value[2] = { preview_time[i] + '0', '\0' };
                u8g2_DrawStr(&u8g2, 27 + offset, 34, &time_value);
                u8g2_SetDrawColor(&u8g2, 1);
            }
            else
            {
                u8g2_DrawXBM(&u8g2, 22 + offset , 15, time_frame_width, time_frame_height, &time_frame);
                char time_value[2] = { preview_time[i] + '0', '\0' };
                u8g2_DrawStr(&u8g2, 27 + offset, 34, &time_value);
            }            
        }

        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9))
        {
            reset_time();
            set_time(setTime);
        }

        u8g2_SendBuffer(&u8g2);
    }
}

void reset_time()
{
    appTime.Hours = 0x0;
    appTime.Minutes = 0x0;
    appTime.Seconds = 0x0;
    appTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    appTime.StoreOperation = RTC_STOREOPERATION_RESET;
    if (HAL_RTC_SetTime(&hrtc, &appTime, RTC_FORMAT_BCD) != HAL_OK)
    {
      Error_Handler();
    }
    HAL_RTC_WaitForSynchro(&hrtc);
}

void reset_date()
{
    appDate.WeekDay = RTC_WEEKDAY_TUESDAY;
    appDate.Month = RTC_MONTH_MAY;
    appDate.Date = 0x0;
    appDate.Year = 0x0;

    if (HAL_RTC_SetDate(&hrtc, &appDate, RTC_FORMAT_BCD) != HAL_OK)
    {
      Error_Handler();
    }
    HAL_RTC_WaitForSynchro(&hrtc);
}

void set_time(RTC_TimeTypeDef time)
{
    setTime.Seconds = 0x0;
    setTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    setTime.StoreOperation = RTC_STOREOPERATION_RESET;
    if (HAL_RTC_SetTime(&hrtc, &setTime, RTC_FORMAT_BIN) != HAL_OK)
    {
      Error_Handler();
    }
    HAL_RTC_WaitForSynchro(&hrtc);
}

void get_time(char* time_str)
{
    HAL_RTC_GetTime(&hrtc, &appTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &appDate, RTC_FORMAT_BIN);
    sprintf(time_str,"%0.2d:%0.2d:%0.2d", appTime.Hours, appTime.Minutes, appTime.Seconds);
}