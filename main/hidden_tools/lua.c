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

#define LUA_ARG_INT luaL_checkinteger(L, increment(&i))
#define LUA_RET_INT(val) lua_pushinteger(L, val)

#define LUA_BIND_VOID(func, name, args) { \
    int c_func(lua_State* L) { \
        uint8_t i = 0; \
        func args; \
        return 0; \
    } \
    lua_pushcfunction(lua, c_func); \
    lua_setglobal(lua, name); \
}

#define LUA_BIND_RETR(func, name, args, returnType) { \
    int c_func(lua_State* L) { \
        uint8_t i = 0; \
        returnType(func args) \
        return 1; \
    } \
    lua_pushcfunction(lua, c_func); \
    lua_setglobal(lua, name); \
}

// -----------------------

void lua_open(const char* path) {
    graphic_clear(color_black);
    graphic_update();
    
    lua_State* lua = luaL_newstate();
    luaL_openlibs(lua);
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