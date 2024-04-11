typedef enum {
    CONTROL_UP,
    CONTROL_RIGHT,
    CONTROL_DOWN,
    CONTROL_LEFT
} control_arrow;


bool control_isEnter();
bool control_isEnterPressed();

bool control_isMoveButton(int index);
bool control_isMoveButtonPressed(int index);

bool control_needExit();
bool control_needExitWithoutGui();
void control_waitExit();
void control_waitEnter();