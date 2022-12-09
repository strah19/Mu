#ifndef WIDGET_LAYER_H
#define WIDGET_LAYER_H

#include "mu.h"
#include "menu.h"

// Widgets
#include "widget.h"
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
        std::vector<Widget*> m_widgets;
    };
}


#endif // !WIDGET_LAYER_H