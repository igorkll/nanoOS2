#include "all.h"

#ifdef SYSTEM_CUSTOMSHELL
    //yes, I'll just connect the shell in the dumbest way
    //this approach will allow you to use all the system APIs from there without having to call include,
    //since all include will already be called before loading customshell
    #include "../profiles/_current/shell.c"
#endif

static void init(const char* title, esp_err_t(*func)(), sys_var var) {
    printf("-------- init %s\n", title);
    esp_err_t result = func();
    if (var >= 0) system_setVar(var, result);
    if (result == ESP_OK) {
        printf("successfully\n");
    } else {
        printf("error: %s\n", esp_err_to_name(result));
    }
    printf("--------\n\n");
}

void app_main() {
    // peripheral init
    init("screen", screen_init, sys_inited_screen);
    init("keyboard", keyboard_init, sys_inited_keyboard);
    init("leds", leds_init, sys_inited_leds);

    // base init
    init("base", function_init, -1);
    init("filesystem", filesystem_init, -1);
    init("nvs", nvs_init, -1);
    init("wifi", wifi_init, -1);

    // print vars
    system_printVars();

    // menu
    #ifdef SYSTEM_CUSTOMSHELL
        customshell_run();
    #else
        shell_run();
    #endif
}