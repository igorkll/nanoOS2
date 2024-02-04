#include "main.h"
#include "lua.h"

#include <../libs/lua/lua.h>
#include <../libs/lua/lualib.h>
#include <../libs/lua/lauxlib.h>

#include "drivers/keyboard.h"
#include "drivers/screen.h"

#include "wifi.h"
#include "graphic.h"

// ----------------------------- bindings

// ---- main
int lua_gpio_output(lua_State* L) {
    int number = luaL_checkinteger(L, 1);
    bool state = lua_toboolean(L, 2);
    gpio_set_level(number, state ? 1 : 0);
    return 0;
}

int lua_gpio_input(lua_State* L) {
    int number = luaL_checkinteger(L, 1);
    int value = gpio_get_level(number);
    lua_pushboolean(L, value > 0);
    return 1;
}

int lua_gpio_mode(lua_State* L) {
    int number = luaL_checkinteger(L, 1);
    int mode = luaL_checkinteger(L, 2);

    switch (mode) {
        case 0:
            pin(number, GPIO_MODE_DEF_DISABLE);
            break;
        case 1:
            pin(number, GPIO_MODE_DEF_INPUT);
            break;
        case 2:
            pin(number, GPIO_MODE_DEF_OUTPUT);
            break;
    }
    
    return 0;
}

int lua_sleep(lua_State* L) {
    int duration = luaL_checkinteger(L, 1);
    if (duration == 0) {
        yeild();
    } else {
        wait(duration);
    }
    return 0;
}

// ---- screen
int lua_screen_set(lua_State* L) {
    screen_set(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
    return 0;
}

int lua_screen_update(lua_State* L) {
    screen_update();
    return 0;
}

// ---- wifi
int lua_wifi_stop(lua_State* L) {
    wifi_stop();
    return 0;
}

int lua_wifi_connect(lua_State* L) {
    lua_pushinteger(L, wifi_connect(luaL_checkstring(L, 1), luaL_checkstring(L, 2)));
    return 1;
}

int lua_wifi_accesspoint(lua_State* L) {
    lua_pushinteger(L, wifi_accesspoint(luaL_checkstring(L, 1), luaL_checkstring(L, 2)));
    return 1;
}

// ---- keyboard
int lua_keyboard_isChar(lua_State* L) {
    lua_pushboolean(L, keyboard_isChar(luaL_checkinteger(L, 1)));
    return 1;
}

int lua_keyboard_isMoveButton(lua_State* L) {
    lua_pushboolean(L, keyboard_isMoveButton(luaL_checkinteger(L, 1)));
    return 1;
}

/*
int lua_keyboard_get(lua_State* L) {
    lua_pushboolean(L, keyboard_get(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2)));
    return 1;
}
*/

// ---- fs

// ---- graphic
int lua_graphic_image(lua_State* L) {
    graphic_image(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkstring(L, 3));
    return 0;
}


// ----------------------------- init

void lua_run() {
    lua_State* Lua = luaL_newstate();
    luaL_openlibs(Lua);
    
    // ---- main
    lua_pushcfunction(Lua, lua_gpio_output);
    lua_setglobal(Lua, "gpio_output");

    lua_pushcfunction(Lua, lua_gpio_input);
    lua_setglobal(Lua, "gpio_input");

    lua_pushcfunction(Lua, lua_gpio_mode);
    lua_setglobal(Lua, "gpio_mode");

    lua_pushcfunction(Lua, lua_sleep);
    lua_setglobal(Lua, "sleep");

    // ---- screen
    lua_pushcfunction(Lua, lua_screen_set);
    lua_setglobal(Lua, "screen_set");

    lua_pushcfunction(Lua, lua_screen_update);
    lua_setglobal(Lua, "screen_update");

    lua_pushinteger(Lua, screen_x());
    lua_setglobal(Lua, "SCREEN_RESX");

    lua_pushinteger(Lua, screen_y());
    lua_setglobal(Lua, "SCREEN_RESY");
    
    // ---- wifi
    lua_pushcfunction(Lua, lua_wifi_stop);
    lua_setglobal(Lua, "wifi_stop");

    lua_pushcfunction(Lua, lua_wifi_connect);
    lua_setglobal(Lua, "wifi_connect");

    lua_pushcfunction(Lua, lua_wifi_accesspoint);
    lua_setglobal(Lua, "wifi_accesspoint");

    // ---- keyboard
    lua_pushcfunction(Lua, lua_keyboard_isChar);
    lua_setglobal(Lua, "keyboard_isChar");

    lua_pushcfunction(Lua, lua_keyboard_isMoveButton);
    lua_setglobal(Lua, "keyboard_isMoveButton");

    //lua_pushcfunction(Lua, lua_keyboard_get);
    //lua_setglobal(Lua, "keyboard_get");

    // ---- fs

    // ---- graphic
    lua_pushcfunction(Lua, lua_graphic_image);
    lua_setglobal(Lua, "graphic_image");


    int ret = luaL_dofile(Lua, "/storage/init.lua");
    if (ret != LUA_OK) {
        printf("Error: %s\n", lua_tostring(Lua, -1));
        lua_pop(Lua, 1);
    }
}