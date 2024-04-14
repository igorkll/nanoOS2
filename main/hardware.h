#include <stdint.h>

struct Button {
    uint32_t pressTime;
    uint32_t changeTime;
    bool realState;
    bool state;
};

uint8_t hardware_newLed(uint8_t pin);
struct Button hardware_newButton();
int8_t hardware_checkButton(struct Button*, bool state);