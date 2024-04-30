struct menuState {
    const char* title;
    int pointsCount;
    const char** points;
    int current;
    int offset;
    int rightLeftState;
    bool rightLeftControl;
    const char** imgs;
};

struct advMenuState {
    const char** titles;
    const char** imgs;
    void(**callbacks)();
    uint8_t count;
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

void gui_advMenu_init(struct advMenuState* menu);
void gui_advMenu_addCallback(struct advMenuState* menu, const char* title, const char* img, void(*callback)());
void gui_advMenu_addExit(struct advMenuState* menu, const char* title, const char* img);
void gui_advMenu_free(struct advMenuState* menu);
void gui_advMenu_run(struct advMenuState* menu);