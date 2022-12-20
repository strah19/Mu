#ifndef WIDGET_LAYER_H
#define WIDGET_LAYER_H

#include "mu.h"
#include "menu.h"
#include "editor.h"

// Widgets
#include "widget.h"
#include "logger_widget.h"
#include "terminal_widget.h"

namespace Iota {
    class WidgetLayer : public Mu::Layer {
    public:
        WidgetLayer(Editor* editor) : Mu::Layer("Widget Layer"), m_editor(editor) { }

        void OnAttach();
        void Detach();
        void UpdateGui();
    private:
        std::vector<Widget*> m_widgets;

        Editor* m_editor;
    };
}


#endif // !WIDGET_LAYER_H