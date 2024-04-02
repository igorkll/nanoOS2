#include <stdint.h>
#include <esp_err.h>

esp_err_t leds_init();
int leds_getCount();
void leds_setColor(int index, uint32_t color);