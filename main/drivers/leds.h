#pragma once
#include <stdint.h>
#include <esp_err.h>
#include "../color.h"

esp_err_t leds_init();
int leds_getCount();
void leds_setColor(int index, tcolor color);
bool leds_isColored(int index);