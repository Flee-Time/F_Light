#include "apps/apps.h"
#include "d_images.h"

extern RTC_HandleTypeDef hrtc;

static RTC_TimeTypeDef appTime = {0};
static RTC_TimeTypeDef setTime = {0};
static RTC_DateTypeDef appDate = {0};
static RTC_DateTypeDef setDate = {0};
static char time[12];
static char date[12];
static uint8_t offset = 0;
static uint8_t a_offset = 0;

static uint32_t left_lastGetTick = 0;
static uint32_t right_lastGetTick = 0;
static uint32_t up_lastGetTick = 0;
static uint32_t down_lastGetTick = 0;

static uint32_t anim_lastGetTick = 0;
static uint32_t init_lastGetTick = 0;
static uint32_t conf_lastGetTick = 0;

extern uint8_t upPressed;
extern uint8_t downPressed;
extern uint8_t leftPressed;
extern uint8_t rightPressed;
extern uint8_t actionPressed;
extern uint8_t cancelPressed;

void get_date_time(char* date_str, char* time_str);
void set_time(RTC_TimeTypeDef time);
void set_date(RTC_DateTypeDef date);
void reset_time(void);
void reset_date(void);

void datetime_app(u8g2_t u8g2)
{
    static uint8_t selected_time_frame = 0;
    static uint8_t initialized = 0;
    static uint8_t date_set = 0;
    static uint8_t time_set = 0;
    init_lastGetTick = HAL_GetTick();
    conf_lastGetTick = HAL_GetTick();

    HAL_RTC_GetTime(&hrtc, &setTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &setDate, RTC_FORMAT_BIN);

    while (!cancelPressed)
    {
        uint8_t preview_time[] = {
            (setTime.Hours / 10) % 10,
            setTime.Hours % 10,
            (setTime.Minutes / 10) % 10,
            setTime.Minutes % 10
        };

        if (rightPressed)
        {
            // NAVIGATION RIGHT
            if((HAL_GetTick() - right_lastGetTick) >= 200)
            {
                if (selected_time_frame < 6)
                {
                    selected_time_frame++;
                }
                right_lastGetTick = HAL_GetTick();
            }
        }
        else if (leftPressed)
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

        u8g2_DrawLine(&u8g2, 0, 54, 128, 54);
        u8g2_SetFont(&u8g2, u8g2_font_6x10_tr);
        u8g2_SetDrawColor(&u8g2, 1);
        u8g2_DrawStr(&u8g2, 2, 63, "Cancel");
        u8g2_DrawStr(&u8g2, 84, 63, "Confirm");

        get_date_time(date, time);

        if((HAL_GetTick() - anim_lastGetTick) >= 250)
        {
            a_offset++;
            anim_lastGetTick = HAL_GetTick();
        }

        if (a_offset > 3)
        {
            a_offset = 0;
        }

        if((HAL_GetTick() - init_lastGetTick) >= 1000)
        {
            initialized = 1;
            init_lastGetTick = HAL_GetTick();
        }

        if (selected_time_frame < 4)
        {
            if (upPressed)
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
                    default:
                        break;
                    }
                    up_lastGetTick = HAL_GetTick();
                }
            }
            else if (downPressed)
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
                    default:
                        break;
                    }
                    down_lastGetTick = HAL_GetTick();
                }
            }

            u8g2_DrawXBM(&u8g2, 115 + a_offset, 25, arrow_width, arrow_height, right_arrow);

            u8g2_SetDrawColor(&u8g2,1);
            u8g2_SetFont(&u8g2, u8g2_font_6x12_tr);
            u8g2_DrawStr(&u8g2, 76, 8, time);

            u8g2_SetFont(&u8g2, u8g2_font_9x18_tr);
            u8g2_DrawStr(&u8g2, 58, 33, ":");

            u8g2_SetFont(&u8g2, u8g2_font_6x12_tr);
            u8g2_DrawStr(&u8g2, 2, 8, "Set Time");

            for (uint8_t i = 0; i < 4; i++)
            {
                // ik i could have done offset = 20 * i, but it didnt work when having a space in the middle, 
                // dont judge me this seemed easier and its staying that way
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
                    default:
                        break;
                }

                u8g2_SetFont(&u8g2, u8g2_font_9x18B_tr);

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

            if (actionPressed && initialized)
            {
                reset_time();
                set_time(setTime);
                time_set = 1;
            }

            if((HAL_GetTick() - conf_lastGetTick) >= 2000 && time_set == 1)
            {
                time_set = 0;
                conf_lastGetTick = HAL_GetTick();
            }

            if (time_set)
            {
                u8g2_SetFont(&u8g2, u8g2_font_5x8_tr);
                u8g2_DrawStr(&u8g2, 40, 63, "Time set");
            }
        }
        else
        {
            if (upPressed)
            {
                // NAVIGATION UP
                if((HAL_GetTick() - up_lastGetTick) >= 200)
                {
                    switch (selected_time_frame - 4)
                    {
                    case 0:
                        if (setDate.Year < 99)
                        {
                            setDate.Year++;
                        }
                        break;
                    case 1:
                        if (setDate.Month < 12)
                        {
                            setDate.Month++;
                        }
                        break;
                    case 2:
                        if (setDate.Date < 31)
                        {
                            setDate.Date++;
                        }
                        break;
                    default:
                        break;
                    }
                    up_lastGetTick = HAL_GetTick();
                }
            }
            else if (downPressed)
            {
                // NAVIGATION DOWN
                if((HAL_GetTick() - down_lastGetTick) >= 200)
                {
                    switch (selected_time_frame - 4)
                    {
                    case 0:
                        if (setDate.Year > 0)
                        {
                            setDate.Year--;
                        }
                        break;
                    case 1:
                        if (setDate.Month > 0)
                        {
                            setDate.Month--;
                        }
                        break;
                    case 2:
                        if (setDate.Date > 0)
                        {
                            setDate.Date--;
                        }
                        break;
                    default:
                        break;
                    }
                    down_lastGetTick = HAL_GetTick();
                }
            }

            u8g2_DrawXBM(&u8g2, 5 - a_offset, 25, arrow_width, arrow_height, left_arrow);

            u8g2_SetFont(&u8g2, u8g2_font_6x10_tr);
            u8g2_DrawStr(&u8g2, 2, 8, "Set Date");

            u8g2_DrawStr(&u8g2, 65, 8, date);

            char year[8];
            sprintf(year,"20%0.2d\0", setDate.Year);
            char month[8];
            sprintf(month,"%0.2d\0", setDate.Month);
            char day[8];
            sprintf(day,"%0.2d\0", setDate.Date);

            u8g2_SetFont(&u8g2, u8g2_font_9x18B_tr);

            for (uint8_t i = 0; i < 3; i++)
            {
                switch (i)
                {
                case 0:
                    if (i == (selected_time_frame - 4))
                    {
                        u8g2_DrawXBM(&u8g2, 36, 14, cursor_width, cursor_height, cursor_up);
                        u8g2_DrawXBM(&u8g2, 36, 41, cursor_width, cursor_height, cursor_down);
                    }
                    u8g2_DrawXBM(&u8g2, 18, 19, year_frame_width, year_frame_height, ((selected_time_frame - 4) == i) ? year_frame_highlighted : year_frame);
                    u8g2_SetDrawColor(&u8g2, ((selected_time_frame - 4) == i) ? 0 : 1);
                    u8g2_DrawStr(&u8g2, 21, 34, year);
                    u8g2_SetDrawColor(&u8g2, 1);
                    break;
                case 1:
                    if (i == (selected_time_frame - 4))
                    {
                        u8g2_DrawXBM(&u8g2, 75, 14, cursor_width, cursor_height, cursor_up);
                        u8g2_DrawXBM(&u8g2, 75, 41, cursor_width, cursor_height, cursor_down);
                    }
                    u8g2_DrawXBM(&u8g2, 65, 19, day_frame_width, day_frame_height, ((selected_time_frame - 4) == 1) ? day_frame_highlighted : day_frame);
                    u8g2_SetDrawColor(&u8g2, ((selected_time_frame - 4) == i) ? 0 : 1);
                    u8g2_DrawStr(&u8g2, 69, 34, month);
                    u8g2_SetDrawColor(&u8g2, 1);
                    break;
                case 2:
                    if (i == (selected_time_frame - 4))
                    {
                        u8g2_DrawXBM(&u8g2, 106, 14, cursor_width, cursor_height, cursor_up);
                        u8g2_DrawXBM(&u8g2, 106, 41, cursor_width, cursor_height, cursor_down);
                    }
                    u8g2_DrawXBM(&u8g2, 96, 19, day_frame_width, day_frame_height, ((selected_time_frame - 4) == 2) ? day_frame_highlighted : day_frame);
                    u8g2_SetDrawColor(&u8g2, ((selected_time_frame - 4) == i) ? 0 : 1);
                    u8g2_DrawStr(&u8g2, 100, 34, day);
                    u8g2_SetDrawColor(&u8g2, 1);
                    break;
                default:
                    break;
                }
            }

            if (actionPressed && initialized)
            {
                reset_date();
                set_date(setDate);
                date_set = 1;
            }

            if((HAL_GetTick() - conf_lastGetTick) >= 2000 && date_set == 1)
            {
                date_set = 0;
                conf_lastGetTick = HAL_GetTick();
            }

            if (date_set)
            {
                u8g2_SetFont(&u8g2, u8g2_font_5x7_tr);
                u8g2_DrawStr(&u8g2, 40, 63, "Date set");
            }
        }

        u8g2_SendBuffer(&u8g2);
    }
    vTaskDelay(50);
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
    time.Seconds = 0x0;
    time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    time.StoreOperation = RTC_STOREOPERATION_RESET;
    if (HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN) != HAL_OK)
    {
      Error_Handler();
    }
    HAL_RTC_WaitForSynchro(&hrtc);
}

void set_date(RTC_DateTypeDef date)
{
    if (HAL_RTC_SetDate(&hrtc, &setDate, RTC_FORMAT_BIN) != HAL_OK)
    {
      Error_Handler();
    }
    HAL_RTC_WaitForSynchro(&hrtc);
}

void get_date_time(char* date_str, char* time_str)
{
    HAL_RTC_GetTime(&hrtc, &appTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &appDate, RTC_FORMAT_BIN);
    sprintf(time_str,"%0.2d:%0.2d:%0.2d", appTime.Hours, appTime.Minutes, appTime.Seconds);
    sprintf(date_str,"20%0.2d/%0.2d/%0.2d", appDate.Year, appDate.Month, appDate.Date);
}