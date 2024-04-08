#include "main.h"
#include "system.h"
#include "graphic.h"

static uint32_t vars[sys_var_count];

uint32_t system_getVar(sys_var index) {
    return vars[index];
}

void system_setVar(sys_var index, uint32_t value) {
    vars[index] = value;
}

void system_printVars() {
    printf("-------- vars\n");
    for (int i = 0; i < sys_var_count; i++) {
        printf("var %i: %li\n", i, system_getVar(i));
    }
    printf("--------\n\n");
}

void system_runApp(void(*app)()) {
    uint8_t cropX = graphic_getCropX();
    uint8_t cropY = graphic_getCropY();
    graphic_resetCrop();
    app();
    graphic_setCropXY(cropX, cropY);
}