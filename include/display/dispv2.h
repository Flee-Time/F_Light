#ifndef __DISPV2_H
#define __DISPV2_H

// Includes
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include <u8g2.h>
#include <stdio.h>
#include "apps/apps.h"

void initDisplay(void);
void drawScreen(void);

// Defines
#define screen_width 128
#define screen_height 64

typedef struct {
    const char* label;
    const char* menuIcon;
    osThreadFunc_t action;
} MenuItem;

typedef struct {
    const char* title;
    const MenuItem* items;
    size_t num_items;
} Menu;

void drawMainMenu(const Menu* menu);

#endif