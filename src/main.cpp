#include <iostream>
//#include "fractal.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

int main() {
    std::cout << "Hello Mu" << std::endl;
   // Fractal::test();

    std::string cmd = "a = 7 + 11";

    lua_State *L = luaL_newstate();

    int error = luaL_dostring(L, cmd.c_str());
    if (error == LUA_OK) {
        lua_getglobal(L, "a");
        if (lua_isnumber(L, -1)) {
            float a_cpp  = (float) lua_tonumber(L, -1);
            std::cout << "a: " << a_cpp << std::endl;
        }
    }
    else {
        std::string error_msg = lua_tostring(L, -1);
        std::cout << error_msg << std::endl;
    }

    lua_close(L);

    return 0;
}