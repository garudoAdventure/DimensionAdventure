#pragma once

#include "IGameEvent.h"
#include "Camera.h"
#include "Player.h"

class TransformDimensionEvent : public IGameEvent {
	public:
		TransformDimensionEvent(Camera* camera) : _camera(camera) {
		}
		~TransformDimensionEvent() {
			
		}
		void update() override {
			_camera->transformDimension();
		}
		void draw() override {

		}
		bool isEnd() override {
			return _camera->is2D() == PLAYER.is2D();
		}

	private:
		Camera* _camera;
};