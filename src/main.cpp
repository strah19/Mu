#include "editor_layer.h"
#include "entry_point.h"

class Iota : public Mu::Application {
public:
    Iota() {
     	PushLayer(new Mu::EditorLayer());   
    }
private:

};

Mu::Application* Mu::CreateApplication() {
    return new Iota;
}