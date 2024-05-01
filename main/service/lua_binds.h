{
    int _print(lua_State *L) {
        int n = lua_gettop(L);
        int i;
        lua_getglobal(L, "tostring");
        xstr str = xstr_new();
        for (i=1; i<=n; i++) {
            const char *s;
            size_t l;
            lua_pushvalue(L, -1);
            lua_pushvalue(L, i);
            lua_call(L, 1, 1);
            s = lua_tolstring(L, -1, &l);
            if (s == NULL) return luaL_error(L, "'tostring' must return a string to 'print'");
            if (i>1) xstr_appendRight(&str, " ");
            xstr_appendRight(&str, s);
            lua_pop(L, 1);
        }
        graphic_sprint(str.ptr, color_white);
        graphic_update();
        xstr_del(&str);
        return 0;
    }

    lua_pushcfunction(lua, _print);
    lua_setglobal(lua, "print");

    // hook
    bool _exitCheck(lua_State *L) {
        control_begin();
        if (control_needExitWithoutGui()) {
            lua_userExit = true;
            lua_pushliteral(L, "interrupted");
            lua_error(L);
            return true;
        }
        return false;
    }
    void _hook(lua_State *L) {
        _exitCheck(L);
    }
    lua_sethook(lua, _hook, LUA_MASKRET | LUA_MASKCOUNT, 100);

    //base
    int _wait(lua_State *L) {
        int ticksTime = luaL_checkinteger(L, 1) / portTICK_PERIOD_MS;
        while (true) {
            if (_exitCheck(L)) return 0;
            vTaskDelay(1);
            if (--ticksTime <= 0) return 0;
        }
        return 0;
    }

    lua_pushcfunction(lua, _wait);
    lua_setglobal(lua, "wait");

    //control
    LUA_PUSH_INT(CONTROL_COUNT);
    LUA_PUSH_INT(CONTROL_UP);
    LUA_PUSH_INT(CONTROL_RIGHT);
    LUA_PUSH_INT(CONTROL_DOWN);
    LUA_PUSH_INT(CONTROL_LEFT);
    LUA_PUSH_INT(CONTROL_ENTER);
    LUA_PUSH_INT(CONTROL_ESC);

    LUA_BIND_VOID(control_begin, ());
    LUA_BIND_RETR(control_getState, (LUA_ARG_INT), LUA_RET_BOOL);
}