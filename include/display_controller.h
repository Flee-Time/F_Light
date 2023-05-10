#ifndef __DISPLAY_CONTROLLER_H
#define __DISPLAY_CONTROLLER_H

// Includes
#include "stm32f4xx_hal.h"
#include <u8g2.h>
#include <stdio.h>

// Defines
#define screen_width 128
#define screen_height 64

// Public Functions
void initDisplay(void);
void loadingScreen(void);
void drawScreen(char time[7], uint8_t battery_level);

#endif