#ifndef WINDOW_H
#define WINDOW_H

#include <cstdint>
#include "event.h"

namespace Mu {
    struct WindowProperties {
        const char* m_name;
        uint32_t m_width = 0;
        uint32_t m_height = 0;

        uint32_t m_x = 0;
        uint32_t m_y = 0;

        WindowProperties() = default;
        WindowProperties(const char* name, uint32_t width, uint32_t height) : m_name(name), m_width(width), m_height(height) { }
    };
    
    class Window {
    public:
    	using EventCallbackFn = std::function<void(Event&)>;

        Window(WindowProperties properties, const EventCallbackFn& event_callback);
        virtual ~Window() = default;

        virtual void* GetNativeWindow() = 0;
        virtual void Update() = 0;
        virtual void Destroy() = 0;

        inline uint32_t Width() const { return m_properties.m_width; }
        inline uint32_t Height() const { return m_properties.m_height; }
        
        inline WindowProperties* Properties() { return &m_properties; }
        inline void Quit() { m_quit = true; }
        inline bool IsQuit() const { return m_quit; }
        static Window* CreateGlfwWindow(WindowProperties properties, const EventCallbackFn& event_callback);
    protected:
        WindowProperties m_properties;
        EventCallbackFn m_event_callback;
        bool m_quit = false;
    };
} 


#endif // !WINDOW_H