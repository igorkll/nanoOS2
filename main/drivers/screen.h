#pragma once
#include <stdint.h>
#include <esp_err.h>
#include "../color.h"

typedef enum {
    screen_blackwhite,
    screen_monochrome,
    screen_colored
} screen_colormode;

esp_err_t screen_init();
int screen_x();
int screen_y();

void screen_set(int x, int y, tcolor color);
tcolor screen_get(int x, int y);
void screen_update();
screen_colormode screen_getColormode();
void screen_setBacklightValue(uint8_t);