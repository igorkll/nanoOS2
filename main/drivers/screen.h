#include <stdint.h>
#include <esp_err.h>

typedef enum {
    screen_blackwhite,
    screen_monochrome,
    screen_colored
} screen_colormode;

esp_err_t screen_init();
int screen_x();
int screen_y();

void screen_set(int x, int y, uint32_t color);
uint32_t screen_get(int x, int y);
void screen_update();
screen_colormode screen_getColormode();