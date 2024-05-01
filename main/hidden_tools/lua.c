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
    luaL_openlibs(lua);
    #include "../service/lua_binds.h"
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