typedef enum {
    blackwhite = 0,
    color      = 1,
    monochrome = 2
} screen_colorModes;

esp_err_t screen_init();
int screen_x();
int screen_y();
int screen_colorMode();

void screen_set(int x, int y, uint32_t color);
uint32_t screen_get(int x, int y);

void screen_update();
bool screen_needUpdate();

void screen_tick();
bool screen_needTick();