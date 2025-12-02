#pragma once

#include "IGameEvent.h"
#include "Player.h"
#include "LayerScreen.h"
#include "DirectX.h"

class TransformLayerEvent : public IGameEvent {
	public:
		TransformLayerEvent(LayerScreen* layerScreen) : _layerScreen(layerScreen) {
		}
		~TransformLayerEvent() {
		}
		void update() override {
			if (!_layerScreen->selectLayer()) {
				PLAYER.setLayer(_layerScreen->getCurrentLayer());
				_isEnd = true;
			}
		}
		void draw() override {
			DX3D.clear();
			_layerScreen->draw();
		}
		bool isEnd() override {
			return _isEnd;
		}

	private:
		LayerScreen* _layerScreen;
		bool _isEnd = false;
};