#define CONTROL_COUNT 6
typedef enum {
    CONTROL_UP,
    CONTROL_RIGHT,
    CONTROL_DOWN,
    CONTROL_LEFT,

    CONTROL_ENTER,
    CONTROL_ESC
} control_keys;

int8_t control_get(control_keys key);
bool control_getState(control_keys key);
bool control_isPressed(control_keys key);
bool control_isPressedExtended(control_keys key);
bool control_isReleased(control_keys key);

bool control_isEnter();
bool control_isEnterPressed();

bool control_isMoveButton(int index);
bool control_isMoveButtonPressed(int index);

bool control_needExit();
bool control_needExitWithoutGui();
void control_waitExit();
void control_waitEnter();