#include "editor_layer.h"
#include "application.h"
#include "imgui_stdlib.h"

#define MAX_FILENAME_LEN 256

namespace Iota {
    void EditorLayer::OnAttach() {
        ImGuiIO& io = ImGui::GetIO();
        code_font = io.Fonts->AddFontFromFileTTF("../res/SourceCodePro-Regular.ttf", 16);

        m_file_menu = Menu("File");

        m_file_menu.menu_items.push_back(MenuItem("New File", BIND_FN(NewFileCallback)));
        m_file_menu.menu_items.push_back(MenuItem("Open",     BIND_FN(OpenFileCallback)));
        m_file_menu.menu_items.push_back(MenuItem("Close",    BIND_FN(CloseFileCallback)));
        m_file_menu.menu_items.push_back(MenuItem("Save",     BIND_FN(SaveFileCallback)));

        MenuViewer::GetMenu()->AddMenu(&m_file_menu);

    }
    void EditorLayer::UpdateGui() {
        ImGui::Begin("Editor");

        if (ImGui::BeginTabBar("editor tab", ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_TabListPopupButton | ImGuiTabBarFlags_AutoSelectNewTabs)) {
            for (int i = 0; i < m_docs.size(); i++) {
                if (i == m_docs.size() - 1 && m_creating_new_file)
                    break;

                char tabname_buf[MAX_FILENAME_LEN];
                sprintf(tabname_buf, "%s###%d", m_docs[i]->name.c_str(), i);
                if (ImGui::BeginTabItem(tabname_buf)) {
                    m_selected_document = i;
                    ImGui::PushFont(code_font);
                    ImGui::InputTextMultiline("input", &m_docs[i]->content, ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_CallbackEdit, [](ImGuiInputTextCallbackData* data) -> int {
                        bool* edited = (bool*)data->UserData;
                        if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit) 
                            *edited = true;
                        return 0;
                    }, &m_docs[i]->edited);

                    if (m_docs[i]->edited && m_docs[i]->name.back() != '*')
                        m_docs[i]->name += '*';

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

    void EditorLayer::OnDetach() {
        for (auto& doc : m_docs) {
            delete doc;
        }
    }

    void EditorLayer::NewFileCallback() {
        m_creating_new_file = true;
        m_docs.push_back(new Document());
    }

    void EditorLayer::OpenFileCallback() {
        std::string path = Mu::FileDialogs::Open("Mu Environment (*.lua *.cpp)\0*.lua;*.cpp\0");
        if (!path.empty()) {
            m_docs.push_back(new Document());
            m_docs.back()->file.Open(path.c_str());
            m_docs.back()->content = m_docs.back()->file.Read();
            m_docs.back()->name = m_docs.back()->file.Path();
        }
    }

    void EditorLayer::CloseFileCallback() {
        m_closing_file = true;
    }

    void EditorLayer::SaveFileCallback() {
        if (!SaveSelectedFile())
            m_no_file_to_save = true;
    }

    void EditorLayer::NewCenterPopup(const char* name) {
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::OpenPopup(name);
    }

    void EditorLayer::NewFile() {
        NewCenterPopup("New File");
        if (ImGui::BeginPopupModal("New File", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::InputText("Name", &m_docs.back()->name);
            if (ImGui::Button("Enter")) {
                if (m_docs.back()->name.size() == 0) 
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
                m_docs.pop_back();
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
        const char* message_title = (m_selected_document == -1) ? "No File Selected" : "Close File";
        const char* message = (m_selected_document == -1) ? "No file selected to close." : (m_docs[m_selected_document]->edited) ? "Do you want to save and close this file?" : "Are you sure you want to close this file?";
        const char* btn_name = (m_selected_document == -1) ? "Back" : "Cancel";

        NewCenterPopup(message_title);
        if (ImGui::BeginPopupModal(message_title, NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text(message);
            if (m_selected_document != -1) {
                Document* doc = m_docs[m_selected_document];
                if (doc->edited) {
                    if (ImGui::Button("Save and Close")) {
                        if (SaveSelectedFile())
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
        delete m_docs[m_selected_document];
        m_docs.erase(m_docs.begin() + m_selected_document);
        m_selected_document = -1;
        ImGui::CloseCurrentPopup();
        m_closing_file = false;
    }

    void EditorLayer::NoFile() {
        if (m_selected_document == -1) {
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

    bool EditorLayer::SaveSelectedFile() {
        bool successful = false;

        if (m_selected_document != -1) {
            Document* doc = m_docs[m_selected_document];
            if (doc->file.IsOpen()) {
                doc->file.Empty();
                doc->file.Write(doc->content);
                successful = true;
            }
            else {
                std::string path = Mu::FileDialogs::Save("Mu Environment (*.lua *.cpp)\0*.lua;*.cpp\0");
                if (!path.empty()) {
                    doc->file.Open(path.c_str());
                    doc->file.Empty();
                    doc->file.Write(doc->content);
                successful = true;
                }
            }
        }

        if (successful && m_docs[m_selected_document]->edited) {
            m_docs[m_selected_document]->edited = false;
            m_docs[m_selected_document]->name.pop_back();
        }

        return successful;
    }
}