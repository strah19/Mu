#include "terminal_widget.h"
#include "imgui.h"

namespace Iota {
    TerminalWidget::TerminalWidget() : Widget("Terminal") { }

    void TerminalWidget::Update() {
        ImGui::Begin(m_name.c_str());

        ImGui::End();
    }
}