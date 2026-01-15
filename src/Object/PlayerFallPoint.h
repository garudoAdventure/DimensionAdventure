#pragma once

#include "GameObj.h"
#include "./Game/IGameEventHandler.h"
#include "./Common/MathStruct.h"
#include "./Utils/MathTool.h"

class PlayerFallPoint : public GameObj {
	public:
		PlayerFallPoint(Float3 pos, Float3 size, IGameEventHandler* gameEvent) : _gameEvent(gameEvent) {
			_pos = pos;
			_size = size;
			_tag = ObjTag::PLAYER_FALL_POINT;
		}
		void collide(GameObj* obj, bool is2D) override {
			if (MathTool::checkCollision(this->getBox(), obj->getBox(), is2D)) {
				obj->setPos(_gameEvent->getSavePointPos());
			}
		}
		void draw() override {
			// 描画しない
		}

	private:
		IGameEventHandler* _gameEvent;
};