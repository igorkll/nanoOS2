#define KEYBOARD_X 4
#define KEYBOARD_INPUTS {0, 9, 11, 6}
#define KEYBOARD_Y 4
#define KEYBOARD_OUTPUTS {1, 2, 3, 10}

// --------------------------------

#include "../main.h"
#include "keyboard.h"

uint8_t debounce[KEYBOARD_X][KEYBOARD_Y];
bool states[KEYBOARD_X][KEYBOARD_Y];
static bool keyboard_get(int x, int y) { //получить состояния кнопки по координате
    int inputs[] = KEYBOARD_INPUTS;
    int outputs[] = KEYBOARD_OUTPUTS;

    gpio_set_level(outputs[y], false);
    bool state = !gpio_get_level(inputs[x]);
    gpio_set_level(outputs[y], true);

    if (state) {
        if (debounce[x][y] < 255) debounce[x][y]++;
    } else {
        if (debounce[x][y] > 0) debounce[x][y]--;
    }

    int val = debounce[x][y];
    if (val == 0) {
        states[x][y] = false;
    } else if (val == 255) {
        states[x][y] = true;
    }
    return states[x][y];
}

// -------------------------------- API

esp_err_t keyboard_init() {
    int inputs[] = KEYBOARD_INPUTS;
    int outputs[] = KEYBOARD_OUTPUTS;

    esp_err_t lastError = ESP_OK;

    for (int i = 0; i < C_SIZE(inputs); i++) {
        esp_err_t err = pin_up(inputs[i], GPIO_MODE_DEF_INPUT);
        if (err != ESP_OK) {
            lastError = err;
        }
    }

    for (int i = 0; i < C_SIZE(outputs); i++) {
        esp_err_t err = pin(outputs[i], GPIO_MODE_DEF_OUTPUT);
        gpio_set_level(outputs[i], true);
        if (err != ESP_OK) {
            lastError = err;
        }
    }

    return lastError;
}

bool keyboard_isChar(char chr) {
    return false;
}

bool keyboard_isMoveButton(uint8_t index) {
    if (index == 0) {
        return keyboard_get(1, 0);
    } else if (index == 1) {
        return keyboard_get(2, 1);
    } else if (index == 2) {
        return keyboard_get(1, 2);
    } else if (index == 3) {
        return keyboard_get(0, 1);
    }
    return false;
}


bool keyboard_isActionButton(uint8_t index) {
    return keyboard_get(3, index);
}

bool keyboard_isEsc() {
    return keyboard_get(0, 3);
}

bool keyboard_isEnter() {
    return keyboard_get(1, 1);
}