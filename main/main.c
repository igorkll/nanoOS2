#include "all.h"

#ifdef SYSTEM_CUSTOMSHELL
    #include "customshell.h"
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

#ifndef SYSTEM_DISABLELOGO
static void logo() {
    uint32_t* img = graphic_loadImage("logo.bmp");
    graphic_clear(color_black);
    if (img != NULL) {
        graphic_setXYCloserTo(img[0], img[1]);
        graphic_draw(graphic_centerX(img[0]), graphic_centerY(img[1]), img);
        free(img);
        graphic_resetCrop();
    } else {
        graphic_drawCenterTextBox(0, 0, graphic_x(), graphic_y(), "LOGO\nPROBLEM", color_red);
    }
    graphic_update();
}
#endif

void app_main() {
    // peripheral init
    init("screen", screen_init, sys_inited_screen);
    init("keyboard", keyboard_init, sys_inited_keyboard);
    system_setDebug(keyboard_isEnter());
    init("filesystem", filesystem_init, -1);
    #ifndef SYSTEM_DISABLELOGO
        uint32_t logoTime = uptime();
        logo();
    #endif
    init("leds", leds_init, sys_inited_leds);
    init("base", function_init, -1);
    init("nvs", nvs_init, -1);
    init("wifi", wifi_init, -1);

    // print vars
    system_printVars();

    // logo wait
    #ifndef SYSTEM_DISABLELOGO
        int waitTime = 3000 - (uptime() - logoTime);
        if (waitTime > 0) waitUntil(waitTime, control_needExitOrEnter);
    #endif

    // menu
    #ifdef SYSTEM_CUSTOMSHELL
        customshell_run();
    #else
        shell_run();
    #endif
}