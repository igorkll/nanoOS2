#include "lua.h"
#include "../all.h"
#include "../../libs/lua53/lua.h"
#include "../../libs/lua53/lauxlib.h"
#include "../../libs/lua53/lualib.h"
#include "../service/lua_binder.h"

void lua_open(const char* path) {
    graphic_clear(color_black);
    graphic_update();
    
    lua_State* lua = luaL_newstate();
    static bool lua_userExit;
    lua_userExit = false;
    luaL_openlibs(lua);
    #include "../service/lua_binds.h"
    if (luaL_dofile(lua, path) && !lua_userExit) {
        graphic_printf(color_red, "error: %s", lua_tostring(lua, -1));
        graphic_update();
    }
    lua_close(lua);
    if (!lua_userExit) control_waitExit();
}