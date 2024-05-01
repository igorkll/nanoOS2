#include "test_keyboard.h"
#include "../all.h"

static void sliderCallback(int16_t val) {
    printf("slider %i\n", val);
}

void guitest_run() {
    uint8_t sliderValue = 96;

    struct tabMenuState menu;
    gui_menu_init(&menu, "gui test");
    gui_menu_addApp(&menu, "run snake", NULL, snake_run);
    gui_menu_addApp(&menu, "run pong", NULL, pong_run);
    gui_menu_addApp(&menu, "run cave", NULL, cave_run);
    gui_menu_addSlider(&menu, "test slider", NULL, sliderCallback, sliderValue);
    gui_menu_addExit(&menu, "exit", NULL);
    gui_menu_run(&menu);
    gui_menu_free(&menu);
}