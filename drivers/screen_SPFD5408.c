// I couldn't get it to work properly.

#include "../main/main.h"
#include "../main/color.h"
#include "../main/util.h"
#include "../main/drivers/screen.h"

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"

#ifndef SCREEN_RESX
    #define SCREEN_RESX 5
#endif

#ifndef SCREEN_RESY
    #define SCREEN_RESY 5
#endif

#ifndef SCREEN_SPEED
    #define SCREEN_SPEED 1000000
#endif

#define BUFFER_SIZE SCREEN_RESX * SCREEN_RESY * 2
static uint8_t buffer[BUFFER_SIZE];
static esp_lcd_i80_bus_handle_t i80_bus = NULL;
static esp_lcd_panel_io_handle_t io_handle = NULL;

// --------------------------------

screen_colormode screen_getColormode() {
    return screen_colored;
}

tcolor screen_get(int x, int y) {
    int index = (x + (y * SCREEN_RESX)) * 2;
    uint16_t color565 = (buffer[index] << 8) + buffer[index+1];
    #ifdef SCREEN_INVERT_COLORS
        color565 = 0xffff - color565;
    #endif
    tcolor color = color_from565(color565);
    #ifdef SCREEN_ALT_COLOR_ORDER
        color = color_swap(color);
    #endif
    return color;
}

void screen_set(int x, int y, tcolor color) {
    #ifdef SCREEN_ALT_COLOR_ORDER
        color = color_swap(color);
    #endif
    uint16_t color565 = color_to565(color);
    #ifdef SCREEN_INVERT_COLORS
        color565 = 0xffff - color565;
    #endif
    int index = (x + (y * SCREEN_RESX)) * 2;
    buffer[index] = color565 >> 8;
    buffer[index+1] = color565 % 256;
}

void screen_update() {
}

int screen_x() {
    return SCREEN_RESX;
}

int screen_y() {
    return SCREEN_RESY;
}

// --------------------------------

#define ILI932X_START_OSC          0x00
#define ILI932X_DRIV_OUT_CTRL      0x01
#define ILI932X_DRIV_WAV_CTRL      0x02
#define ILI932X_ENTRY_MOD          0x03
#define ILI932X_RESIZE_CTRL        0x04
#define ILI932X_DISP_CTRL1         0x07
#define ILI932X_DISP_CTRL2         0x08
#define ILI932X_DISP_CTRL3         0x09
#define ILI932X_DISP_CTRL4         0x0A
#define ILI932X_RGB_DISP_IF_CTRL1  0x0C
#define ILI932X_FRM_MARKER_POS     0x0D
#define ILI932X_RGB_DISP_IF_CTRL2  0x0F
#define ILI932X_POW_CTRL1          0x10
#define ILI932X_POW_CTRL2          0x11
#define ILI932X_POW_CTRL3          0x12
#define ILI932X_POW_CTRL4          0x13
#define ILI932X_GRAM_HOR_AD        0x20
#define ILI932X_GRAM_VER_AD        0x21
#define ILI932X_RW_GRAM            0x22
#define ILI932X_POW_CTRL7          0x29
#define ILI932X_FRM_RATE_COL_CTRL  0x2B
#define ILI932X_GAMMA_CTRL1        0x30
#define ILI932X_GAMMA_CTRL2        0x31
#define ILI932X_GAMMA_CTRL3        0x32
#define ILI932X_GAMMA_CTRL4        0x35
#define ILI932X_GAMMA_CTRL5        0x36
#define ILI932X_GAMMA_CTRL6        0x37
#define ILI932X_GAMMA_CTRL7        0x38
#define ILI932X_GAMMA_CTRL8        0x39
#define ILI932X_GAMMA_CTRL9        0x3C
#define ILI932X_GAMMA_CTRL10       0x3D
#define ILI932X_HOR_START_AD       0x50
#define ILI932X_HOR_END_AD         0x51
#define ILI932X_VER_START_AD       0x52
#define ILI932X_VER_END_AD         0x53
#define ILI932X_GATE_SCAN_CTRL1    0x60
#define ILI932X_GATE_SCAN_CTRL2    0x61
#define ILI932X_GATE_SCAN_CTRL3    0x6A
#define ILI932X_PART_IMG1_DISP_POS 0x80
#define ILI932X_PART_IMG1_START_AD 0x81
#define ILI932X_PART_IMG1_END_AD   0x82
#define ILI932X_PART_IMG2_DISP_POS 0x83
#define ILI932X_PART_IMG2_START_AD 0x84
#define ILI932X_PART_IMG2_END_AD   0x85
#define ILI932X_PANEL_IF_CTRL1     0x90
#define ILI932X_PANEL_IF_CTRL2     0x92
#define ILI932X_PANEL_IF_CTRL3     0x93
#define ILI932X_PANEL_IF_CTRL4     0x95
#define ILI932X_PANEL_IF_CTRL5     0x97
#define ILI932X_PANEL_IF_CTRL6     0x98

#define INIT_DELAY 0xfff1
#define INIT_END   0xfff2

