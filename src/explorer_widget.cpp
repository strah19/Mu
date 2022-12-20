#include "explorer_widget.h"
#include "menu.h"
#include "imgui.h"
#include "log.h"
#include "project.h"

namespace Iota {
    const char* NO_FOLDER = "No Folder Opened";
    const char* NO_FOLDER_MSG = "You have not opened a folder yet.";
    const char* OPEN_FOLDER_TEXT = "Open Folder";
    const char* OPEN_FOLDER_MSG = "Opening a folder will close all current files.";

    ExplorerWidget::ExplorerWidget(Editor* editor) : Widget("Explorer"), m_editor(editor) {
        Menu* file_menu = MenuViewer::GetMenu()->GetSpecificMenu("File");
        if (file_menu) {
            file_menu->menu_items.insert(file_menu->menu_items.end() - 1, MenuItem("Open Folder",  BIND_FN(OpenFolderCallback)));
            file_menu->menu_items.insert(file_menu->menu_items.end() - 1, MenuItem("Close Folder", BIND_FN(CloseFolderCallback)));
            MenuViewer::GetMenu()->UpdateMenu(file_menu);
        }
    }

    void ExplorerWidget::CloseFolderCallback() {
        m_project.CloseProject();
        m_editor->CloseFiles();
    }

    void ExplorerWidget::OpenFolderCallback() {
        std::string folderpath;
        folderpath.reserve(MAX_FILENAME_LEN);
        if (Mu::FileDialogs::BrowseFolder(folderpath.data(), MAX_FILENAME_LEN, std::filesystem::current_path().string().c_str())) {
            MU_LOG("Folder path '%s' opened", folderpath.c_str());
            if (!m_project.InitializeProject(folderpath.c_str())) 
                MU_LOG("Failed to initialize project '%s'", folderpath.c_str());
            else
                MU_LOG("Initialized project '%s'", folderpath.c_str());
        }
        else
            MU_LOG("No folder selected in dialog");
    }

    void ExplorerWidget::Update() {
        ImGui::Begin(m_name.c_str());

        if (!m_project.Working()) {
            ImGui::TextWrapped(NO_FOLDER);
        
            ImGui::Separator();
            ImGui::TextWrapped(NO_FOLDER_MSG);
            ImGui::NewLine();
            float window_width = ImGui::GetWindowSize().x;
            float text_width   = ImGui::CalcTextSize(OPEN_FOLDER_TEXT).x;
            ImGui::SetCursorPosX((window_width - text_width) * 0.5f);
            if (ImGui::Button(OPEN_FOLDER_TEXT)) {
                OpenFolderCallback();
            }
            ImGui::NewLine();
            ImGui::TextWrapped(OPEN_FOLDER_MSG);
        }
        else {
            ImGui::TextWrapped(m_project.GetPath().c_str());
            ImGui::Separator();

            if (ImGui::TreeNode(m_project.GetName().c_str())) {
                DisplayDirTree(*m_project.GetDirectory());
                if (ImGui::BeginPopup("select_popup")) {
                    if (ImGui::Selectable("Open")) {
                        m_editor->CreateDocumentFromFile(m_selected_filepath);
                    }
                    ImGui::Selectable("Delete");
                    ImGui::EndPopup();    
                }
                ImGui::TreePop();
            }  
        }

        ImGui::End();
    }

    void ExplorerWidget::DisplayDirTree(const std::filesystem::path& path) {
        if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
            size_t entry_index = 0;
            for (const auto& entry : std::filesystem::directory_iterator(path)) {
                auto filename = entry.path().filename();
                if (std::filesystem::is_directory(entry.status())) {
                    if (ImGui::TreeNode(filename.string().c_str())) {
                        DisplayDirTree(entry);
                        ImGui::TreePop();
                    }
                }
                else {
                    ImGui::TreeNodeEx((void*)(intptr_t) entry_index, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, filename.string().c_str());
                    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {    
                        m_selected_filepath = filename.string().c_str();            
                        ImGui::OpenPopup("select_popup");
                    }
                }
                entry_index++;
            }
        }
    }
}