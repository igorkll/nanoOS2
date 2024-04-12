#include <stdint.h>
#include <esp_err.h>

esp_err_t keyboard_init();
bool keyboard_isChar(char chr);
bool keyboard_isMoveButton(uint8_t index);
bool keyboard_isActionButton(uint8_t index);
bool keyboard_isEsc();
bool keyboard_isEnter();

uint8_t keyboard_getActionsCount();
bool keyboard_isCharSupport(char chr);
bool keyboard_isEnterSupport();
bool keyboard_isEscSupport();
bool keyboard_isMoveSupport(uint8_t index);