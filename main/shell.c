#include "all.h"
#include "shell.h"

#ifndef SHELL_TITLE
    #define SHELL_TITLE "nanoOS"
#endif

static void tab_apps() {
    const char* strs[] = {"explorer", "settings", "< back"};
    
    struct menuState menu = {
        .title = "apps",
        .pointsCount = C_SIZE(strs),
        .points = strs
    };
    
    while (true) {
        gui_menu(&menu);
        switch (menu.current) {
            case 0:
                system_runApp(explorer_run);
                break;
            case 1:
                system_runApp(settings_run);
                break;
            default:
                return;
        }
    }
}

static void tab_games() {
    const char* strs[] = {"snake", "pong", "cave", "< back"};
    
    struct menuState menu = {
        .title = "games",
        .pointsCount = C_SIZE(strs),
        .points = strs
    };
    
    while (true) {
        gui_menu(&menu);
        switch (menu.current) {
            case 0:
                system_runApp(snake_run);
                break;
            case 1:
                system_runApp(pong_run);
                break;
            case 2:
                system_runApp(cave_run);
                break;
            default:
                return;
        }
    }
}

static void tab_tests() {
    const char* strs[] = {"screen test", "keyboard test", "gui test", "< back"};
    
    struct menuState menu = {
        .title = "tests",
        .pointsCount = C_SIZE(strs),
        .points = strs
    };
    
    while (true) {
        gui_menu(&menu);
        switch (menu.current) {
            case 0:
                system_runApp(screentest_run);
                break;
            case 1:
                system_runApp(keyboardtest_run);
                break;
            case 2:
                system_runApp(guitest_run);
                break;
            default:
                return;
        }
    }
}

static bool _checker() {
    gui_popUpMenu();
    control_begin();
    return false;
}

void shell_run() {
    const char* strs[] = {"apps", "games", "tests"};
    
    struct menuState menu = {
        .title = SHELL_TITLE,
        .pointsCount = C_SIZE(strs),
        .points = strs,
        .alwaysRedraw = true,
        .checker = _checker
    };
    
    while (true) {
        gui_menu(&menu);
        switch (menu.current) {
            case 0:
                tab_apps();
                break;
            case 1:
                tab_games();
                break;
            case 2:
                tab_tests();
                break;
        }
    }
}