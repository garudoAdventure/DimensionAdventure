#pragma once

#include "GameObj.h"
#include "Texture.h"
#include "Sprite.h"
#include "MathStruct.h"
#include "MathTool.h"
#include "Model.h"
#include "ModelManager.h"
#include "IGameEventHandler.h"
#include <string>

enum class ItemTag {
	RING,
	CRYSTAL,
	REMOTE_CONTROL
};

class Item : public GameObj {
	public:
		Item(Float3 pos, IGameEventHandler* gameEvent) : _gameEvent(gameEvent) {
			_pos = pos;
			_color = { 1.0f, 1.0f, 1.0f, 1.0f };
		}
		void update() override {
			_pos.y += cosf(_passTime * 0.05f) * 0.02f;
			_passTime++;
		}
		void draw() override {}
		void collide(GameObj* obj, bool is2D) override {
			if (obj->getTag() != ObjTag::PLAYER_TAG) return;
			if (MathTool::checkCollision(this->getBox(), obj->getBox(), is2D)) {
				_isActive = false;
				getItem();
			}
		}
		virtual ItemTag getTag() = 0;
		virtual std::wstring getName() = 0;
		virtual void getItem() = 0;

	protected:
		IGameEventHandler* _gameEvent;

	private:
		int _passTime = 0;
};