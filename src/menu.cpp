#include "menu.h"
#include "imgui.h"

namespace Iota {
    MenuViewer* MenuViewer::m_instance = nullptr;

    MenuViewer::MenuViewer() { 
        m_instance = this;
    }

    void MenuViewer::DrawMenu() {
        if (ImGui::BeginMenuBar()) {
            for (auto& menu : m_menus) {
                if (ImGui::BeginMenu(menu->name.c_str())) {
                    DrawMenuItems(menu);
                    ImGui::EndMenu();
                }
            }
            ImGui::EndMenuBar();
        }
    }

    void MenuViewer::DrawMenuItems(Menu* menu) {
        for (auto& menu_item : menu->menu_items) {
            if (menu_item.menu) 
                DrawMenuItems(menu_item.menu);
            if (ImGui::MenuItem(menu_item.name.c_str(), menu_item.shortcut.c_str()) && !menu_item.menu) {
                menu_item.item_callback();
            }
        }
    }

    Menu* MenuViewer::GetSpecificMenu(const std::string& name) {
        for (auto& menu : m_menus) 
            if (menu->name == name)
                return menu;
        return nullptr;
    }

    void MenuViewer::UpdateMenu(Menu* menu) {
        for (auto& finder_menu : m_menus) 
            if (finder_menu->name == menu->name) 
                finder_menu = menu;
    }
}