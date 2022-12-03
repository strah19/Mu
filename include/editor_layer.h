#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "mu.h"
#include "menu.h"
#include <vector>
#include "imgui.h"

namespace Iota {
    class EditorLayer : public Mu::Layer {
    public:
        EditorLayer() : Mu::Layer ("Editor Layer") { }
        virtual ~EditorLayer() { }

        void MenuEventCall(MenuEvent menu_event);
        void UpdateGui();
    private:
        void NewFile();
        std::string new_file_name;
        std::vector<std::string> in_editor_files;
        std::vector<ImVector<char>> in_editor_data;
        bool creating_new_file = false;
    };
}

#endif // !EDITOR_LAYER_H