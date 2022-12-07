#include "logger_widget.h"
#include "imgui.h"
#include "log.h"

namespace Iota {
    void LoggerWidget::UpdateGui() {
        if (m_selected) {
            ImGui::Begin("Logger");

            for (auto& entry : *Mu::GetLogEntries()) 
                ImGui::Text("%s", entry.c_str());

            ImGui::End();
        }
    }
}