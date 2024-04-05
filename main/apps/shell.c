#include "shell.h"
#include "../all.h"

void menu_wifi() {
    gui_splash("wifi");
}

void menu_explorer() {
    gui_splash("explorer");
}

void shell_run() {
    char* strs[] = {"wifi", "explorer", "snake", "pong", "screen test", "keyboard test"};
    
    struct menuState menu = {
        .title = "nanoOS",
        .pointsCount = C_SIZE(strs),
        .points = strs
    };
    
    while (true) {
        int num = gui_menu(&menu);
        switch (menu.current) {
            case 0:
                menu_wifi();
                break;
            case 1:
                menu_explorer();
                break;
            case 2:
                snake_run();
                break;
            case 3:
                pong_run();
                break;
            case 4:
                screentest_run();
                break;
            case 5:
                keyboardtest_run();
                break;
        }
    }
}