#include "terminal_widget.h"
#include "imgui_stdlib.h"

namespace Iota {
    TerminalWidget::TerminalWidget() : Widget("Terminal") { }

    void TerminalWidget::Update() {
        ImGui::Begin(m_name.c_str());

        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        if (ImGui::SmallButton("Clear")) 
            ClearLog(); 
        ImGui::SameLine();
        bool copy_to_clipboard = ImGui::SmallButton("Copy");

        if (m_show_filter) {
            ImGui::SameLine();
            m_filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180); 
        }

        if (ImGui::BeginPopup("Options")) {
            ImGui::Checkbox("Auto-scroll", &m_auto_scroll);
            ImGui::Checkbox("Filter", &m_show_filter);
            ImGui::EndPopup();
        }
        ImGui::Separator();

        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::Selectable("Clear")) ClearLog();
            ImGui::EndPopup(); 
        }

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
        if (copy_to_clipboard)
            ImGui::LogToClipboard();
        for (int i = 0; i < m_items.size(); i++) {
            const char* item = m_items[i].c_str();
            if (!m_filter.PassFilter(item) && m_show_filter)
                continue;

            ImVec4 color;
            bool has_color = false;
            if (strstr(item, "[error]"))          { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
            if (has_color)
                ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImGui::TextUnformatted(item);
            if (has_color)
                ImGui::PopStyleColor();      
        }
        if (copy_to_clipboard)
            ImGui::LogFinish();

        if (m_scroll_to_bottom || (m_auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
            ImGui::SetScrollHereY(1.0f);
        m_scroll_to_bottom = false;

        ImGui::PopStyleVar();
        ImGui::EndChild();
        ImGui::Separator();

        ImGui::PushItemWidth(-1);
        bool reclaim_focus = false;
        if (ImGui::InputText("##terminal input", &m_input, ImGuiInputTextFlags_EnterReturnsTrue)) {
            m_terminal.SetInput(m_input);
            m_terminal.Execute();

            std::vector<std::string> outputs = m_terminal.GetOutput();
            for (int i = 0; i < outputs.size(); i++)
                m_items.push_back(outputs[i]);

            m_input.clear();
            reclaim_focus = true;
            m_scroll_to_bottom = true;
        }
        ImGui::PopItemWidth();

        ImGui::SetItemDefaultFocus();
        if (reclaim_focus)
            ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

        ImGui::End();
    }

    void TerminalWidget::ClearLog() {
        m_items.clear();
    }
}