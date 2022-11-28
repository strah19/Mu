#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include "layer.h"

namespace Mu {
    class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();

		virtual void OnAttach();
		void OnDetach();

		void Begin();
		void End();
	};
}

#endif // !IMGUI_LAYER_H