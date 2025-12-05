#pragma once

#include "GameObj.h"
#include "Texture.h"
#include "Sprite.h"
#include "MathStruct.h"
#include "Model.h"
#include "IGameEventHandler.h"
#include <string>

enum class ItemTag {
	KEY,
	BANGLE,
	CRYSTAL,
};

class Item : public GameObj {
	public:
		Item(Float3 pos, IGameEventHandler* gameEvent) : _gameEvent(gameEvent) {
			_pos = pos;
			_color = { 1.0f, 1.0f, 1.0f, 1.0f };
			_model = new Model("./assets/model/cube.fbx");
			_size = _model->getSize();
		}
		void update() override {
			_pos.y += cosf(passTime * 0.05f) * 0.02f;
			passTime++;
		}
		void draw() override {
			_model->draw(_pos, { 0.0f, 0.0f, 0.0f });
		}
		void collide(GameObj* obj, bool is2D) override {
			if (obj->getTag() != ObjTag::PLAYER_TAG) return;
			if (MathTool::checkCollision(this->getBox(), obj->getBox(), is2D)) {
				_isActive = false;
				getItem();
			}
		}
		virtual ItemTag getTag() = 0;
		virtual std::string getName() = 0;
		virtual void getItem() = 0;

	protected:
		IGameEventHandler* _gameEvent;
		unsigned int texID;
		Model* _model;

	private:
		int passTime = 0;
};