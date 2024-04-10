#include "../main/main.h"
#include "../main/color.h"
#include "../main/drivers/screen.h"
#include "driver/spi_master.h"

#ifndef SCREEN_RESX
    #define SCREEN_RESX 84
#endif

#ifndef SCREEN_RESY
    #define SCREEN_RESY 48
#endif

#ifndef SCREEN_GRIDIENT_DELAY
    #define SCREEN_GRIDIENT_DELAY  2500
#endif

#ifdef SCREEN_GRIDIENT
    #define SCREEN_DATA_BUFFER_SIZE  ((SCREEN_RESX * SCREEN_RESY) / 4)
    static uint8_t temp_buffer[SCREEN_DATA_BUFFER_SIZE];
    static uint8_t data_buffer[SCREEN_DATA_BUFFER_SIZE];
#endif

#define SCREEN_FLUSH_BUFFER_SIZE ((SCREEN_RESX * SCREEN_RESY) / 8)
static uint8_t flush_buffer[SCREEN_FLUSH_BUFFER_SIZE];

// -------------------------------- SPI

static spi_device_handle_t spi;

static void spi_pre_transfer_callback(spi_transaction_t *t) {
    gpio_set_level(SCREEN_DC, (int)t->user);
}

static void sendCmd(uint8_t cmd) {
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length=8;
    t.tx_buffer=&cmd;
    t.user=(void*)0;
    spi_device_polling_transmit(spi, &t);
}

static void sendData(const uint8_t *data, int len) {
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length=len*8;
    t.tx_buffer=data;
    t.user=(void*)1;
    spi_device_polling_transmit(spi, &t);
}

static void trigger() {
    sendCmd(0);
}

#ifdef SCREEN_GRIDIENT
static void _screen_tick() {
    static int16_t count = 0;

    for (int ix = 0; ix < SCREEN_RESX; ix++) {
        for (int iy = 0; iy < SCREEN_RESY; iy++) {
            int index = ix + ((iy / 4) * SCREEN_RESX);
            uint8_t col = (data_buffer[index] >> ((iy % 4) * 2)) % 4;
            uint8_t bytepos = iy % 8;
            index = ix + ((iy / 8) * SCREEN_RESX);

            bool state;
            if (col == 0) {
                state = true;
            } else if (col == 1) {
                state = count == 0 || count == 3 || count == 6 || count == 9 || count == 12 || count == 1 || count == 7;
            } else if (col == 2) {
                state = count == 0 || count == 3 || count == 6 || count == 9 || count == 12;
            } else if (col == 3) {
                state = false;
            }

            if (state) {
                flush_buffer[index] |= 1 << bytepos;
            } else {
                flush_buffer[index] &= ~(1 << bytepos);
            }
        }
    }

    sendData(flush_buffer, SCREEN_FLUSH_BUFFER_SIZE);
    trigger();
    if (++count > 14) count = 0;
}

screen_colormode screen_getColormode() {
    return screen_monochrome;
}
#else
screen_colormode screen_getColormode() {
    return screen_blackwhite;
}
#endif

// -------------------------------- API

#ifdef SCREEN_GRIDIENT
tcolor screen_get(int x, int y) {
    uint8_t col = 0;
    int index = x + ((y / 4) * SCREEN_RESX);
    int add = y % 4;

    for (int i = 0; i < 2; i++) {
        uint8_t offset = i + (add * 2);
        if ((temp_buffer[index] & (1 << offset)) > 0) {
            col += (1 << i);
        }
    }

    return color_pack(col * 85, col * 85, col * 85);
}

void screen_set(int x, int y, tcolor color) {
    uint8_t col = map(color_getGray(color), 0, 255, 0, 4);
    if (col >= 4) col = 3;

    int index = x + ((y / 4) * SCREEN_RESX);
    int add = y % 4;

    for (int i = 0; i < 2; i++) {
        uint8_t offset = i + (add * 2);
        if ((col >> i) % 2 == 1) {
            temp_buffer[index] |= 1 << offset;
        } else {
            temp_buffer[index] &= ~(1 << offset);
        }
    }
}
#else
void screen_set(int x, int y, tcolor color) {
    uint8_t bytepos = y % 8;
    int index = x + ((y / 8) * SCREEN_RESX);
    if (index < 0 || index >= SCREEN_FLUSH_BUFFER_SIZE) return;

    if (color == 0) { //если цвет 0(тоесть полностью черный) значит тут нужно поставить пиксель
        flush_buffer[index] = flush_buffer[index] | (1 << bytepos); //включить
    } else { //а если есть хоть какой-то цвет, значит не нужно
        flush_buffer[index] = flush_buffer[index] & ~(1 << bytepos); //выключить
    }
}

tcolor screen_get(int x, int y) {
    uint8_t bytepos = y % 8;
    int index = x + ((y / 8) * SCREEN_RESX);

    if ((flush_buffer[index] & (1 << bytepos)) > 0) {
        return color_black;
    } else {
        return color_white;
    }
}
#endif

void screen_update() {
    #ifdef SCREEN_GRIDIENT
        for (int i = 0; i < SCREEN_DATA_BUFFER_SIZE; i++) {
            data_buffer[i] = temp_buffer[i];
        }
    #else
        sendData(flush_buffer, SCREEN_FLUSH_BUFFER_SIZE);
        trigger();
    #endif
}

int screen_x() {
    return SCREEN_RESX;
}

int screen_y() {
    return SCREEN_RESY;
}

esp_err_t screen_init() {
    esp_err_t ret = ESP_OK;

    // SPI init
    spi_bus_config_t buscfg={
        .mosi_io_num=SCREEN_DIN,
        .sclk_io_num=SCREEN_CLK,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1,
        .max_transfer_sz=SCREEN_FLUSH_BUFFER_SIZE
    };
    ret = spi_bus_initialize(SCREEN_SPI, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) return ret;

    // device init
    spi_device_interface_config_t devcfg={
        .clock_speed_hz=SCREEN_SPI_SPEED,
        .mode=0,
        .queue_size=7,
        .pre_cb=spi_pre_transfer_callback,
    };
    #ifdef SCREEN_CS
        pin(SCREEN_CS, GPIO_MODE_DEF_OUTPUT);
        devcfg.spics_io_num=SCREEN_CS;
    #endif
    ret = spi_bus_add_device(SCREEN_SPI, &devcfg, &spi);
    if (ret != ESP_OK) return ret;

    // display init
    pin(SCREEN_DC, GPIO_MODE_DEF_OUTPUT);

    #ifdef SCREEN_RST
        pin(SCREEN_RST, GPIO_MODE_DEF_OUTPUT);
        gpio_set_level(SCREEN_RST, 0);
        wait(50);
        gpio_set_level(SCREEN_RST, 1);
        wait(50);
    #endif

    sendCmd(0x21);
    sendCmd(0x10 + SCREEN_VOLTAGE); //напряжения макс.7
    sendCmd(0x04 + SCREEN_CONTRASTE); //контрасность макс.3
    sendCmd(0xB8);
    sendCmd(0x20);
    sendCmd(0x0C);
    sendCmd(0x80);
    sendCmd(0x40);

    // tick callback
    #ifdef SCREEN_GRIDIENT
        const esp_timer_create_args_t timer_args = {
            .callback = &_screen_tick,
        };
        esp_timer_handle_t timer;
        esp_timer_create(&timer_args, &timer);
        esp_timer_start_periodic(timer, SCREEN_GRIDIENT_DELAY);
    #endif

    return ret;
}