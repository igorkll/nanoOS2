#include "test_keyboard.h"
#include "../all.h"

static void sliderCallback(int16_t val) {
    xprintf("slider %i\n", val);
}

void guitest_run() {
    uint8_t sliderValue = 96;
    uint8_t slider2Value = 96;

    struct tabMenuState menu = gui_menu_new("gui test");
    gui_menu_addApp(&menu, "run snake", NULL, snake_run);
    gui_menu_addApp(&menu, "run pong", NULL, pong_run);
    gui_menu_addApp(&menu, "run cave", NULL, cave_run);
    gui_menu_addSlider(&menu, "test slider-callback", NULL, sliderCallback, &sliderValue);
    gui_menu_addSlider(&menu, "test slider", NULL, NULL, &slider2Value);
    gui_menu_addExit(&menu, NULL, NULL);
    gui_menu_runOnce(&menu);
}