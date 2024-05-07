#pragma once
#include <stdint.h>
#include <stdbool.h>

#define sys_var_count 3
typedef enum {
    sys_inited_screen,
    sys_inited_leds,
    sys_inited_keyboard
} sys_var;

extern uint32_t system_vars[];
extern uint8_t system_debugMode;

void system_printVars();
void system_reset();
void* system_beforeApp();
void system_afterApp(void*);
void system_runApp(void(*app)());
bool system_isLittleEndian();
void system_xApp(int32_t stack, uint8_t fps, uint8_t tps, void(*draw)(int, float, void*), bool(*tick)(int, float, void*), void*);
uint32_t system_uptime();
esp_err_t system_init();