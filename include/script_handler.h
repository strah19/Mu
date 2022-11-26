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
        void Initialize();
    private:
    };
}

#endif // !SCRIT_HANDLER_H