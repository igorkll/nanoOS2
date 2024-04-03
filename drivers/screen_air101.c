#include "../main/main.h"
#include "../main/color.h"
#include "../main/drivers/screen.h"
#include "driver/spi_master.h"

#ifndef SCREEN_RESX
    #define SCREEN_RESX 160
#endif

#ifndef SCREEN_RESY
    #define SCREEN_RESY 80
#endif

uint16_t buffer[SCREEN_RESX * SCREEN_RESY];

// -------------------------------- SPI

void spi_pre_transfer_callback(spi_transaction_t *t) {
    gpio_set_level(SCREEN_DC, (int)t->user);
}

spi_device_handle_t spi;

void sendCmd(uint8_t cmd) {
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length=8;
    t.tx_buffer=&cmd;
    t.user=(void*)0;
    spi_device_polling_transmit(spi, &t);
}

void sendData(const uint8_t* data, int len) {
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length=len*8;
    t.tx_buffer=data;
    t.user=(void*)1;
    spi_device_polling_transmit(spi, &t);
}

void sendDataByte(uint8_t data) {
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length=8;
    t.tx_buffer=&data;
    t.user=(void*)1;
    spi_device_polling_transmit(spi, &t);
}

// --------------------------------

screen_colormode screen_getColormode() {
    return screen_colored;
}

uint32_t screen_get(int x, int y) {
    return 0;
}

void screen_set(int x, int y, uint32_t color) {
    buffer[x + (y * SCREEN_RESX)] = color_to565(color);
}

void screen_update() {
    sendData(buffer, sizeof(buffer));
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
    pin(SCREEN_DC, GPIO_MODE_DEF_OUTPUT);
    spi_bus_config_t buscfg={
        .mosi_io_num=SCREEN_DIN,
        .sclk_io_num=SCREEN_CLK,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1,
        .max_transfer_sz=sizeof(buffer)
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

    #ifdef SCREEN_RST
        pin(SCREEN_RST, GPIO_MODE_DEF_OUTPUT);
        gpio_set_level(SCREEN_RST, 0);
        wait(100);
        gpio_set_level(SCREEN_RST, 1);
        wait(120);
    #else
        sendCmd(0x01);
        wait(120);
    #endif

    sendCmd(0x11); // SLPOUT
    wait(120);

    sendCmd(0x36); // MADCTL
    sendDataByte(0x68);
    sendCmd(0x3A); // COLMOD (16 bit)
    sendDataByte(5);
    sendCmd(0x13); // NORON

    sendCmd(0x29); // DISPON

    return ret;
}