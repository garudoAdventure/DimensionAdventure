#pragma once

#include "IGameEvent.h"
#include "LayerSwitcher.h"
#include "./DirectX/DirectX.h"
#include "./Player/Player.h"

class TransformLayerEvent : public IGameEvent {
	public:
		TransformLayerEvent(LayerSwitcher* layerScreen) : _layerSwitcher(layerScreen) {
			_layerSwitcher->init();
		}

		void update() override {
			_layerSwitcher->update();
			if (_layerSwitcher->isSelectLayerEnd()) {
				PLAYER.setLayer(_layerSwitcher->getCurrentLayer());
				_isEnd = true;
			}
		}

		void draw() override {
			DX3D.setTargetView();
			DX3D.clear({ 0.0f, 0.0f, 0.0f, 1.0f });
			_layerSwitcher->draw();
		}

		bool isEnd() override {
			return _isEnd;
		}

	private:
		LayerSwitcher* _layerSwitcher;
		bool _isEnd = false;
};