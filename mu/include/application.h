#ifndef APPLICATION_H
#define APPLICATION_H

#include "window.h"
#include "window_event.h"
#include "layer.h"
#include "imgui_layer.h"

int main(int argc, char* argv[]);

namespace Mu {
    class Application {
    public:
        Application();
        virtual ~Application();

        void Run();
        void Update();

        void PushLayer(Layer* layer);

        static Application& Get() { return *m_instance; }
        Window* GetWindow() { return m_window; }
        void Quit();
    protected:
        WindowProperties m_properties;
        Window* m_window = nullptr;
    	LayerStack layers; 
    	ImGuiLayer* m_imgui_layer;
    private:
        void OnClose(const QuitEvent& event);
        void OnResize(const ResizeEvent& event);
        void OnEvent(Event& event);
    private:
        static Application* m_instance;
        float lastframe_time = 0.0f;
		friend int ::main(int argc, char** argv);
    };

    static Application* CreateApplication();
}

#endif // !APPLICATION_H