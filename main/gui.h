struct menuState
{
    const char* title;
    int pointsCount;
    char** points;
    int current;
    int offset;
    int rightLeftState;
    bool rightLeftControl;
};

bool gui_isEnter();
bool gui_isMoveButton(int index);
void gui_status(const char* text);
void gui_splash(const char* text);
int gui_menu(struct menuState* menu);