// base
#include "main.h"
#include "filesystem.h"
#include "nvs.h"
#include "wifi.h"
#include "graphic.h"
#include "color.h"
#include "control.h"
#include "xstr.h"
#include "gui.h"
#include "system.h"
#include "hardware.h"
#include "palette.h"
#include "gfx.h"
#include "xmath.h"
#include "vec.h"
#include "shell.h"
#include "customshell.h"
#include "strtool.h"
#include "util.h"
#include "storage.h"
#include "device.h"

// drivers
#include "drivers/keyboard.h"
#include "drivers/screen.h"
#include "drivers/leds.h"

// apps
#include "apps/explorer.h"
#include "apps/settings.h"

// hidden tools
#include "hidden_tools/viewer.h"
#include "hidden_tools/pkg.h"
#include "hidden_tools/lua.h"

// games
#include "games/pong.h"
#include "games/snake.h"
#include "games/cave.h"

// tests
#include "tests/test_keyboard.h"
#include "tests/test_screen.h"
#include "tests/test_gui.h"