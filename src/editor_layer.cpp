#include "editor_layer.h"
#include "application.h"
#include "imgui_stdlib.h"

#define MAX_FILENAME_LEN 256

namespace Iota {
    void EditorLayer::UpdateGui() {
        ImGui::Begin("Editor");

        if (ImGui::BeginTabBar("editor tab", ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_TabListPopupButton)) {
            for (int i = 0; i < m_in_editor_files.size(); i++) {
                if (i == m_in_editor_files.size() - 1 && m_creating_new_file)
                    break;

                if (ImGui::BeginTabItem(m_in_editor_files[i].c_str())) {
                    m_selected_document = i;
                    ImGui::InputTextMultiline("input", &m_in_editor_data[i], ImGui::GetContentRegionAvail());
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }

        if (m_creating_new_file) {
            NewFile();
        }
        else if (m_closing_file) {
            CloseFile();
        }


        ImGui::End();
    }

    void EditorLayer::MenuEventCall(MenuEvent menu_event) {
        if (menu_event == MenuEvent::NEW) {
            m_creating_new_file = true;

            m_in_editor_files.push_back(std::string());
            m_in_editor_data.push_back(std::string());
        }
        else if (menu_event == MenuEvent::CLOSE) {
            m_closing_file = true;
        }
    }

    void EditorLayer::NewFile() {
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::OpenPopup("New File");
        if (ImGui::BeginPopupModal("New File", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::InputText("Name", &m_in_editor_files.back());
            if (ImGui::Button("Enter")) {
                if (m_in_editor_files.back().size() == 0) {
                    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                    ImGui::OpenPopup("No Name Given");
                }
                else {
                    ImGui::CloseCurrentPopup();
                    m_creating_new_file = false;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                ImGui::CloseCurrentPopup();
                m_creating_new_file = false;
                m_in_editor_files.pop_back();
                m_in_editor_data.pop_back();
            }

            if (ImGui::BeginPopupModal("No Name Given", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)){
                ImGui::Text("Name for new file cannot be nothing.");
                if (ImGui::Button("Close"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }

        ImGui::EndPopup();
        }
    }

    void EditorLayer::CloseFile() {
        const char* message_title = (m_selected_document == -1) ? "No File Selected" : "Close File";
        const char* message = (m_selected_document == -1) ? "No file selected to close." : "Are you sure you want to close this file?";

        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::OpenPopup(message_title);
        if (ImGui::BeginPopupModal(message_title, NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)){
            ImGui::Text(message);
            if (ImGui::Button("Close")) {
                if (m_selected_document != -1) {
                    m_in_editor_data.erase(m_in_editor_data.begin() + m_selected_document);
                    m_in_editor_files.erase(m_in_editor_files.begin() + m_selected_document);
                    m_selected_document = -1;
                }
                ImGui::CloseCurrentPopup();
                m_closing_file = false;
            }
            if (m_selected_document != -1) {
                ImGui::SameLine();
                if (ImGui::Button("Cancel")) {
                    ImGui::CloseCurrentPopup();
                    m_closing_file = false;
                }
            }
            ImGui::EndPopup();
        }
    }
}