#include "logger_widget.h"
#include "imgui.h"
#include "log.h"
#include "imgui_stdlib.h"

namespace Iota {
    LoggerWidget::LoggerWidget() : Widget("Logger") { }

    void LoggerWidget::Update() {
        ImGui::Begin(m_name.c_str());

        if (ImGui::Button("Clear Log")) {
            Mu::GetLogEntries()->clear();
        }

        ImGui::SameLine();
        if (ImGui::Button("Add Log") && !m_input_log.empty()) {
            MU_LOG("%s", m_input_log.c_str());
            m_input_log.clear();
        }

        ImGui::PushItemWidth(-1);
        ImGui::SameLine();
        ImGui::InputText("##custom log", &m_input_log);
        ImGui::PopItemWidth();

        ImGui::Separator();

        for (auto& entry : *Mu::GetLogEntries()) 
            ImGui::Text("%s", entry.c_str());

        ImGui::End();
    }
}