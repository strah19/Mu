#include "application.h"
#include "log.h"

namespace Mu {
    Application* Application::m_instance = nullptr;

    Application::Application() {
        m_instance = this;
        MU_LOG("Mu initialized properly")
    }

    Application::~Application() {
        MU_LOG("Mu properly shutdown")
    }

    void Application::Run() {
        
    }

    Application* Application::CreateApplication() {
        return new Application();
    }
}