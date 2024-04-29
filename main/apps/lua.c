#include "lua.h"
#include "../all.h"
#include "../../libs/lua53/lua.h"
#include "../../libs/lua53/lauxlib.h"
#include "../../libs/lua53/lualib.h"

void lua_run() {

}

// --------------------------------------

void lua_open(const char* path) {
    uint8_t t; //un5555555

    lua_State* lua = luaL_newstate();
    luaL_openlibs(lua);

    t = luaL_dofile(lua, path);

    lua_close(lua);
}