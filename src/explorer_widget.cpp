#include "explorer_widget.h"
#include "menu.h"
#include "imgui.h"

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
        Mu::FileDialogs::BrowseFolder("C:/Users/User");
    }

    void ExplorerWidget::Update() {
        ImGui::Begin(m_name.c_str());

        if (m_current_folder_open.empty()) {
            ImGui::TextWrapped(NO_FOLDER);
        
            ImGui::Separator();
            ImGui::TextWrapped(NO_FOLDER_MSG);
            ImGui::NewLine();
            float window_width = ImGui::GetWindowSize().x;
            float text_width   = ImGui::CalcTextSize(OPEN_FOLDER_TEXT).x;
            ImGui::SetCursorPosX((window_width - text_width) * 0.5f);
            if (ImGui::Button(OPEN_FOLDER_TEXT)) {

            }
            ImGui::NewLine();
            ImGui::TextWrapped(OPEN_FOLDER_MSG);
        }

        ImGui::End();
    }
}