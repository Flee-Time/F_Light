#include "applications/apps.h"

extern RTC_HandleTypeDef hrtc;

static RTC_TimeTypeDef appTime = {0};
static RTC_DateTypeDef appDate = {0};
char time[8];

void get_time(char* time_str);

void datetime_app(u8g2_t u8g2)
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

        get_time(time);

        u8g2_SetDrawColor(&u8g2,1);
        u8g2_SetFont(&u8g2, u8g2_font_6x12_tr);
        u8g2_DrawStr(&u8g2, 40, 15, time);

        u8g2_SendBuffer(&u8g2);
    }
}

void reset_datetime()
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

void get_time(char* time_str)
{
    HAL_RTC_GetTime(&hrtc, &appTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &appDate, RTC_FORMAT_BIN);
    sprintf(time_str,"%0.2d:%0.2d:%0.2d", appTime.Hours, appTime.Minutes, appTime.Seconds);
}