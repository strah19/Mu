#ifndef TERMINAL_WIDGET_H
#define TERMINAL_WIDGET_H

#include "mu.h"
#include "widget.h"
#include <filesystem>
#include "terminal.h"
#include "imgui.h"

namespace Iota {
    class TerminalWidget : public Widget {
    public:
        TerminalWidget();
        void Update();
    private:
        void ClearLog();
    private:
        std::string m_input = "";
        Terminal m_terminal;
        std::vector<std::string> m_items;

        bool m_auto_scroll = true;
        bool m_show_filter = true;
        bool m_scroll_to_bottom = false;
        ImGuiTextFilter m_filter;
    };
}


#endif // !TERMINAL_WIDGET_H