#ifndef WIDGET_LAYER_H
#define WIDGET_LAYER_H

#include "mu.h"
#include "menu.h"

// Widgets
#include "logger_widget.h"
#include "terminal_widget.h"

namespace Iota {
    class WidgetLayer : public Mu::Layer {
    public:
        WidgetLayer() : Mu::Layer("Widget Layer") { }

        void OnAttach();
        void Detach();
        void UpdateGui();
    private:
        LoggerWidget m_logger;
        TerminalWidget m_terminal;

        Menu m_view_menu;
    };
}


#endif // !WIDGET_LAYER_H