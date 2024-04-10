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

#ifndef SYSTEM_DISABLELOGO
static void logo() {
    const char* logoPath;
    if (graphic_isColor()) {
        logoPath = "/storage/logo_c.bmp";
    } else {
        logoPath = "/storage/logo_w.bmp";
    }
    void* img = graphic_loadImage(logoPath);
    graphic_clear(color_black);
    if (img != NULL) {
        graphic_setYCloserTo(graphic_getDumpY(img));
        graphic_draw(graphic_centerX(graphic_getDumpX(img)), graphic_centerY(graphic_getDumpY(img)), img);
        free(img);
        graphic_resetCrop();
    } else {
        graphic_drawConterTextBox(0, 0, graphic_x(), graphic_y(), "LOGO\nPROBLEM", color_red);
    }
    graphic_update();
}
#endif

void app_main() {
    // peripheral init
    init("screen", screen_init, sys_inited_screen);
    init("filesystem", filesystem_init, -1);
    #ifndef SYSTEM_DISABLELOGO
        uint32_t logoTime = uptime();
        logo();
    #endif
    init("keyboard", keyboard_init, sys_inited_keyboard);
    init("leds", leds_init, sys_inited_leds);
    init("base", function_init, -1);
    init("nvs", nvs_init, -1);
    init("wifi", wifi_init, -1);

    // print vars
    system_printVars();

    // logo wait
    #ifndef SYSTEM_DISABLELOGO
        int waitTime = 3000 - (uptime() - logoTime);
        if (waitTime > 0) wait(waitTime);
    #endif

    // menu
    #ifdef SYSTEM_CUSTOMSHELL
        customshell_run();
    #else
        shell_run();
    #endif
}