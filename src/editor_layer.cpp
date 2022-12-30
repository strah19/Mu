#include "editor_layer.h"
#include "application.h"
#include "imgui_stdlib.h"

const int32_t EDITOR_TAB_FLAGS = ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_TabListPopupButton | ImGuiTabBarFlags_AutoSelectNewTabs;
const int32_t TEXT_INPUT_FLAGS = ImGuiInputTextFlags_CallbackEdit | ImGuiInputTextFlags_AllowTabInput;
const uint32_t POPUP_FLAGS = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize;

namespace Iota {
    void EditorLayer::OnAttach() {
        InitializeEditorFont();
        InitializeFileMenu();
    }

    void EditorLayer::InitializeEditorFont() {
        ImGuiIO& io = ImGui::GetIO();
        code_font = io.Fonts->AddFontFromFileTTF("res/SourceCodePro-Regular.ttf", 16);
    }

    void EditorLayer::InitializeFileMenu() {
        m_file_menu = Menu(FILEMENU);

        m_file_menu.menu_items.push_back(MenuItem("New File", BIND_FN(NewFileCallback)));
        m_file_menu.menu_items.push_back(MenuItem("Open",     BIND_FN(OpenFileCallback)));
        m_file_menu.menu_items.push_back(MenuItem("Close",    BIND_FN(CloseFileCallback)));
        m_file_menu.menu_items.push_back(MenuItem("Save",     BIND_FN(SaveFileCallback)));
        m_file_menu.menu_items.push_back(MenuItem("Quit",     BIND_FN(QuitFileCallback)));

        MenuViewer::GetMenu()->AddMenu(&m_file_menu);
    }

    void EditorLayer::UpdateGui() {
        ImGui::Begin("Editor");

        DrawTabBar();

        if      (m_creating_new_file)    NewFilePopup();
        else if (m_closing_file)         CloseFilePopup();
        else if (m_no_file_to_save)      NoFile();

        ImGui::End();
    }

    void EditorLayer::DrawTabBar() {
        if (ImGui::BeginTabBar("editor tab", EDITOR_TAB_FLAGS)) {
            for (int i = 0; i < m_editor->DocumentCount(); i++) {
                if (i == m_editor->DocumentCount() - 1 && m_creating_new_file) break;
                DrawTab(i);
            }
            ImGui::EndTabBar();
        }
    }

    void EditorLayer::DrawTab(uint32_t i) {
        Document* doc = m_editor->GetDocuments()[i];

        char tabname_buf[MAX_FILENAME_LEN];
        sprintf(tabname_buf, "%s###%d", doc->name.c_str(), i);

        if (ImGui::BeginTabItem(tabname_buf)) {
            m_editor->SetSelectedDocument(i);
            ImGui::PushFont(code_font);

            ImGui::InputTextMultiline("input", &doc->content, ImGui::GetContentRegionAvail(), TEXT_INPUT_FLAGS, SetEditStatus, &doc->edited);

            UpdateEditStatus(doc);
            ImGui::PopFont();
            ImGui::EndTabItem();
        }
    }

    int EditorLayer::SetEditStatus(ImGuiInputTextCallbackData* data) {
        bool* edited = (bool*)data->UserData;
        if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit) 
            *edited = true;
        return 0;
    }

    void EditorLayer::UpdateEditStatus(Document* doc) {
        if (doc->edited && doc->name.back() != '*')
            doc->name.push_back('*');
    }

    void EditorLayer::NewFileCallback() {
        m_creating_new_file = true;
        m_newfile_name.clear();
    }

    void EditorLayer::OpenFileCallback() {
        m_editor->OpenFileFromDialog();
    }

    void EditorLayer::CloseFileCallback() {
        m_closing_file = true;
    }

    void EditorLayer::SaveFileCallback() {
        if (!m_editor->SaveSelectedDocument())
            m_no_file_to_save = true;
    }

    void EditorLayer::QuitFileCallback() {
        Mu::Application::Get().Quit();
    }

    void EditorLayer::NewCenterPopup(const char* name) {
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::OpenPopup(name);
    }

    void EditorLayer::NewFilePopup() {
        NewCenterPopup("New File");
        if (!ImGui::BeginPopupModal("New File", NULL, POPUP_FLAGS)) return;

        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) {
            CloseNewFilePopup();
            return; 
        }

        if (!ImGui::InputText("Name", &m_newfile_name, ImGuiInputTextFlags_EnterReturnsTrue)) return;

        if (!m_newfile_name.empty()) {
            m_editor->CreateNewFileFromDialog(m_newfile_name);
        }

        CloseNewFilePopup();

        ImGui::EndPopup();
    }

    void EditorLayer::CloseNewFilePopup() {
        ImGui::CloseCurrentPopup();
        m_creating_new_file = false;     
    }

    void EditorLayer::CloseFilePopup() {
        const char* message_title = (!m_editor->IsThereSelectedDocument()) ? "No File Selected" : "Close File";
        const char* message = (!m_editor->IsThereSelectedDocument()) ? "No file selected to close." :
            (m_editor->SelectedFileEdited()) ? "Do you want to save and close this file?" : "Are you sure you want to close this file?";
        const char* btn_name = (!m_editor->IsThereSelectedDocument()) ? "Back" : "Cancel";

        NewCenterPopup(message_title);
        if (!ImGui::BeginPopupModal(message_title, NULL, POPUP_FLAGS)) return;

        ImGui::Text(message);
        if (m_editor->IsThereSelectedDocument()) DrawSaveAndClose();
        
        ImGui::SameLine();
        if (ImGui::Button(btn_name)) {
            ImGui::CloseCurrentPopup();
            m_closing_file = false;
        }
        ImGui::EndPopup();
    }

    void EditorLayer::DrawSaveAndClose() {
        if (m_editor->SelectedFileEdited()) {
            if (ImGui::Button("Save and Close")) {
                if (m_editor->SaveSelectedDocument())
                    CloseSelectedFile();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Close")) {
            CloseSelectedFile();
        }
    }

    void EditorLayer::CloseSelectedFile() {
        m_editor->CloseSelectedDocument();
        ImGui::CloseCurrentPopup();
        m_closing_file = false;
    }

    void EditorLayer::NoFile() {
        if (m_editor->IsThereSelectedDocument()) return;

        NewCenterPopup("No Selected File");
        if (!ImGui::BeginPopupModal("No Selected File", NULL, POPUP_FLAGS)) return;
    
        ImGui::Text("No file selected to save.");
        if (ImGui::Button("Back")) {
            ImGui::CloseCurrentPopup();
            m_no_file_to_save = false;
        }
        
        ImGui::EndPopup();
    }
}