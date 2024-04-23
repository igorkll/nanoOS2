#include "../main/main.h"
#include "../main/color.h"
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

static uint8_t buffer[SCREEN_RESX * SCREEN_RESY * 2];

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

esp_err_t screen_init() {
    #ifdef SCREEN_RST
        pin(SCREEN_RST, GPIO_MODE_DEF_OUTPUT);
        gpio_set_level(SCREEN_RST, false);
        wait(100);
        gpio_set_level(SCREEN_RST, true);
    #endif

    bool flush_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx) {
        return false;
    }
    
    esp_lcd_i80_bus_handle_t i80_bus = NULL;
    esp_lcd_i80_bus_config_t bus_config = {
        .clk_src = LCD_CLK_SRC_DEFAULT,
        .dc_gpio_num = SCREEN_DC,
        .wr_gpio_num = SCREEN_WR,
        .data_gpio_nums = SCREEN_BUS,
        .bus_width = SCREEN_BUS_WIDTH,
        .max_transfer_bytes = sizeof(buffer)
    };
    ESP_ERROR_CHECK(esp_lcd_new_i80_bus(&bus_config, &i80_bus));

    esp_lcd_panel_io_handle_t io_handle = NULL;
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
            .swap_color_bytes = false,
        },
        .on_color_trans_done = flush_ready,
        .user_ctx = NULL,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i80(i80_bus, &io_config, &io_handle));

    esp_lcd_panel_io_tx_param(io_handle, 0xF2, (uint8_t[]) { 0 }, 1); // 3Gamma function disable
    esp_lcd_panel_io_tx_param(io_handle, 0x26, (uint8_t[]) { 1 }, 1); // Gamma curve 1 selected
    esp_lcd_panel_io_tx_param(io_handle, 0xE0, (uint8_t[]) {          // Set positive gamma
        0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00 }, 15);
    esp_lcd_panel_io_tx_param(io_handle, 0xE1, (uint8_t[]) {          // Set negative gamma
        0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F }, 15);

    while (true) {
        printf("TRY SEND\n");
        esp_lcd_panel_io_tx_color(io_handle, -1, esp_random(), 1);
        printf("END SEND\n");
        wait(1000);
    }
    

    return ESP_OK;
}