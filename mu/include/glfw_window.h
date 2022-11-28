#ifndef GLFW_WINDOW_H
#define GLFW_WINDOW_H

#include "window.h"
#include "keyboard_event.h"
#include "mouse_event.h"
#include "window_event.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Mu {
    class GLFWWindow : public Window {
    public:
        GLFWWindow(WindowProperties properties, const EventCallbackFn& event_callback);
        virtual ~GLFWWindow();

        virtual void* GetNativeWindow() override;
        virtual void Update() override;
        virtual void Destroy() override;
    private:
        GLFWwindow* m_window = nullptr;
    };
} 

#endif // !GLFW_WINDOW_H