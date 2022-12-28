#include "script_handler.h"

namespace Mu {
    ScriptLoader::ScriptLoader() {
        L = luaL_newstate();
        luaL_openlibs(L);
    }

    ScriptLoader::~ScriptLoader() {
        lua_close(L);
    }

    int ScriptLoader::RunScript(const char* path) {
        return luaL_dofile(L, path);
    }

    int ScriptLoader::RunLine(const char* line) {
        return luaL_dostring(L, line);
    }

    const char* ScriptLoader::GetError(int error) {
        return (error != LUA_OK) ? lua_tostring(L, error) : NULL;
    }
}