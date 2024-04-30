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

void settings_run() {
    const char* strs[] = {"scale", "< back"};
    
    struct menuState menu = {
        .title = "settings",
        .pointsCount = C_SIZE(strs),
        .points = strs
    };
    
    while (true) {
        gui_menu(&menu);
        switch (menu.current) {
            case 0:
                _scale();
                break;
            default:
                return;
        }
    }
}