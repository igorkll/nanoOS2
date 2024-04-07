#include "main.h"
#include "system.h"

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