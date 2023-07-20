#ifndef __DISPLAY_CONTROLLER_H
#define __DISPLAY_CONTROLLER_H

// Includes
#include "stm32f4xx_hal.h"
#include <u8g2.h>
#include <stdio.h>

// APPS
//#include "applications/apps.h"

// Defines
#define screen_width 128
#define screen_height 64

// Public Functions
void clearScreenBuffer(void);
void sendScreenBuffer(void);
void initDisplay(void);
void drawScreen(char time[7], uint8_t battery_level);

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

#endif