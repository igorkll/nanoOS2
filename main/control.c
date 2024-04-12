#include "main.h"
#include "control.h"
#include "hardware.h"
#include "gui.h"
#include "drivers/keyboard.h"

static struct Button buttons[CONTROL_COUNT];
static bool initedButtons[CONTROL_COUNT];
int8_t control_get(control_keys key) {
    if (!initedButtons[key]) {
        buttons[key] = hardware_newButton(50);
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

bool control_getState(control_keys key) {
    return control_get(key) > 0;
}

bool control_isPressed(control_keys key) {
    return control_get(key) == 1;
}

bool control_isPressedExtended(control_keys key) {
    int8_t delta = control_get(key);
    return delta == 1 || delta == 3;
}

bool control_isReleased(control_keys key) {
    return control_get(key) == -1;
}





// default methods
bool control_isEnter() {
    return control_get(CONTROL_ENTER);
}

bool control_isMoveButton(int index) {
    return control_get(index);
}

// pressed methods
static bool enterState = false;
bool control_isEnterPressed() {
    bool currentState = control_isEnter();
    bool resultState = !enterState && currentState;
    enterState = currentState;
    return resultState;
}

static bool moveState[4];
static long holdTime[4];
static long holdTimer[4];
bool control_isMoveButtonPressed(int index) {
    bool currentState = control_isMoveButton(index);
    bool resultState = !moveState[index] && currentState;
    moveState[index] = currentState;
    if (currentState && !resultState) {
        uint32_t t = uptime();
        if (holdTime[index] >= 0) {
            if (t - holdTime[index] > 500) {
                if (holdTimer[index] < 0 || t - holdTimer[index] > 200) {
                    holdTimer[index] = t;
                    return true;
                }
            }
        } else {
            holdTime[index] = t;
            holdTimer[index] = -1;
        }
    } else {
        holdTime[index] = -1;
    }
    return resultState;
}

// other
bool control_needExitWithoutGui() {
    return control_isEnterPressed(); //There is no support for processing other buttons yet
}

bool control_needExit() {
    if (control_needExitWithoutGui()) {
        return gui_exitQuestion();
    }
    return false;
}

void control_waitExit() {
    while (!control_needExit()) yield();
}

void control_waitEnter() {
    while (!control_isEnterPressed()) yield();
}