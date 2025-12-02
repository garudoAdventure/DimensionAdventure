#pragma once

#include "GameObj.h"
#include "ActivableGameObj.h"
#include "Model.h"
#include "Shader.h"
#include "DirectX.h"

class Block : public ActivableGameObj {
	public:
		Block(Float3 pos, Model* model) : _model(model) {
			_pos = pos;
			_size = model->getSize();
			_triggerSize = { _size.x + 1.0f, _size.y + 1.0f, _size.z + 1.0f };
			_color = { 0.54f, 1.0f, 0.43f, 1.0f };
		}
		void update() override {
			GameObj::update();
			ActivableGameObj::update();
			_color = { 1.0f, 1.0f, 1.0f, 0.3f };
		}
		void draw() override {
			Light light;
			light.enable = false;
			SHADER.setLight(light);
			_model->updateColor(_color);
			_model->draw(
				_pos, { 0.0f, 0.0f, 0.0f }
			);
		}
		void onTrigger(GameObj* player) override {
			player->hitObj(this);
			_color = { 0.54f, 1.0f, 0.43f, 1.0f };
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