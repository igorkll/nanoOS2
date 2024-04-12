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
    printf("objs %i\n", filesystem_objCount("/storage"));
    printf("file %i\n", filesystem_fileCount("/storage"));
    printf("dirs %i\n", filesystem_dirCount("/storage"));

    printf("2.objs %i\n", filesystem_objCount("/storage/cave"));
    printf("2.file %i\n", filesystem_fileCount("/storage/cave"));
    printf("2.dirs %i\n", filesystem_dirCount("/storage/cave"));

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