#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "mu.h"

namespace Mu {
    class EditorLayer : public Layer {
    public:
        EditorLayer() : Layer("Editor Layer") { }

        void UpdateGui();
    };
}

#endif // !EDITOR_LAYER_H