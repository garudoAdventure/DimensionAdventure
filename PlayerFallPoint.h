#pragma once

#include "GameObj.h"
#include "MathStruct.h"
#include "MathTool.h"
#include "IGameEventHandler.h"

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
			// •`‰æ‚µ‚È‚¢
		}

	private:
		IGameEventHandler* _gameEvent;
};