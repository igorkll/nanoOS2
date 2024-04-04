#include "../main/main.h"
#include "../main/drivers/keyboard.h"

unsigned long debounce[KEYBOARD_X][KEYBOARD_Y];
bool states[KEYBOARD_X][KEYBOARD_Y];
bool lastStates[KEYBOARD_X][KEYBOARD_Y];
static bool keyboard_get(int x, int y) { //получить состояния кнопки по координате
    if (x < 0 || y < 0 || x >= KEYBOARD_X || y >= KEYBOARD_Y) return false;

    int inputs[] = KEYBOARD_INPUTS;
    int outputs[] = KEYBOARD_OUTPUTS;

    gpio_set_level(outputs[y], false);
    bool state = !gpio_get_level(inputs[x]);
    gpio_set_level(outputs[y], true);

    unsigned long currentUptime = uptime();
    if (lastStates[x][y] != state) {
        lastStates[x][y] = state;

        if (currentUptime - debounce[x][y] > 50) {
            states[x][y] = state;
        }
        debounce[x][y] = currentUptime;
    } else if (currentUptime - debounce[x][y] > 50) {
        states[x][y] = state;
        debounce[x][y] = currentUptime;
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