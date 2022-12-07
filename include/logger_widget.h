#ifndef LOGGER_WIDGET_H
#define LOGGER_WIDGET_H

#include "mu.h"

namespace Iota {
    class LoggerWidget {
    public:
        void UpdateGui();
        void MenuSelected() { m_selected = !m_selected; }
    private:
        bool m_selected = false;
    };
}

#endif // !LOGGER_WIDGET_H