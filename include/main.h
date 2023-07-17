/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#include "stm32f4xx_ll_i2c.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_rtc.h"
#include "stm32f4xx_ll_spi.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CC1101_GD0_Pin LL_GPIO_PIN_1
#define CC1101_GD0_GPIO_Port GPIOA
#define CC1101_CS_Pin LL_GPIO_PIN_4
#define CC1101_CS_GPIO_Port GPIOA
#define BUTTON_UP_Pin LL_GPIO_PIN_12
#define BUTTON_UP_GPIO_Port GPIOB
#define BUTTON_RIGHT_Pin LL_GPIO_PIN_13
#define BUTTON_RIGHT_GPIO_Port GPIOB
#define BUTTON_DOWN_Pin LL_GPIO_PIN_14
#define BUTTON_DOWN_GPIO_Port GPIOB
#define BUTTON_LEFT_Pin LL_GPIO_PIN_8
#define BUTTON_LEFT_GPIO_Port GPIOA
#define BUTTON_BACK_Pin LL_GPIO_PIN_9
#define BUTTON_BACK_GPIO_Port GPIOA
#define BUTTON_ACTION_Pin LL_GPIO_PIN_10
#define BUTTON_ACTION_GPIO_Port GPIOA
#define SDIO_DETECT_Pin LL_GPIO_PIN_8
#define SDIO_DETECT_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
