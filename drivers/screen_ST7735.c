// supported chips: ST7735S

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

#ifndef SCREEN_OFFSET_X
    #define SCREEN_OFFSET_X 0
#endif

#ifndef SCREEN_OFFSET_Y
    #define SCREEN_OFFSET_Y 0
#endif

uint8_t buffer[SCREEN_RESX * SCREEN_RESY * 2];

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

void sendData(const void* data, int len) {
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

void sendCmdArg(uint8_t cmd, uint8_t arg) {
    sendCmd(cmd);
    sendDataByte(arg);
}

void sendCmdArgs(uint8_t cmd, const void* data, int len) {
    sendCmd(cmd);
    sendData(data, len);
}

// --------------------------------

screen_colormode screen_getColormode() {
    return screen_colored;
}

uint32_t screen_get(int x, int y) {
    return 0;
}

void screen_set(int x, int y, uint32_t color) {
    uint16_t color565 = color_to565(color);
    #ifdef SCREEN_COLORFIX
        color565 = 0xffff - color565;
    #endif
    int index = (x + (y * SCREEN_RESX)) * 2;
    buffer[index] = color565 >> 8;
    buffer[index+1] = color565 % 256;
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


void static _init() {
    sendCmdArg(0x36, 0b01101000); //Memory data access control
    sendCmdArg(0x3A, 5); //Color mode
    sendCmd(0x11); //Sleep out
    sendCmd(0x29); //Display on
}

void static _select(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
  uint8_t COL_START = 0  + SCREEN_OFFSET_X;
  uint8_t ROW_START = 24 + SCREEN_OFFSET_Y;
  uint8_t args[4];

  args[0] = 0;
  args[1] = COL_START + x;
  args[2] = 0;
  args[3] = COL_START + x + w - 1;
  sendCmdArgs(0x2A, args, sizeof(args)); // CASET

  args[1] = ROW_START + y;
  args[3] = ROW_START + y + h - 1;
  sendCmdArgs(0x2B, args, sizeof(args)); // RASET
}


void static _setup() {
    _select(0, 0, SCREEN_RESX, SCREEN_RESY);
    sendCmd(0x2C);
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

    // screen init
    #ifdef SCREEN_RST
        pin(SCREEN_RST, GPIO_MODE_DEF_OUTPUT);
        gpio_set_level(SCREEN_RST, 0);
        wait(50);
        gpio_set_level(SCREEN_RST, 1);
    #else
        sendCmd(0x01);
    #endif
    wait(50);
    _init();
    _setup();

    return ret;
}