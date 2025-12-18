#pragma once

#include "GameObj.h"
#include "ActivableGameObj.h"
#include "Model.h"
#include "Shader.h"
#include "DirectX.h"
#include "MathTool.h"
#include "Color.h"

class Block : public ActivableGameObj {
	public:
		Block(Float3 pos, Float3 scale, Float4 color, Model* model) :
			_scale(scale), _model(model), _activeColor(color)
		{
			_pos = pos;
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
			_color = MathTool::lerp<Float4>(_inactiveColor, _activeColor, _count / 60.0f);
			_count = std::max(0, _count - 1);
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
			if (MathTool::checkCollision(player->getBox(), this->getBox(), PLAYER.is2D())) {
				player->hitObj(this);
			}
			_count = 60;
		}
	
	protected:
		Model* _model;
		Float3 _scale;
		Float4 _activeColor;
		Float4 _inactiveColor;
		int _count = 0;
};

class HintBlock : public Block {
	public:
		HintBlock(Float3 pos, Float3 scale, Float4 color, Model* model) :
			Block(pos, scale, color, model) {
		}
		void update() override {
			Block::update();
		}
		void draw() override {
			ActivableGameObj::drawHint({ _pos.x, _pos.y + _size.y / 2 + 1.0f, _pos.z });
			Block::draw();
		}
		void onTrigger(GameObj* player) override {
			Block::onTrigger(player);
			if (Keyboard_IsKeyTrigger(KK_ENTER)) {
				PLAYER.getSpirit()->introHintBlock();
			}
		}
};

class MovingFloor : public Block {
	public:
		MovingFloor(Float3 startPos, Float3 endPos, Float3 scale, Float4 color, Model* model) :
			Block(startPos, scale, color, model), _startPos(startPos), _endPos(endPos)
		{
			float dx = endPos.x - startPos.x;
			float dy = endPos.y - startPos.y;
			float dz = endPos.z - startPos.z;
			_distance = sqrtf(dx * dx + dy * dy + dz * dz);
			_tag = ObjTag::MOVING_FLOOR;
		}
		void update() override {
			Block::update();
			_pos = MathTool::lerp<Float3>(_startPos, _endPos, _count / (_distance * 8));
			_vel = _pos - _oldPos;
			if (_count == 0 || _count == (int)_distance * 8) {
				_countAdd = -_countAdd;
			}
			_count += _countAdd;
		}
	
	private:
		Float3 _startPos;
		Float3 _endPos;
		int _count = 0;
		int _countAdd = -1;
		float _distance;
};

class MovableBox : public Block {
	public:
		MovableBox(Float3 pos, Float3 scale, Float4 color, Model* model) : Block(pos, scale, color, model) {
			_color = color;
		}
		void update() override {
			GameObj::update();
			ActivableGameObj::update();
		}
};