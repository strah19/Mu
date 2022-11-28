#include "window.h"
#include "glfw_window.h"

namespace Mu {
    Window::Window(WindowProperties properties, const EventCallbackFn& event_callback) : m_properties(properties), m_event_callback(event_callback) { }

    Window* Window::CreateGlfwWindow(WindowProperties properties, const EventCallbackFn& event_callback) {
        return new GLFWWindow(properties, event_callback);
    }  
}