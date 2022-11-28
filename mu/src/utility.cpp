#include "utility.h"

#include <GLFW/glfw3.h>

namespace Mu {
    float Time::GetTime() {
        return glfwGetTime();
    }
}