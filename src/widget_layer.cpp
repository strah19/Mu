#include "widget_layer.h"

namespace Iota {
    void WidgetLayer::OnAttach() {
        m_widgets.push_back(new LoggerWidget());
        m_widgets.push_back(new TerminalWidget());
    }

    void WidgetLayer::Detach() {
        for (auto& widget : m_widgets)
            delete widget;
    }

    void WidgetLayer::UpdateGui() {
        for (auto& widget : m_widgets)
            widget->Update();
    }
}