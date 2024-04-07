#include "../../../main.h"
#include "../../../hardware.h"
#include "../../screen.h"
#include <driver/ledc.h>

static int bl_channel = -1;
void screen_setBacklightValue(uint8_t value) {
    // standard code backlight control
    #ifdef SCREEN_BL
        if (bl_channel < 0) bl_channel = hardware_newLed(SCREEN_BL);
        if (bl_channel >= 0) {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, bl_channel, CRTValue(value));
            ledc_update_duty(LEDC_LOW_SPEED_MODE, bl_channel);
        }
    #endif
}