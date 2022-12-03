#ifndef SETUP_LAYER_H
#define SETUP_LAYER_H

#include "mu.h"
#include "menu.h"

namespace Iota {
    class SetupLayer : public Mu::Layer {
    public:
        SetupLayer(Menu menu) : Mu::Layer("Setup Layer"), m_menu(menu) { }

        void UpdateGui();
    private:
        Menu m_menu;
    };
}

#endif // !SETUP_LAYER_H