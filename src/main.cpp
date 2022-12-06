#include "editor_layer.h"
#include "entry_point.h"
#include "menu.h"
#include "setup_layer.h"
#include "widget_layer.h"

namespace Iota {
    class MuApp : public Mu::Application {
    public:
        MuApp() {
            setup_layer = new SetupLayer();
            editor_layer = new EditorLayer();
           // widget_layer = new WidgetLayer();

            PushLayer(setup_layer);
            PushLayer(editor_layer);
        }

    private:
        MenuViewer menu_viewer;
        SetupLayer* setup_layer;
        EditorLayer* editor_layer;
        WidgetLayer* widget_layer;
    };
}

Mu::Application* Mu::CreateApplication() {
    return new Iota::MuApp;
}