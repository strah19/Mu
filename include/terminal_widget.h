#ifndef TERMINAL_WIDGET_H
#define TERMINAL_WIDGET_H

#include "mu.h"

namespace Iota {
    class TerminalWidget {
    public:
        void UpdateGui();
        void MenuSelected() { m_selected = !m_selected; }
    private:
        bool m_selected = false;
    };
}


#endif // !TERMINAL_WIDGET_H