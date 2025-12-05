#pragma once

#include "GameObj.h"
#include "ActivableGameObj.h"
#include "Model.h"
#include "ModelManager.h"
#include "Shader.h"
#include "DirectX.h"
#include "MathTool.h"
#include "Color.h"

class Block : public ActivableGameObj {
	public:
		Block(Float3 pos, Float3 scale, Float4 color) : _activeColor(color) {
			_pos = pos;
			_scale = scale;
			_model = MODEL.loadModel("./assets/model/transposeBox.fbx");
			_size = _model->getSize();
			_size.x *= scale.x;
			_size.y *= scale.y;
			_size.z *= scale.z;
			_triggerSize = { _size.x + 0.1f, _size.y + 0.1f, _size.z + 0.1f };
			_inactiveColor = Color::white;
			_inactiveColor.a = 0.3f;
		}
		void update() override {
			GameObj::update();
			ActivableGameObj::update();
			_color = MathTool::lerp<Float4>(_inactiveColor, _activeColor, count / 60.0f);
			count = count < 0 ? 0 : count - 1;
		}
		void draw() override {
			Light light;
			light.enable = false;
			SHADER.setLight(light);
			_model->updateColor(_color);
			_model->draw(
				_pos, { 0.0f, 0.0f, 0.0f }, _scale
			);
		}
		void onTrigger(GameObj* player) override {
			player->hitObj(this);
			count = 60;
		}
	
	private:
		Model* _model;
		Float3 _scale;
		Float4 _activeColor;
		Float4 _inactiveColor;
		int count = 0;
};

class MovingFloor : public Block {
	public:
		MovingFloor(Float3 pos, Float3 scale, Float4 color, Float3 vel) : Block(pos, scale, color) {
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
		MovableBox(Float3 pos, Float3 scale, Float4 color) : Block(pos, scale, color) {
		}
};