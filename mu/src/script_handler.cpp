#include "script_handler.h"

namespace Mu {
    Script::Script() {
        L = luaL_newstate();
        luaL_openlibs(L);
    }

    Script::~Script() {
        lua_close(L);
    }

    const char* Script::RunScript(const char* path) {
        LuaError status = luaL_dofile(L, path);
        if (status == LUA_OK) lua_pop(L, lua_gettop(L));
        return GetError(status);
    }

    const char* Script::RunLine(const char* line) {
        LuaError status luaL_dostring(L, line);
        if (status == LUA_OK) lua_pop(L, lua_gettop(L));
        return GetError(status);
    }

    void Script::SetInteger(const char* name, int value) {
        lua_pushinteger(L, value);
        lua_setglobal(L, name);
    }

    int Script::GetInteger(const char* name) {
        lua_getglobal(L, name);
        int value = lua_tointeger(L, -1);
        PopStack();
        return value;
    }

    const char* Script::GetError(LuaError error) {
        return (error != LUA_OK) ? lua_tostring(L, error) : NULL;
    }

    void Script::PopStack() {
        lua_pop(L, 1);
    }
}