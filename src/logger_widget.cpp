#include "logger_widget.h"
#include "imgui.h"
#include "log.h"

namespace Iota {
    LoggerWidget::LoggerWidget() : Widget("Logger") { }

    void LoggerWidget::Update() {
        ImGui::Begin(m_name.c_str());

        for (auto& entry : *Mu::GetLogEntries()) 
            ImGui::Text("%s", entry.c_str());

        ImGui::End();
    }
}