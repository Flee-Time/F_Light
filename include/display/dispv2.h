#ifndef __DISPV2_H
#define __DISPV2_H

// Includes
#include "stm32f4xx_hal.h"
#include <u8g2.h>
#include <stdio.h>

// APPS
//#include "applications/apps.h"

void initDisplay(void);
void drawScreen(void);

// Defines
#define screen_width 128
#define screen_height 64

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

void drawMainMenu(const Menu* menu);

#endif