esp_err_t screen_init();
int screen_x();
int screen_y();

void screen_set(int x, int y, uint32_t color);
uint32_t screen_get(int x, int y);
void screen_update();