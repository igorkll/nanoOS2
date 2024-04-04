#include "../main/main.h"
#include "../main/color.h"
#include "../main/drivers/keyboard.h"

#ifndef KEYBOARD_ROTATE
#define KEYBOARD_ROTATE 0
#endif

esp_err_t keyboard_init() {
    pin_up(KEYBOARD_UP,     GPIO_MODE_DEF_INPUT);
    pin_up(KEYBOARD_RIGHT,  GPIO_MODE_DEF_INPUT);
    pin_up(KEYBOARD_DOWN,   GPIO_MODE_DEF_INPUT);
    pin_up(KEYBOARD_LEFT,   GPIO_MODE_DEF_INPUT);
    pin_up(KEYBOARD_ACTION, GPIO_MODE_DEF_INPUT);
}

bool keyboard_isMoveButton(uint8_t index) {
    index = (index + KEYBOARD_ROTATE) % 4;
    if (index == 0) {
        return !gpio_get_level(KEYBOARD_UP);
    } else if (index == 1) {
        return !gpio_get_level(KEYBOARD_RIGHT);
    } else if (index == 2) {
        return !gpio_get_level(KEYBOARD_DOWN);
    } else if (index == 3) {
        return !gpio_get_level(KEYBOARD_LEFT);
    }
    return false;
}

bool keyboard_isActionButton(uint8_t index) {
    return keyboard_isEnter();
}

bool keyboard_isEnter() {
    return !gpio_get_level(KEYBOARD_ACTION);
}