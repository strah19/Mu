#ifndef MENU_H
#define MENU_H

#include "mu.h"

namespace Iota {
    enum MenuEvent{
        QUIT, OPEN, NEW, CLOSE, SAVE
    };
    
    using MenuCallbackFn = std::function<void(MenuEvent)>;
    class Menu {
    public:
        Menu(MenuCallbackFn menu_callback);

        void DrawMenu();
    private:
        MenuCallbackFn m_menu_callback;
    };
}

#endif // !MENU_H