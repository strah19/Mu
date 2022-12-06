#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "mu.h"
#include "menu.h"
#include <vector>
#include "imgui.h"

namespace Iota {
    struct Document {
        std::string name;
        std::string content;
        Mu::File file;
        bool edited = false;

        Document() = default;
        virtual ~Document() { }
        Document(const std::string& name) : name(name) { }
    };

    class EditorLayer : public Mu::Layer {
    public:
        EditorLayer() : Mu::Layer ("Editor Layer") { }

        void OnAttach();
        void OnDetach();
        void UpdateGui();

        void NewFileCallback();
        void OpenFileCallback();
        void CloseFileCallback();
        void SaveFileCallback();
    private:
        void NewFile();
        void CloseFile();
        void CloseSelectedFile();
        bool SaveSelectedFile();
        void NoFile();

        void NewCenterPopup(const char* name);
    private:
        uint32_t m_selected_document = -1;

        std::vector<Document*> m_docs;

        ImFont* code_font = nullptr;
        Menu m_file_menu;

        bool m_creating_new_file = false;
        bool m_closing_file = false;
        bool m_no_file_to_save = false;
    };
}

#endif // !EDITOR_LAYER_H