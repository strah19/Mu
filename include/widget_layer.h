#ifndef WIDGET_LAYER_H
#define WIDGET_LAYER_H

#include "mu.h"
#include "menu.h"

namespace Iota {
    class WidgetLayer : public Mu::Layer {
    public:
        WidgetLayer() : Mu::Layer("Widget Layer") { }

        void OnAttach();
        void Detach();
        void UpdateGui();
        //void MenuEventCall(MenuEvent menu_event);
    };
}


#endif // !WIDGET_LAYER_H