#define SCREEN_RST  7
#define SCREEN_DC   8
#define SCREEN_DIN  5
#define SCREEN_CLK  4

#define SCREEN_RESX 84
#define SCREEN_RESY 48

#define gridientSupport

// --------------------------------

#include "../main.h"
#include "screen.h"

#define SCREEN_BITBUFFSIZE ((SCREEN_RESX * SCREEN_RESY) / 8)
#ifdef gridientSupport
    #define SCREEN_BUFFSIZE ((SCREEN_RESX * SCREEN_RESY) / 2)
#else
    #define SCREEN_BUFFSIZE SCREEN_BITBUFFSIZE
#endif

uint8_t new_buffer[SCREEN_BUFFSIZE]; //вы ресуете в этом буфере
uint8_t current_buffer[SCREEN_BUFFSIZE]; //то что реально сейчас на экране
bool _screen_firstUpdate = true;

#ifdef gridientSupport
uint8_t newbit_buffer[SCREEN_BITBUFFSIZE];
uint8_t bit_buffer[SCREEN_BITBUFFSIZE];
#endif

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

#ifdef gridientSupport

void screen_set(int x, int y, uint32_t color) {
    uint8_t col = color_getGray(color) / 17;
    int index = x + ((y / 2) * SCREEN_RESX);
    bool shift = y % 2 == 1;

    for (int i = 0; i < 4; i++) {
        bool offset = i + (shift ? 4 : 0);
        if ((col >> i) % 2 == 1) {
            new_buffer[index] = new_buffer[index] | (1 << offset);
        } else {
            new_buffer[index] = new_buffer[index] & ~(1 << offset);
        }
    }
}

void screen_update() {
    for (int i = 0; i < SCREEN_BUFFSIZE; i++) {
        current_buffer[i] = new_buffer[i];
    }
}

uint8_t count = 1; //диапозон 1-15
void screen_tick() {
    for (int ix = 0; ix < SCREEN_RESX; ix++) {
        for (int iy = 0; iy < SCREEN_RESY; iy++) {
            int index = ix + ((iy / 2) * SCREEN_RESX);
            uint8_t col;
            if (iy % 2 == 1) {
                col = current_buffer[index] >> 4;
            } else {
                col = current_buffer[index] % 16;
            }
            
            uint8_t bytepos = iy % 8;
            index = ix + ((iy / 8) * SCREEN_RESX);
            if (col < count) {
                newbit_buffer[index] = newbit_buffer[index] | (1 << bytepos);
            } else {
                newbit_buffer[index] = newbit_buffer[index] & ~(1 << bytepos);
            }
        }
    }

    bool sendPos = true;
    for (int i = 0; i < SCREEN_BUFFSIZE; i++) {
        if (newbit_buffer[i] != bit_buffer[i] || _screen_firstUpdate) {
            int posX = i % SCREEN_RESX;
            int posY = i / SCREEN_RESX;

            if (sendPos) {
                _screen_send(false, 0x80 | posX);
                _screen_send(false, 0x40 | posY);
                sendPos = false;
            }
            _screen_send(true, newbit_buffer[i]);

            bit_buffer[i] = newbit_buffer[i];
        } else {
            sendPos = true;
        }
    }
    _screen_send(false, 0);

    count++;
    if (count > 15) {
        count = 0;
    }
}

bool screen_needTick() {
    return true;
}

#else

void screen_set(int x, int y, uint32_t color) {
    uint8_t bytepos = y % 8;
    int index = x + ((y / 8) * SCREEN_RESX);
    if (index < 0 || index >= SCREEN_BUFFSIZE) return;

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

void screen_tick() {}
bool screen_needTick() { return false; }

#endif

// -------------------------------- API

int screen_x() {
    return SCREEN_RESX;
}

int screen_y() {
    return SCREEN_RESY;
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
    _screen_send(false, 0x04 + 0); //контрасность макс.3
    _screen_send(false, 0xB8);
    _screen_send(false, 0x20);
    _screen_send(false, 0x0C);

    screen_update();
    #ifdef gridientSupport
        screen_tick();
    #endif
    _screen_firstUpdate = false;

    return ESP_OK;
}