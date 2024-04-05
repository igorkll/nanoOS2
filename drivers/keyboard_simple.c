#include "../main/main.h"
#include "../main/color.h"
#include "../main/drivers/keyboard.h"

#ifndef KEYBOARD_ROTATE
    #define KEYBOARD_ROTATE 0
#endif

#ifndef KEYBOARD_ACTIONS
    #define KEYBOARD_ACTIONS {}
#endif

#ifdef KEYBOARD_HIGH_LEVEL
    #define localPin pin_down

    bool static _get(int gpio) {
        return gpio_get_level(gpio);
    }
#else
    #define localPin pin_up

    bool static _get(int gpio) {
        return !gpio_get_level(gpio);
    }
#endif

uint8_t actionsPins[] = KEYBOARD_ACTIONS;
uint8_t actionsCount = sizeof(actionsPins);

esp_err_t keyboard_init() {
    #ifdef KEYBOARD_UP
        localPin(KEYBOARD_UP, GPIO_MODE_DEF_INPUT);
    #endif

    #ifdef KEYBOARD_RIGHT
        localPin(KEYBOARD_RIGHT, GPIO_MODE_DEF_INPUT);
    #endif
    
    #ifdef KEYBOARD_DOWN
        localPin(KEYBOARD_DOWN, GPIO_MODE_DEF_INPUT);
    #endif
    
    #ifdef KEYBOARD_LEFT
        localPin(KEYBOARD_LEFT, GPIO_MODE_DEF_INPUT);
    #endif

    for (uint8_t i = 0; i < actionsCount; i++) {
        localPin(actionsPins[i], GPIO_MODE_DEF_INPUT);
    }
    
    return ESP_OK;
}

bool keyboard_isMoveButton(uint8_t index) {
    index = (index + KEYBOARD_ROTATE) % 4;
    if (index == 0) {
        #ifdef KEYBOARD_UP
            return _get(KEYBOARD_UP);
        #endif
    } else if (index == 1) {
        #ifdef KEYBOARD_RIGHT
            return _get(KEYBOARD_RIGHT);
        #endif
    } else if (index == 2) {
        #ifdef KEYBOARD_DOWN
            return _get(KEYBOARD_DOWN);
        #endif
    } else if (index == 3) {
        #ifdef KEYBOARD_LEFT
            return _get(KEYBOARD_LEFT);
        #endif
    }
    return false;
}

bool keyboard_isActionButton(uint8_t index) {
    if (index >= actionsCount) return false;
    return _get(actionsPins[index]);
}

bool keyboard_isEnter() {
    #ifdef KEYBOARD_ENTER
        return _get(KEYBOARD_ENTER);
    #else
        return false;
    #endif
}

bool keyboard_isEsc() {
    #ifdef KEYBOARD_ESC
        return _get(KEYBOARD_ESC);
    #else
        return false;
    #endif
}