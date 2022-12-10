#include "terminal_widget.h"
#include "imgui.h"
#include "imgui_stdlib.h"

namespace Iota {
    TerminalWidget::TerminalWidget() : Widget("Terminal") { }

    void TerminalWidget::Update() {
        ImGui::Begin(m_name.c_str());

        ImGui::PushItemWidth(-1);
        ImGui::InputText("##terminal input", &m_input);
        ImGui::PopItemWidth();

        ImGui::End();
    }
}