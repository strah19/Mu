#ifndef ENTRY_POINT_H
#define ENTRY_POINT_H

#include "application.h"
#include "log.h"

extern Mu::Application* Mu::CreateApplication();

int main(int argc, char *argv[]) {    
    Mu::InitializeLoggingSystem();
    Mu::Logs::InitializeLoggers();
    
    Mu::Application* application = Mu::CreateApplication();
    MU_LOG_GOOD("Application Initalized");

    application->Run();

    delete application;
    MU_LOG_GOOD("Application closed");
}

#endif // !ENTRY_POINT_H