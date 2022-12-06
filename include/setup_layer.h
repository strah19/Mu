#ifndef SETUP_LAYER_H
#define SETUP_LAYER_H

#include "mu.h"
#include "menu.h"

namespace Iota {
    class SetupLayer : public Mu::Layer {
    public:
        SetupLayer() : Mu::Layer("Setup Layer") { }

        void UpdateGui();
    };
}

#endif // !SETUP_LAYER_H