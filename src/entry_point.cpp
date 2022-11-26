#include "application.h"
#include "log.h"

int main(int argc, char *argv[]) {    
    Mu::InitializeLoggingSystem();
    Mu::Logs::InitializeLoggers();
    
    Mu::Application* application = Mu::Application::CreateApplication();

    application->Run();

    delete application;
}

