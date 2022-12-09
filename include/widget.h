#ifndef WIDGET_H
#define WIDGET_H

#include <string>

namespace Iota {
    class Widget {
    public:
        Widget(const std::string& name) : m_name(name) { }
        virtual ~Widget() = default;

        virtual void Update() = 0;
        inline std::string GetName() const { return m_name; }
    protected:
        std::string m_name;
    };
}

#endif // !WIDGET_H