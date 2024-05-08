struct menuState {
    const char* title;
    const char** points;
    const char** imgs;
    uint8_t pointsCount;
    int16_t current;
    int16_t offset;
    int rightLeftState;
    bool rightLeftControl;
    bool alwaysRedraw;
    bool (*checker)();
};

struct tabMenuState {
    const char* title;
    uint8_t pointsCount;
    const char** points;
    const char** imgs;
    void** callbacks;
    uint8_t* callbacksInfo;
    void** callbacksData;
};

void gui_status(const char* text);
bool gui_splash(const char* text);
int gui_menu(struct menuState* menu);
int gui_selectMenu(struct menuState* menu);
bool gui_yesno(const char* title);
int gui_selectNumber(const char* title, bool back, int from, int to, int step, int current);
bool gui_exitQuestion();
int gui_drawStatusBar(const char* text);
int gui_drawScoreBar(int score);
int gui_getStatusBarPosY();
void gui_getFileImage(char* dst, const char* path);
int16_t gui_slider(const char* title, uint8_t defaultVal);
int16_t gui_sliderWithCallback(const char* title, uint8_t defaultVal, void(*)(int16_t));

void gui_menu_init(struct tabMenuState* menu, const char* title);
struct tabMenuState gui_menu_new(const char* title);
void gui_menu_addCallback(struct tabMenuState* menu, const char* title, const char* img, void(*callback)());
void gui_menu_addApp(struct tabMenuState* menu, const char* title, const char* img, void(*callback)());
void gui_menu_addSlider(struct tabMenuState* menu, const char* title, const char* img, void(*callback)(int16_t), uint8_t* defaultVal);
void gui_menu_addExit(struct tabMenuState* menu, const char* title, const char* img);
struct tabMenuState* gui_menu_addTab(struct tabMenuState* menu, const char* title, const char* img);
void gui_menu_run(struct tabMenuState* menu);
void gui_menu_runOnce(struct tabMenuState* menu);
void gui_menu_free(struct tabMenuState* menu);
void gui_popUpMenu();