static const uint16_t regValues[] = {
    ILI932X_START_OSC        , 0x0001, // Start oscillator
    INIT_DELAY               , 50,     // 50 millisecond delay
    ILI932X_DRIV_OUT_CTRL    , 0x0100,
    ILI932X_DRIV_WAV_CTRL    , 0x0700,
    ILI932X_ENTRY_MOD        , 0x1030,
    ILI932X_RESIZE_CTRL      , 0x0000,
    ILI932X_DISP_CTRL2       , 0x0202,
    ILI932X_DISP_CTRL3       , 0x0000,
    ILI932X_DISP_CTRL4       , 0x0000,
    ILI932X_RGB_DISP_IF_CTRL1, 0x0,
    ILI932X_FRM_MARKER_POS   , 0x0,
    ILI932X_RGB_DISP_IF_CTRL2, 0x0,
    ILI932X_POW_CTRL1        , 0x0000,
    ILI932X_POW_CTRL2        , 0x0007,
    ILI932X_POW_CTRL3        , 0x0000,
    ILI932X_POW_CTRL4        , 0x0000,
    INIT_DELAY               , 200,
    ILI932X_POW_CTRL1        , 0x1690,
    ILI932X_POW_CTRL2        , 0x0227,
    INIT_DELAY               , 50,
    ILI932X_POW_CTRL3        , 0x001A,
    INIT_DELAY               , 50,
    ILI932X_POW_CTRL4        , 0x1800,
    ILI932X_POW_CTRL7        , 0x002A,
    INIT_DELAY               , 50,
    ILI932X_GAMMA_CTRL1      , 0x0000,
    ILI932X_GAMMA_CTRL2      , 0x0000,
    ILI932X_GAMMA_CTRL3      , 0x0000,
    ILI932X_GAMMA_CTRL4      , 0x0206,
    ILI932X_GAMMA_CTRL5      , 0x0808,
    ILI932X_GAMMA_CTRL6      , 0x0007,
    ILI932X_GAMMA_CTRL7      , 0x0201,
    ILI932X_GAMMA_CTRL8      , 0x0000,
    ILI932X_GAMMA_CTRL9      , 0x0000,
    ILI932X_GAMMA_CTRL10     , 0x0000,
    ILI932X_GRAM_HOR_AD      , 0x0000,
    ILI932X_GRAM_VER_AD      , 0x0000,
    ILI932X_HOR_START_AD     , 0x0000,
    ILI932X_HOR_END_AD       , 0x00EF,
    ILI932X_VER_START_AD     , 0X0000,
    ILI932X_VER_END_AD       , 0x013F,
    ILI932X_GATE_SCAN_CTRL1  , 0xA700, // Driver Output Control (R60h)
    ILI932X_GATE_SCAN_CTRL2  , 0x0003, // Driver Output Control (R61h)
    ILI932X_GATE_SCAN_CTRL3  , 0x0000, // Driver Output Control (R62h)
    ILI932X_PANEL_IF_CTRL1   , 0X0010, // Panel Interface Control 1 (R90h)
    ILI932X_PANEL_IF_CTRL2   , 0X0000,
    ILI932X_PANEL_IF_CTRL3   , 0X0003,
    ILI932X_PANEL_IF_CTRL4   , 0X1100,
    ILI932X_PANEL_IF_CTRL5   , 0X0000,
    ILI932X_PANEL_IF_CTRL6   , 0X0000,
    ILI932X_DISP_CTRL1       , 0x0133, // Main screen turn on
    INIT_END
};

esp_err_t screen_init() {
    #ifdef SCREEN_RD
        pin(SCREEN_RD, GPIO_MODE_DEF_OUTPUT);
        gpio_set_level(SCREEN_RD, true);
    #endif

    pin(SCREEN_CS, GPIO_MODE_DEF_OUTPUT);
    gpio_set_level(SCREEN_CS, true);
    pin(SCREEN_WR, GPIO_MODE_DEF_OUTPUT);
    gpio_set_level(SCREEN_WR, true);
    pin(SCREEN_DC, GPIO_MODE_DEF_OUTPUT);
    gpio_set_level(SCREEN_DC, false);
    
    #ifdef SCREEN_RST
        pin(SCREEN_RST, GPIO_MODE_DEF_OUTPUT);
        gpio_set_level(SCREEN_RST, false);
        wait(100);
        gpio_set_level(SCREEN_RST, true);
        wait(200);
    #endif

    bool flush_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx) {
        return false;
    }
    
    esp_lcd_i80_bus_config_t bus_config = {
        .clk_src = LCD_CLK_SRC_DEFAULT,
        .dc_gpio_num = SCREEN_DC,
        .wr_gpio_num = SCREEN_WR,
        .data_gpio_nums = SCREEN_BUS,
        .bus_width = 8,
        .max_transfer_bytes = sizeof(buffer)
    };
    ESP_ERROR_CHECK(esp_lcd_new_i80_bus(&bus_config, &i80_bus));
    
    esp_lcd_panel_io_i80_config_t io_config = {
        #ifdef SCREEN_CS
            .cs_gpio_num = SCREEN_CS,
        #endif
        .pclk_hz = SCREEN_SPEED,
        .trans_queue_depth = 10,
        .dc_levels = {
            .dc_idle_level = 0,
            .dc_cmd_level = 0,
            .dc_dummy_level = 0,
            .dc_data_level = 1,
        },
        .flags = {
            .swap_color_bytes = false
        },
        .on_color_trans_done = flush_ready,
        .user_ctx = NULL,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i80(i80_bus, &io_config, &io_handle));

    uint16_t regCount = 0;
    while (true) {
        uint16_t cmd = regValues[regCount++];
        if (cmd == INIT_END) break;
        uint16_t data = regValues[regCount++];
        printf("%i %i\n", cmd, data);
        if (cmd == INIT_DELAY) {
            wait(data);
        } else {
            ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, cmd, &data, 2));
        }
    }

    printf("START SEND\n");
    while (true) {
        uint16_t x = esp_random() % 200;
        uint16_t y = esp_random() % 200;
        uint16_t col = esp_random();
        uint8_t cmd = esp_random();

        ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, cmd, &x, 2));
        ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, cmd, &y, 2));
        ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, cmd, &col, 2));
        wait(10);
    }
    
    return ESP_OK;
}