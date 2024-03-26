typedef enum {
    colormode_blackwhite = 0,
    colormode_color      = 1,
    colormode_monochrome = 2
} screen_colormode;

esp_err_t screen_init();
int screen_x();
int screen_y();
screen_colormode screen_getColorMode();

void screen_set(int x, int y, uint32_t color);
uint32_t screen_get(int x, int y);

void screen_update();
bool screen_needUpdate();

void screen_tick();
bool screen_needTick();