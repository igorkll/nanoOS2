#include "../main/main.h"
#include "../main/color.h"
#include "../main/drivers/keyboard.h"

esp_err_t keyboard_init() {
    pin_up(KEYBOARD_UP,     GPIO_MODE_DEF_INPUT);
    pin_up(KEYBOARD_RIGHT,  GPIO_MODE_DEF_INPUT);
    pin_up(KEYBOARD_DOWN,   GPIO_MODE_DEF_INPUT);
    pin_up(KEYBOARD_LEFT,   GPIO_MODE_DEF_INPUT);
    pin_up(KEYBOARD_ACTION, GPIO_MODE_DEF_INPUT);
}

bool keyboard_isMoveButton(uint8_t index) {

}

bool keyboard_isActionButton(uint8_t index);
bool keyboard_isEsc();
bool keyboard_isEnter();