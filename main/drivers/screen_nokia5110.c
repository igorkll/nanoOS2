#define SCREEN_RST  7
#define SCREEN_DC   8
#define SCREEN_DIN  5
#define SCREEN_CLK  4

#define SCREEN_RESX 84
#define SCREEN_RESY 48
#define SCREEN_BUFFSIZE (SCREEN_RESX * SCREEN_RESY) / 8

// --------------------------------

#include "../main.h"
#include "screen.h"

uint8_t current_buffer[SCREEN_BUFFSIZE];
uint8_t new_buffer[SCREEN_BUFFSIZE];
bool _screen_firstUpdate = true;

static void _screen_send(bool mode, uint8_t value) {
    gpio_set_level(SCREEN_DC, mode);
    for (int8_t i = 7; i >= 0; i--) {
        int num = pow(2, i);
        gpio_set_level(SCREEN_DIN, (value & num) == num);
        gpio_set_level(SCREEN_CLK, 1);
        gpio_set_level(SCREEN_CLK, 0);
    }
}

// -------------------------------- API

int screen_x() {
    return SCREEN_RESX;
}

int screen_y() {
    return SCREEN_RESY;
}

void screen_set(int x, int y, uint32_t color) {
    uint8_t bytepos = y % 8;
    int index = x + ((y / 8) * SCREEN_RESX);

    if (color == 0) { //если цвет 0(тоесть полностью черный) значит тут нужно поставить пиксель
        new_buffer[index] = new_buffer[index] | (1 << bytepos); //включить
    } else { //а если есть хоть какой-то цвет, значит не нужно
        new_buffer[index] = new_buffer[index] & ~(1 << bytepos); //выключить
    }
}

void screen_update() {
    bool sendPos = true;
    for (int i = 0; i < SCREEN_BUFFSIZE; i++) {
        if (new_buffer[i] != current_buffer[i] || _screen_firstUpdate) {
            int posX = i % SCREEN_RESX;
            int posY = i / SCREEN_RESX;

            if (sendPos) {
                _screen_send(false, 0x80 | posX);
                _screen_send(false, 0x40 | posY);
                sendPos = false;
            }
            _screen_send(true, new_buffer[i]);

            current_buffer[i] = new_buffer[i];
        } else {
            sendPos = true;
        }
    }
    _screen_send(false, 0); //последний байт не обновлялся, это кастыль для решения
}

esp_err_t screen_init() {
    pin(SCREEN_RST, GPIO_MODE_DEF_OUTPUT);
    pin(SCREEN_DC , GPIO_MODE_DEF_OUTPUT);
    pin(SCREEN_DIN, GPIO_MODE_DEF_OUTPUT);
    pin(SCREEN_CLK, GPIO_MODE_DEF_OUTPUT);

    gpio_set_level(SCREEN_RST, 0);
    wait(10);
    gpio_set_level(SCREEN_RST, 1);
    wait(10);

    _screen_send(false, 0x21);
    _screen_send(false, 0x10 + 4); //напряжения макс.7
    _screen_send(false, 0x04 + 1); //контрасность макс.3
    _screen_send(false, 0xB8);
    _screen_send(false, 0x20);
    _screen_send(false, 0x0C);

    screen_update();
    _screen_firstUpdate = false;

    return ESP_OK;
}