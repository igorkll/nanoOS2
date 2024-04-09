#include "shell.h"
#include "../all.h"

#ifndef SHELL_TITLE
    #define SHELL_TITLE "nanoOS"
#endif

static void tab_games() {
    char* strs[] = {"snake", "pong", "cave", "exit"};
    
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
            case 3:
                return;
        }
    }
}

static void tab_tests() {
    char* strs[] = {"screen test", "keyboard test", "exit"};
    
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
                return;
        }
    }
}

void shell_run() {
    graphic_drawImage(0, 0, "/storage/.test/1.bmp");
    graphic_update();
    wait(1000);

    char* strs[] = {"games", "tests"};
    
    struct menuState menu = {
        .title = SHELL_TITLE,
        .pointsCount = C_SIZE(strs),
        .points = strs
    };
    
    while (true) {
        gui_menu(&menu);
        switch (menu.current) {
            case 0:
                tab_games();
                break;
            case 1:
                tab_tests();
                break;
        }
    }
}