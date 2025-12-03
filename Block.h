#pragma once

#include "GameObj.h"
#include "ActivableGameObj.h"
#include "Model.h"
#include "Shader.h"
#include "DirectX.h"
#include "MathTool.h"

class Block : public ActivableGameObj {
	public:
		Block(Float3 pos, Model* model) : _model(model) {
			_pos = pos;
			_size = model->getSize();
			_triggerSize = { _size.x + 0.1f, _size.y + 0.1f, _size.z + 0.1f };
			_color = { 0.54f, 1.0f, 0.43f, 1.0f };
		}
		void update() override {
			GameObj::update();
			ActivableGameObj::update();
			_color = MathTool::lerp<Float4>(normalColor, lightColor, count / 60.0f);
			count = count < 0 ? 0 : count - 1;
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
			count = 60;
		}
	
	private:
		Model* _model;
		int count = 0;
		const Float4 lightColor = { 0.54f, 1.0f, 0.43f, 1.0f };
		const Float4 normalColor = { 1.0f, 1.0f, 1.0f, 0.3f };
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