#include "application.h"
#include "event.h"
#include "renderer_commands.h"
#include "log.h"

#include "imgui.h"

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

            for (Layer* layer : layers)
                layer->OnUpdate(0.0f);

            m_imgui_layer->Begin();

            static bool opt_fullscreen = true;
            static bool opt_padding = false;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            if (opt_fullscreen)
            {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->GetWorkPos());
                ImGui::SetNextWindowSize(viewport->GetWorkSize());
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }
            else
            {
                dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
            }

            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;

            if (!opt_padding)
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", NULL, window_flags);

            if (!opt_padding)
                ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            ImGui::End();

            ImGui::Begin("Test Panel");
            ImGui::End();

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