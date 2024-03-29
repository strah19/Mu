#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "mu.h"
#include "menu.h"
#include <vector>
#include "imgui.h"
#include "editor.h"

namespace Iota {
    class EditorLayer : public Mu::Layer {
    public:
        EditorLayer(Editor* editor) : Mu::Layer ("Editor Layer"), m_editor(editor) { }

        void OnAttach();
        void UpdateGui();
    private:
        void InitializeFileMenu();
        void InitializeEditorFont();

        void DrawTabBar();
        void DrawTab(uint32_t i);

        void NewFilePopup();
        void CloseNewFilePopup();

        void CloseFilePopup();
        void CloseSelectedFile();
        void DrawSaveAndClose();
        void NoFile();

        void NewFileCallback();
        void OpenFileCallback();
        void CloseFileCallback();
        void SaveFileCallback();
        void QuitFileCallback();

        void NewCenterPopup(const char* name);
        void UpdateEditStatus(Document* doc);
        static int SetEditStatus(ImGuiInputTextCallbackData* data);
    private:
        Editor* m_editor;
        ImFont* code_font = nullptr;
        Menu m_file_menu;

        std::string m_newfile_name;

        bool m_creating_new_file = false;
        bool m_closing_file = false;
        bool m_no_file_to_save = false;
    };
}

#endif // !EDITOR_LAYER_H