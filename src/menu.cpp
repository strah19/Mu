#include "menu.h"
#include "imgui.h"

namespace Iota {
    Menu::Menu(MenuCallbackFn menu_callback) : m_menu_callback(menu_callback) { }

    void Menu::DrawMenu() {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New File", "Ctrl + N"))
                    m_menu_callback(MenuEvent::NEW);
                if (ImGui::MenuItem("Open File", "Ctrl + O"))
                    m_menu_callback(MenuEvent::OPEN);
                if (ImGui::BeginMenu("Open Recent")) {
                    if (ImGui::MenuItem("Clear Recently Opened")) {
                        
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Close"))
                    m_menu_callback(MenuEvent::CLOSE);
                if (ImGui::MenuItem("Save", "Ctrl + S")) 
                    m_menu_callback(MenuEvent::SAVE);
                if (ImGui::MenuItem("Quit", "Alt + F4")) 
                    m_menu_callback(MenuEvent::QUIT);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help")) {
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }
}