#ifndef TERMINAL_WIDGET_H
#define TERMINAL_WIDGET_H

#include "mu.h"
#include "widget.h"

namespace Iota {
    class TerminalWidget : public Widget {
    public:
        TerminalWidget();
        void Update();
    };
}


#endif // !TERMINAL_WIDGET_H