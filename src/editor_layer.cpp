#include "editor_layer.h"
#include "application.h"
#include "imgui_stdlib.h"

#define MAX_FILENAME_LEN 256

namespace Iota {
    void EditorLayer::OnAttach() {
        ImGuiIO& io = ImGui::GetIO();
        code_font = io.Fonts->AddFontFromFileTTF("res/SourceCodePro-Regular.ttf", 16);

        m_file_menu = Menu("File");

        m_file_menu.menu_items.push_back(MenuItem("New File", BIND_FN(NewFileCallback)));
        m_file_menu.menu_items.push_back(MenuItem("Open",     BIND_FN(OpenFileCallback)));
        m_file_menu.menu_items.push_back(MenuItem("Close",    BIND_FN(CloseFileCallback)));
        m_file_menu.menu_items.push_back(MenuItem("Save",     BIND_FN(SaveFileCallback), "Ctrl+S"));
        m_file_menu.menu_items.push_back(MenuItem("Save As",  BIND_FN(SaveAsFileCallback)));
        m_file_menu.menu_items.push_back(MenuItem("Quit",     BIND_FN(QuitFileCallback), "Alt+F4"));

        MenuViewer::GetMenu()->AddMenu(&m_file_menu);
    }

    void EditorLayer::UpdateGui() {
        ImGui::Begin("Editor");

        if (ImGui::BeginTabBar("editor tab", ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_TabListPopupButton | ImGuiTabBarFlags_AutoSelectNewTabs)) {
            for (int i = 0; i < m_editor->DocumentCount(); i++) {
                if (i == m_editor->DocumentCount() - 1 && m_creating_new_file)
                    break;
                Document* doc = m_editor->GetDocuments()[i];
                char tabname_buf[MAX_FILENAME_LEN];
                sprintf(tabname_buf, "%s###%d", doc->name.c_str(), i);
                if (ImGui::BeginTabItem(tabname_buf)) {
                    m_editor->SetSelectedDocument(i);
                    ImGui::PushFont(code_font);
                    ImGui::InputTextMultiline("input", &doc->content, ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_CallbackEdit | ImGuiInputTextFlags_AllowTabInput, [](ImGuiInputTextCallbackData* data) -> int {
                        bool* edited = (bool*)data->UserData;
                        if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit) 
                            *edited = true;
                        return 0;
                    }, & doc->edited);

                    if (doc->edited && doc->name.back() != '*')
                        doc->name += '*';

                    ImGui::PopFont();
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }

        if (m_creating_new_file) 
            NewFile();
        else if (m_closing_file)
            CloseFile();
        else if (m_no_file_to_save)
            NoFile();

        ImGui::End();
    }

    void EditorLayer::NewFileCallback() {
        m_creating_new_file = true;
        m_editor->CreateBlankDocument();
    }

    void EditorLayer::OpenFileCallback() {
        m_editor->CreateDocumentFromFileDialog();
    }

    void EditorLayer::CloseFileCallback() {
        m_closing_file = true;
    }

    void EditorLayer::SaveFileCallback() {
        if (!m_editor->SaveSelectedDocument())
            m_no_file_to_save = true;
    }

    void EditorLayer::SaveAsFileCallback() {
        if (!m_editor->SaveAsSelectedDocument())
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

    void EditorLayer::NewFile() {
        NewCenterPopup("New File");
        if (ImGui::BeginPopupModal("New File", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::InputText("Name", &m_editor->GetDocuments().back()->name);
            if (ImGui::Button("Enter")) { 
                if (m_editor->GetDocuments().back()->name.size() == 0)
                    NewCenterPopup("No Name Given");
                else {
                    ImGui::CloseCurrentPopup();
                    m_creating_new_file = false;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                ImGui::CloseCurrentPopup();
                m_creating_new_file = false;
                m_editor->PopDocument();
            }
            if (ImGui::BeginPopupModal("No Name Given", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::PushStyleColor(ImGuiCol_Text, {1.0f, 0.0f, 0.0f, 1.0f});
                ImGui::Text("Name for new file cannot be nothing.");
                ImGui::PopStyleColor();
                if (ImGui::Button("Close"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }

        ImGui::EndPopup();
        }
    }

    void EditorLayer::CloseFile() {
        const char* message_title = (!m_editor->IsThereSelectedDocument()) ? "No File Selected" : "Close File";
        const char* message = (!m_editor->IsThereSelectedDocument()) ? "No file selected to close." :
            (m_editor->SelectedFileEdited()) ? "Do you want to save and close this file?" : "Are you sure you want to close this file?";
        const char* btn_name = (!m_editor->IsThereSelectedDocument()) ? "Back" : "Cancel";

        NewCenterPopup(message_title);
        if (ImGui::BeginPopupModal(message_title, NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text(message);
            if (m_editor->IsThereSelectedDocument()) {
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
            ImGui::SameLine();
            if (ImGui::Button(btn_name)) {
                ImGui::CloseCurrentPopup();
                m_closing_file = false;
            }
            ImGui::EndPopup();
        }
    }

    void EditorLayer::CloseSelectedFile() {
        m_editor->CloseSelectedDocument();
        ImGui::CloseCurrentPopup();
        m_closing_file = false;
    }

    void EditorLayer::NoFile() {
        if (!m_editor->IsThereSelectedDocument()) {
            NewCenterPopup("No Selected File");
            if (ImGui::BeginPopupModal("No Selected File", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("No file selected to save.");
                if (ImGui::Button("Back")) {
                    ImGui::CloseCurrentPopup();
                    m_no_file_to_save = false;
                }
            }
            ImGui::EndPopup();
        }
    }
}