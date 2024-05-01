struct menuState {
    const char* title;
    const char** points;
    const char** imgs;
    uint8_t pointsCount;
    int8_t current;
    int offset;
    int rightLeftState;
    bool rightLeftControl;
    
};

struct tabMenuState {
    const char* title;
    const char** points;
    const char** imgs;
    uint8_t pointsCount;
    void(**callbacks)();
};

void gui_status(const char* text);
void gui_splash(const char* text);
int gui_menu(struct menuState* menu);
bool gui_yesno(const char* title);
int gui_selectNumber(const char* title, int from, int to);
bool gui_exitQuestion();
int gui_drawStatusBar(const char* text);
int gui_drawScoreBar(int score);
int gui_getStatusBarPosY();
void gui_getFileImage(char* dst, const char* path);
int16_t gui_slider(const char* title, uint8_t defaultVal);
int16_t gui_sliderWithCallback(const char* title, uint8_t defaultVal, void(*)(int16_t));

void gui_menu_init(struct tabMenuState* menu, const char* title);
void gui_menu_addCallback(struct tabMenuState* menu, const char* title, const char* img, void(*callback)());
void gui_menu_addExit(struct tabMenuState* menu, const char* title, const char* img);
void gui_menu_free(struct tabMenuState* menu);
void gui_menu_run(struct tabMenuState* menu);