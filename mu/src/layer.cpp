#include "layer.h"
#include "log.h"
#include <algorithm>

namespace Mu {
	void LayerStack::Destroy() {
		for (Layer* layer : layers) {
			layer->OnDetach();
			MU_LOG("Deleted '%s'", layer->GetName().c_str());
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer) {
		layers.emplace(layers.begin() + insert_index, layer);
		insert_index++;
	}

	void LayerStack::PopLayer(Layer* layer) {
		auto it = std::find(layers.begin(), layers.begin() + insert_index, layer);
		if (it != layers.begin() + insert_index) {
			layer->OnDetach();
			layers.erase(it);
			insert_index--;
		}
	}
}