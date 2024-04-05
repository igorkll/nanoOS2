#include "all.h"

#ifdef SYSTEM_CUSTOMSHELL
    //yes, I'll just connect the shell in the dumbest way
    //this approach will allow you to use all the system APIs from there without having to call include,
    //since all include will already be called before loading customshell
    #include "../profiles/_current/shell.c"
#endif

void init(const char* title, esp_err_t(*func)()) {
    printf("-------- init %s\n", title);
    esp_err_t result = func();
    if (result == ESP_OK) {
        printf("successfully\n");
    } else {
        printf("error: %s", esp_err_to_name(result));
        ESP_ERROR_CHECK_WITHOUT_ABORT(result);
    }
    printf("--------\n\n");
}

void app_main() {
    // init
    init("base", function_init);
    init("leds", leds_init);
    init("screen", screen_init);
    init("filesystem", filesystem_init);
    init("keyboard", keyboard_init);
    init("nvs", nvs_init);
    init("wifi", wifi_init);

    // menu
    #ifdef SYSTEM_CUSTOMSHELL
        customshell_run();
    #else
        shell_run();
    #endif
}