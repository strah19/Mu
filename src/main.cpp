#include "editor_layer.h"
#include "entry_point.h"
#include "menu.h"
#include "setup_layer.h"

#define BIND_MENU_FN(fn) std::bind(fn, this, std::placeholders::_1)

namespace Iota {
    class MuApp : public Mu::Application {
    public:
        MuApp() {
            setup_layer = new SetupLayer(Menu(BIND_MENU_FN(OnMenuCall)));
            editor_layer = new EditorLayer();
            PushLayer(setup_layer);
            PushLayer(editor_layer);
        }

        void OnMenuCall(MenuEvent menu_event) {
            editor_layer->MenuEventCall(menu_event);

            if (menu_event == MenuEvent::QUIT)
                Quit();
        }
    private:
        SetupLayer* setup_layer;
        EditorLayer* editor_layer;
    };
}

Mu::Application* Mu::CreateApplication() {
    return new Iota::MuApp;
}