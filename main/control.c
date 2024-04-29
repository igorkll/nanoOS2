#include "main.h"
#include "control.h"
#include "hardware.h"
#include "gui.h"
#include "drivers/keyboard.h"

static struct Button buttons[CONTROL_COUNT];
static bool initedButtons[CONTROL_COUNT];
int8_t control_get(control_key key) {
    if (!initedButtons[key]) {
        buttons[key] = hardware_newButton();
        initedButtons[key] = true;
    }
    bool state = false;
    switch (key) {
        case CONTROL_UP:
        case CONTROL_RIGHT:
        case CONTROL_DOWN:
        case CONTROL_LEFT:
            state = keyboard_isMoveButton(key);
            break;
        case CONTROL_ENTER:
            state = keyboard_isEnter();
            break;
        case CONTROL_ESC:
            state = keyboard_isEsc();
            break;
    }
    return hardware_checkButton(&buttons[key], state);
}

bool control_isSupport(control_key key) {
    switch (key) {
        case CONTROL_UP:
        case CONTROL_RIGHT:
        case CONTROL_DOWN:
        case CONTROL_LEFT:
            return keyboard_isMoveSupport(key);
        case CONTROL_ENTER:
            return keyboard_isEnterSupport();
        case CONTROL_ESC:
            return keyboard_isEscSupport();
    }
    return false;
}

bool control_getState(control_key key) {
    return control_get(key) > 0;
}

bool control_isPressed(control_key key) {
    return control_get(key) == 1;
}

bool control_isPressedExtended(control_key key) {
    int8_t delta = control_get(key);
    return delta == 1 || delta == 3;
}

bool control_isReleased(control_key key) {
    return control_get(key) == -1;
}

// --------------------------------------------------

bool control_isEnter() {
    return control_getState(CONTROL_ENTER);
}

bool control_isEnterPressed() {
    return control_isPressed(CONTROL_ENTER);
}

bool control_isMoveButton(control_key keys) {
    return control_getState(keys);
}

bool control_isMoveButtonPressed(control_key keys) {
    return control_isPressedExtended(keys);
}

// -------------------------------------------------- other

bool control_needExitWithoutGui() {
    if (control_isSupport(CONTROL_ESC)) {
        return control_isPressed(CONTROL_ESC);
    } else {
        bool state1 = control_getState(CONTROL_ENTER);
        bool state2 = false;
        if (control_isSupport(CONTROL_DOWN)) {
            state2 = control_getState(CONTROL_DOWN);
        } else {
            state2 = control_getState(CONTROL_RIGHT);
        }
        return state1 && state2;
    }
}

bool control_needExit() {
    if (control_needExitWithoutGui()) {
        return gui_exitQuestion();
    }
    return false;
}

void control_waitExit() {
    while (!control_needExitWithoutGui()) yield();
}

void control_waitEnter() {
    while (!control_isEnterPressed()) yield();
}

// -------------------------------------------------- smart control

bool control_pageUp() {
    if (control_isSupport(CONTROL_UP)) {
        return control_isPressedExtended(CONTROL_UP);
    } else {
        return control_isPressedExtended(CONTROL_LEFT);
    }
}

bool control_pageDown() {
    if (control_isSupport(CONTROL_DOWN)) {
        return control_isPressedExtended(CONTROL_DOWN);
    } else {
        return control_isPressedExtended(CONTROL_RIGHT);
    }
}