#include "widget_layer.h"

namespace Iota {
    void WidgetLayer::OnAttach() {
        m_view_menu = Menu("View");

        m_view_menu.menu_items.push_back(MenuItem("Logger", BIND_FN(m_logger.MenuSelected)));     
        m_view_menu.menu_items.push_back(MenuItem("Terminal", BIND_FN(m_terminal.MenuSelected)));

        MenuViewer::GetMenu()->AddMenu(&m_view_menu);
    }

    void WidgetLayer::Detach() {

    }

    void WidgetLayer::UpdateGui() {
        m_logger.UpdateGui();
        m_terminal.UpdateGui();
    }
}