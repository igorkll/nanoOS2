#include "settings.h"
#include "../all.h"

void scaleMenu() {
    uint8_t scale = gui_selectNumber("scale", false, 1, 4, 1, sysconf_data.cropX);
    if (scale != sysconf_data.cropX) {
        sysconf_data.cropX = scale;
        sysconf_data.cropY = scale;
        storage_sysconf_push();
        storage_sysconf_save();
    }
}

void doBrightness(int16_t val, uint8_t valnum) {
    switch (val) {
        case -2:
            switch (valnum) {
                case 0:
                    sysconf_data.screen_light_active = device_getBacklightValue();
                    break;
                case 1:
                    sysconf_data.screen_light_idle = device_getBacklightValue();
                    break;
            }
            storage_sysconf_save();
            device_setAutoBacklight(true);
            break;
        case -1:
            device_setAutoBacklight(true);
            break;
        default:
            device_setBacklightValue(val);
            break;
    }
}

void brightnessCallback(int16_t val) {
    doBrightness(val, 0);
}

void idleBrightnessCallback(int16_t val) {
    doBrightness(val, 1);
}

void settings_run() {
    struct tabMenuState menu = gui_menu_new("settings");
    
    // settings
    struct tabMenuState* screen = gui_menu_addTab(&menu, "screen", NULL);
    struct tabMenuState* gui = gui_menu_addTab(&menu, "gui", NULL);
    gui_menu_addExit(&menu, NULL, NULL);

    // screen
    gui_menu_addSlider(screen, "brightness", NULL, brightnessCallback, &sysconf_data.screen_light_active);
    gui_menu_addSlider(screen, "idle brightness", NULL, idleBrightnessCallback, &sysconf_data.screen_light_idle);
    gui_menu_addExit(screen, NULL, NULL);

    // gui
    gui_menu_addCallback(gui, "scale", NULL, scaleMenu);
    gui_menu_addExit(gui, NULL, NULL);

    gui_menu_runOnce(&menu);
}