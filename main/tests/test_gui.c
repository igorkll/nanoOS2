#include "test_keyboard.h"
#include "../all.h"

static void sliderCallback(int16_t val) {
    printf("%i\n", val);
}

void guitest_run() {
    printf("return %i\n", gui_sliderWithCallback("LOLZ", 0, sliderCallback));

    struct tabMenuState menu;
    gui_menu_init(&menu, "gui test");
    gui_menu_addExit(&menu, "exit", NULL);
    gui_menu_run(&menu);
    gui_menu_free(&menu);
}