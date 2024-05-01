#include "lua.h"
#include "../all.h"
#include "../../libs/lua53/lua.h"
#include "../../libs/lua53/lauxlib.h"
#include "../../libs/lua53/lualib.h"

// ----------------------- bind utils

static uint8_t increment(uint8_t* v) {
    (*v)++;
    return *v;
}

#define LUA_INT luaL_checkinteger(L, increment(&i))
#define LUA_BIND(func, name, ...) { \
    int c_func(lua_State* L) { \
        uint8_t i = 0; \
        func(__VA_ARGS__); \
        return 0; \
    } \
    lua_pushcfunction(lua, c_func); \
    lua_setglobal(lua, name);\
}

// -----------------------

void _test(int x, int y) {
    printf("%i %i\n", x, y);
}

void lua_open(const char* path) {
    graphic_clear(color_black);
    graphic_update();
    
    lua_State* lua = luaL_newstate();
    luaL_openlibs(lua);
    LUA_BIND(_test, "print", LUA_INT, LUA_INT);
    if (luaL_dofile(lua, path)) {
        graphic_printf(color_red, "error: %s", lua_tostring(lua, -1));
        graphic_update();
    } else {
        graphic_printf(color_green, "press ESC to exit");
        graphic_update();
    }
    lua_close(lua);
    control_waitExit();
}