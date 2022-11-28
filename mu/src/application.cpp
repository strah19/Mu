#include "application.h"
#include "event.h"
#include "renderer_commands.h"
#include "log.h"
#include "utility.h"

namespace Mu {
    Application* Application::m_instance = nullptr;

    Application::Application() {
        m_instance = this;

        m_properties = WindowProperties("Mu", 1280, 720);
        m_window = Window::CreateGlfwWindow(m_properties, BIND_EVENT(OnEvent));

        RendererCommands::Initialize();

        m_imgui_layer = new ImGuiLayer();
        PushOverlay(m_imgui_layer);

        MU_LOG("Mu initialized properly");
    }

    Application::~Application() {
        layers.Destroy();
        delete m_window;

        MU_LOG("Mu properly shutdown");
    }

    void Application::Update() {
        m_window->Update();
        RendererCommands::Clear(1.0f, 0.0f, 0.0f, 1.0f);
    }

    void Application::Run() {
        while(!m_window->IsQuit()) {
			float time = Mu::Time::GetTime();
			float delta = time - lastframe_time;
			lastframe_time = time;

            for (Layer* layer : layers)
                layer->OnUpdate(delta);

            m_imgui_layer->Begin();

			for (Layer* layer : layers)
				layer->UpdateGui();

            m_imgui_layer->End();
            
            Update();
        }
    }

    void Application::OnEvent(Event& event) {
		EventDispatcher dispatcher(&event);

		for (auto it = layers.rbegin(); it != layers.rend(); ++it)
			(*it)->UserDefEvent(event);

		dispatcher.Dispatch<QuitEvent>(BIND_EVENT(OnClose));
		dispatcher.Dispatch<ResizeEvent>(BIND_EVENT(OnResize));
	}

	void Application::OnClose(const QuitEvent& event) {
		m_window->Quit();
	}

	void Application::OnResize(const ResizeEvent& event) {
		m_window->Properties()->m_width = event.m_width;
		m_window->Properties()->m_height = event.m_height;

        RendererCommands::SetViewport(0, 0, event.m_width, event.m_height);
	}

    void Application::PushLayer(Layer* layer) {
		layers.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		layers.PushOverlay(layer);
		layer->OnAttach();
	}
}