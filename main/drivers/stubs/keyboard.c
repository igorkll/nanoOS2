#include "../../main.h"
#include "../keyboard.h"

esp_err_t keyboard_init() {
    return ESP_OK;
}

bool keyboard_isChar(char chr) {
    return false;
}

bool keyboard_isMoveButton(uint8_t index) {
    return false;
}

bool keyboard_isActionButton(uint8_t index) {
    return false;
}

bool keyboard_isEsc() {
    return false;
}

bool keyboard_isEnter() {
    return false;
}