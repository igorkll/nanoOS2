#include <stdint.h>

struct Button {
    uint32_t pressTime;
    uint16_t debounce;
    uint32_t changeTime;
    bool realState;
    bool state;
};

uint8_t hardware_newLed(uint8_t pin);
struct Button hardware_newButton(uint16_t debounce);
int8_t hardware_checkButton(struct Button*, bool state);