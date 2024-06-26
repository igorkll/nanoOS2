#include "all.h"

#ifdef SYSTEM_CUSTOMSHELL
    #include "customshell.h"
#endif

static void init(const char* title, esp_err_t(*func)(), sys_var var) {
    printf("-------- init %s\n", title);
    esp_err_t result = func();
    if (var >= 0) system_vars[var] = result;
    if (result == ESP_OK) {
        printf("successfully\n");
    } else {
        printf("error: %s\n", esp_err_to_name(result));
    }
    printf("--------\n\n");
}

static void updateState() {
    if (control_rawGet(CONTROL_UP)) {
        graphic_setPreprocessor(graphic_preprocessor_monochrome);
    } else if (control_rawGet(CONTROL_DOWN)) {
        graphic_setPreprocessor(graphic_preprocessor_blackwhite);
    }

    if (control_rawGet(CONTROL_ENTER)) system_debugMode = 1;
}

void app_main() {
    init("system", system_init, -1);
    init("screen", screen_init, sys_inited_screen);
    init("keyboard", keyboard_init, sys_inited_keyboard);
    init("filesystem", filesystem_init, -1);
    updateState();
    #ifndef SYSTEM_DISABLELOGO
        bool logoDrawed = false;
        uint32_t logoTime = system_uptime();
        if (viewer_draw("logo.bmp")) {
            logoDrawed = true;
            graphic_update();
        }
    #endif
    storage_sysconf_load();
    init("leds", leds_init, sys_inited_leds);
    init("device", device_init, -1);
    init("nvs", nvs_init, -1);
    init("wifi", wifi_init, -1);

    // print vars
    system_printVars();

    // logo wait
    #ifndef SYSTEM_DISABLELOGO
        if (logoDrawed) {
            int waitTime = 3000 - (system_uptime() - logoTime);
            if (waitTime > 0) waitUntilWithControlBegin(waitTime, control_needExitOrEnter);
        }
    #endif

    // menu
    #ifdef SYSTEM_CUSTOMSHELL
        customshell_run();
    #else
        shell_run();
    #endif
}