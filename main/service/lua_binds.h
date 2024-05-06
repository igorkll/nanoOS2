//THIS FILE WAS GENERATED AUTOMATICALLY BY THE '/tools/generate_binds' UTILITY
{
    // -------- hook
    void _luaError(lua_State *L) {        lua_pushliteral(L, "interrupted");        lua_error(L);    }        bool _exitCheck(lua_State *L) {        if (lua_userExit) {            _luaError(L);            return true;        }                control_begin();        if (control_needExitWithoutGui()) {            lua_userExit = true;            _luaError(L);            return true;        }        return false;    }        void _hook(lua_State *L) {        _exitCheck(L);    }        lua_sethook(lua, _hook, LUA_MASKRET | LUA_MASKCOUNT, 100);

    // -------- print
    int _print(lua_State *L) {        int n = lua_gettop(L);        int i;        lua_getglobal(L, "tostring");        xstr str = xstr_new();        for (i=1; i<=n; i++) {            const char *s;            size_t l;            lua_pushvalue(L, -1);            lua_pushvalue(L, i);            lua_call(L, 1, 1);            s = lua_tolstring(L, -1, &l);            if (s == NULL) return luaL_error(L, "'tostring' must return a string to 'print'");            if (i>1) xstr_appendRight(&str, " ");            xstr_appendRight(&str, s);            lua_pop(L, 1);        }        graphic_sprint(str.ptr, color_white);        graphic_update();        xstr_del(&str);        return 0;    }        lua_pushcfunction(lua, _print);    lua_setglobal(lua, "print");

    // -------- wait
    int _wait(lua_State *L) {        int ticksTime = luaL_checkinteger(L, 1) / portTICK_PERIOD_MS;        while (true) {            if (_exitCheck(L)) return 0;            vTaskDelay(1);            if (--ticksTime <= 0) return 0;        }        return 0;    }        lua_pushcfunction(lua, _wait);    lua_setglobal(lua, "wait");

    // -------- storage.h
    LUA_BIND_VOID(storage_sysconf_push, ());
    LUA_BIND_VOID(storage_sysconf_pull, ());
    LUA_BIND_RETR(storage_sysconf_save, (), LUA_RET_BOOL);
    LUA_BIND_RETR(storage_sysconf_load, (), LUA_RET_BOOL);

    // -------- shell.h
    LUA_BIND_VOID(shell_run, ());

    // -------- system.h
    LUA_BIND_VOID(system_printVars, ());
    LUA_BIND_VOID(system_reset, ());
    LUA_BIND_RETR(system_isLittleEndian, (), LUA_RET_BOOL);

    // -------- xmath.h
    LUA_BIND_VOID(xmath_fpsCountReset, ());
    LUA_BIND_RETR(xmath_fpsCount, (LUA_ARG_INT), LUA_RET_INT);

    // -------- customshell.h
    LUA_BIND_VOID(customshell_run, ());

    // -------- control.h
    LUA_BIND_VOID(control_begin, ());
    LUA_BIND_RETR(control_isEnter, (), LUA_RET_BOOL);
    LUA_BIND_RETR(control_isEnterPressed, (), LUA_RET_BOOL);
    LUA_BIND_RETR(control_needExit, (), LUA_RET_BOOL);
    LUA_BIND_RETR(control_needExitWithoutGui, (), LUA_RET_BOOL);
    LUA_BIND_VOID(control_waitExit, ());
    LUA_BIND_VOID(control_waitEnter, ());
    LUA_BIND_RETR(control_waitExitOrEnter, (), LUA_RET_BOOL);
    LUA_BIND_RETR(control_needExitOrEnter, (), LUA_RET_BOOL);
    LUA_BIND_RETR(control_pageUpState, (), LUA_RET_INT);
    LUA_BIND_RETR(control_pageDownState, (), LUA_RET_INT);
    LUA_BIND_RETR(control_pageLeftState, (), LUA_RET_INT);
    LUA_BIND_RETR(control_pageRightState, (), LUA_RET_INT);
    LUA_BIND_RETR(control_pageUp, (), LUA_RET_BOOL);
    LUA_BIND_RETR(control_pageDown, (), LUA_RET_BOOL);
    LUA_BIND_RETR(control_pageLeft, (), LUA_RET_BOOL);
    LUA_BIND_RETR(control_pageRight, (), LUA_RET_BOOL);

    // -------- graphic.h
    LUA_BIND_VOID(graphic_resetCrop, ());
    LUA_BIND_VOID(graphic_saveCrop, ());
    LUA_BIND_VOID(graphic_setCrop, (LUA_ARG_INT));
    LUA_BIND_VOID(graphic_setCropXY, (LUA_ARG_INT, LUA_ARG_INT));
    LUA_BIND_VOID(graphic_setXCloserTo, (LUA_ARG_INT));
    LUA_BIND_VOID(graphic_setYCloserTo, (LUA_ARG_INT));
    LUA_BIND_VOID(graphic_setXYCloserTo, (LUA_ARG_INT, LUA_ARG_INT));
    LUA_BIND_RETR(graphic_centerX, (LUA_ARG_INT), LUA_RET_INT);
    LUA_BIND_RETR(graphic_centerY, (LUA_ARG_INT), LUA_RET_INT);
    LUA_BIND_RETR(graphic_isColor, (), LUA_RET_BOOL);
    LUA_BIND_VOID(graphic_setRotation, (LUA_ARG_INT));
    LUA_BIND_VOID(graphic_update, ());
    LUA_BIND_VOID(graphic_end, ());
    LUA_BIND_VOID(graphic_copy, (LUA_ARG_INT, LUA_ARG_INT, LUA_ARG_INT, LUA_ARG_INT, LUA_ARG_INT, LUA_ARG_INT));
    LUA_BIND_VOID(graphic_resetCursor, ());
    LUA_BIND_VOID(graphic_setCursor, (LUA_ARG_INT, LUA_ARG_INT));
    LUA_BIND_RETR(graphic_getCursorX, (), LUA_RET_INT);
    LUA_BIND_RETR(graphic_getCursorY, (), LUA_RET_INT);

    // -------- functions.h
    LUA_BIND_VOID(yield, ());
    LUA_BIND_VOID(mYield, ());
    LUA_BIND_VOID(loop, ());
    LUA_BIND_RETR(nRound, (LUA_ARG_NUM), LUA_RET_INT);
    LUA_BIND_RETR(map, (LUA_ARG_INT, LUA_ARG_INT, LUA_ARG_INT, LUA_ARG_INT, LUA_ARG_INT), LUA_RET_INT);
    LUA_BIND_RETR(rmap, (LUA_ARG_INT, LUA_ARG_INT, LUA_ARG_INT, LUA_ARG_INT, LUA_ARG_INT), LUA_RET_INT);

    // -------- gfx.h
    LUA_BIND_VOID(gfx_boxBlur, (LUA_ARG_INT, LUA_ARG_INT, LUA_ARG_INT, LUA_ARG_INT, LUA_ARG_INT));

    // -------- gui.h
    LUA_BIND_RETR(gui_exitQuestion, (), LUA_RET_BOOL);
    LUA_BIND_RETR(gui_drawScoreBar, (LUA_ARG_INT), LUA_RET_INT);
    LUA_BIND_RETR(gui_getStatusBarPosY, (), LUA_RET_INT);

    // -------- filesystem.h
    LUA_BIND_VOID(filesystem_defaultDirectory, ());

    // -------- device.h
    LUA_BIND_RETR(device_isAutoBacklight, (), LUA_RET_BOOL);
    LUA_BIND_VOID(device_update, ());
    LUA_BIND_VOID(device_setBacklightValue, (LUA_ARG_INT));
}