#pragma once

#include "GameObj.h"
#include "Model.h"
#include "Shader.h"

class Block : public GameObj {
	public:
		Block(Float3 pos, Model* model) : _model(model) {
			_pos = pos;
			_size = model->getSize();
			_color = { 1.0f, 1.0f, 1.0f, 0.3f };
		}
		void draw() override {
			Light light;
			light.enable = false;
			SHADER.setLight(light);
			_model->draw(
				_pos, { 0.0f, 0.0f, 0.0f }
			);
		}
	
	private:
		Model* _model;
};

class MovingFloor : public Block {
	public:
		MovingFloor(Float3 pos, Float3 vel, Model* model) : Block(pos, model) {
			_vel = vel;
		}
		void update() override {
			if (count == 300) {
				_vel.x = -_vel.x;
				count = 0;
			}
			count++;

			_pos += _vel;
		}
	
	private:
		int count = 0;
};

class MovableBox : public Block {
	public:
		MovableBox(Float3 pos, Model* model) : Block(pos, model) {
		}
};