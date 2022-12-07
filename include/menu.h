#ifndef MENU_H
#define MENU_H

#include "mu.h"

namespace Iota {
    using MenuCallbackFn = std::function<void()>;

    struct Menu;
    struct MenuItem {
        std::string name;
        std::string shortcut;
        Menu* menu = nullptr;
        bool select = false;
        MenuCallbackFn item_callback;

        MenuItem(const std::string& name, MenuCallbackFn item_callback, const std::string& shortcut = "") : name(name), item_callback(item_callback), shortcut(shortcut) { }
    };

    struct Menu {
        std::string name;
        std::vector<MenuItem> menu_items;

        Menu(const std::string& name) : name(name) { }
        Menu() { }
    };
    
    class MenuViewer {
    public:
        MenuViewer();

        void AddMenu(Menu* menu) { m_menus.push_back(menu); }
        void DrawMenu();
        static MenuViewer* GetMenu() { return m_instance; }
    private:
        void DrawMenuItems(Menu* menu);
    private:
        static MenuViewer* m_instance;
        std::vector<Menu*> m_menus;
};
}

#endif // !MENU_H