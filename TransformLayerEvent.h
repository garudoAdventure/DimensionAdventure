#pragma once

#include "IGameEvent.h"
#include "Player.h"
#include "LayerScreen.h"
#include "DirectX.h"

class TransformLayerEvent : public IGameEvent {
	public:
		TransformLayerEvent(LayerScreen* layerScreen) : _layerScreen(layerScreen) {
		}
		void update() override {
			_layerScreen->update();
			if (!_layerScreen->selectLayer()) {
				PLAYER.setLayer(_layerScreen->getCurrentLayer());
				_isEnd = true;
			}
		}
		void draw() override {
			DX3D.setTargetView();
			DX3D.clear({0.0f, 0.0f, 0.0f, 1.0f});
			_layerScreen->draw();
		}
		bool isEnd() override {
			return _isEnd;
		}

	private:
		LayerScreen* _layerScreen;
		bool _isEnd = false;
};