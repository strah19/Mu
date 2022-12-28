#ifndef SCRIPT_HANDLER_H
#define SCRIPT_HANDLER_H

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace Mu {
    class ScriptLoader {
    public:
        ScriptLoader();
        ~ScriptLoader();

        int RunLine(const char* line);
        int RunScript(const char* path);
        const char* GetError(int error);
    private:
        lua_State* L;
    };
}

#endif // !SCRIT_HANDLER_H