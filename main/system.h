#include <stdint.h>
#include <stdbool.h>

#define sys_var_count 3
typedef enum {
    sys_inited_screen,
    sys_inited_leds,
    sys_inited_keyboard
} sys_var;

uint32_t system_getVar(sys_var index);
void system_setVar(sys_var index, uint32_t value);
void system_printVars();
void system_runApp(void(*app)());
bool system_isLittleEndian();
bool system_isDebug();
void system_setDebug(bool state);
void system_xApp(int stack, int fps, int tps, void(*draw)(int, float, void*), bool(*tick)(int, float, void*), void*);