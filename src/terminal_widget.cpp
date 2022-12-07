#include "terminal_widget.h"
#include "imgui.h"

namespace Iota {
    void TerminalWidget::UpdateGui() {
        if (m_selected) {
            ImGui::Begin("Terminal");

            ImGui::End();
        }
    }
}