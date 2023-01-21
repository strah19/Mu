#ifndef SCRIPT_HANDLER_H
#define SCRIPT_HANDLER_H

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace Mu {
    using LuaError = int;

    class Script {
    public:
        Script();
        ~Script();

        const char* RunLine(const char* line);
        const char* RunScript(const char* path);

        void SetInteger(const char* name, int value);
        int GetInteger(const char* name);

        const char* GetError(LuaError error);
        void PopStack();
    private:
        lua_State* L;
    };
}

#endif // !SCRIT_HANDLER_H