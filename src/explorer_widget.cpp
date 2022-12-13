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

    ExplorerWidget::ExplorerWidget() : Widget("Explorer") {
        Menu* file_menu = MenuViewer::GetMenu()->GetSpecificMenu("File");
        if (file_menu) {
            file_menu->menu_items.insert(file_menu->menu_items.end() - 1, MenuItem("Open Folder", BIND_FN(OpenFolderCallback)));
            MenuViewer::GetMenu()->UpdateMenu(file_menu);
        }
    }

    void ExplorerWidget::OpenFolderCallback() {
        std::string folderpath;
        folderpath.reserve(256);
        if (Mu::FileDialogs::BrowseFolder(folderpath.data(), 256, "C:/Users/User")) {
            MU_LOG("Folder path '%s' opened", folderpath.c_str());
            m_project.InitializeProject(folderpath);
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

        }

        ImGui::End();
    }
}