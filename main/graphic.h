int graphic_x();
int graphic_y();
void graphic_update();

int graphic_getTextSize(const char* text);
int graphic_getFontSizeX();
int graphic_getFontSizeY();

void graphic_clear(uint32_t color);
void graphic_drawText(int x, int y, const char* text, uint32_t color);
void graphic_drawTextBox(int x, int y, int sizeX, int sizeY, const char* text, uint32_t color);
void graphic_drawImage(int x, int y, const char* path);
void graphic_drawPixel(int x, int y, uint32_t color);
void graphic_drawRect(int x, int y, int sizeX, int sizeY, uint32_t color);
void graphic_fillRect(int x, int y, int sizeX, int sizeY, uint32_t color);