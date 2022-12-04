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
        void CloseFile();
    private:
        uint32_t m_selected_document = -1;
        std::vector<std::string> m_in_editor_files;
        std::vector<std::string> m_in_editor_data;

        bool m_creating_new_file = false;
        bool m_closing_file = false;
    };
}

#endif // !EDITOR_LAYER_H