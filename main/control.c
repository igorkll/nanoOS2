#include "main.h"
#include "control.h"
#include "drivers/keyboard.h"

bool enterState = false;
bool control_isEnter() {
    bool currentState = keyboard_isEnter();
    bool resultState = !enterState && currentState;
    enterState = currentState;
    return resultState;
}

bool moveState[4];
long holdTime[4];
long holdTimer[4];
bool control_isMoveButton(int index) {
    bool currentState = keyboard_isMoveButton(index);
    bool resultState = !moveState[index] && currentState;
    moveState[index] = currentState;
    if (currentState && !resultState) {
        unsigned long t = uptime();
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

void control_waitEnter() {
    while (!control_isEnter()) yield();
}