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

static void logo() {
    #ifndef SYSTEM_DISABLELOGO
        const char* logoPath;
        if (graphic_isColor()) {
            logoPath = "/storage/logo_c.bmp";
        } else {
            logoPath = "/storage/logo_w.bmp";
        }
        uint32_t* img = graphic_loadImage(logoPath);
        graphic_clear(color_black);
        if (img != NULL) {
            graphic_draw(graphic_centerX(img[0]), graphic_centerY(img[1]), img);
            free(img);
        } else {
            graphic_drawConterTextBox(0, 0, graphic_x(), graphic_y(), "LOGO\nPROBLEM", color_red);
        }
        graphic_update();
        wait(100);
    #endif
}

void app_main() {
    // peripheral init
    init("screen", screen_init, sys_inited_screen);
    init("filesystem", filesystem_init, -1);
    logo();
    init("keyboard", keyboard_init, sys_inited_keyboard);
    init("leds", leds_init, sys_inited_leds);
    init("base", function_init, -1);
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