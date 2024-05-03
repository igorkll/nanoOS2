#include "settings.h"
#include "../all.h"

static void _scale() {
    const char* strs[] = {"1", "2", "3", "4", "< back"};
    
    struct menuState menu = {
        .title = "settings - scale",
        .pointsCount = C_SIZE(strs),
        .points = strs
    };
    
    while (true) {
        if (gui_menu(&menu) == C_SIZE(strs) - 1) break;
        sysconf_data.cropX = menu.current + 1;
        sysconf_data.cropY = menu.current + 1;
        storage_sysconf_save();
    }
}

void doBrightness(int16_t val, uint8_t valnum) {
    switch (val) {
        case -2:
            switch (valnum) {
                case 0:
                    sysconf_data.screen_light_active = device_getBacklightValue(val);
                    break;
                case 1:
                    sysconf_data.screen_light_idle = device_getBacklightValue(val);
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
    gui_menu_addExit(&menu, NULL, NULL);

    // screen
    gui_menu_addSlider(screen, "brightness", NULL, brightnessCallback, sysconf_data.screen_light_active);
    gui_menu_addSlider(screen, "idle brightness", NULL, idleBrightnessCallback, sysconf_data.screen_light_idle);
    gui_menu_addExit(&menu, NULL, NULL);

    gui_menu_runOnce(&menu);
}