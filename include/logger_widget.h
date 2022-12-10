#ifndef LOGGER_WIDGET_H
#define LOGGER_WIDGET_H

#include "mu.h"
#include "widget.h"

namespace Iota {
    class LoggerWidget : public Widget {
    public:
        LoggerWidget();
        virtual void Update();
    private:
        std::string m_input_log;
    };
}

#endif // !LOGGER_WIDGET_H