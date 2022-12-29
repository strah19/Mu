#include "utility.h"

#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "application.h"

namespace Mu {
    float Time::GetTime() {
        return glfwGetTime();
    }

    void SetIcon(const char* path) {
        GLFWimage images[1]; 
        images[0].pixels = stbi_load(path, &images[0].width, &images[0].height, 0, 4); 
        glfwSetWindowIcon(static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow()), 1, images); 
        stbi_image_free(images[0].pixels);
    }

    std::string GetNameOfPath(const std::string& path) {
        return path.substr(path.find_last_of("/\\") + 1);;
    }
}