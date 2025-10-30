#pragma once

#include "IGameEvent.h"
#include "Camera.h"
#include "Player.h"

class TransformDimensionEvent : public IGameEvent {
	public:
		TransformDimensionEvent(Camera* camera, Player* player) : _camera(camera), _player(player) {
		}
		~TransformDimensionEvent() {
			
		}
		void update() override {
			_camera->transformDimension(_player);
		}
		void draw() override {

		}
		bool isEnd() override {
			return _camera->is2D() == _player->is2D();
		}

	private:
		Camera* _camera;
		Player* _player;
};