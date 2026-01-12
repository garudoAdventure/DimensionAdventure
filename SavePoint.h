#pragma once

#include "GameObj.h"
#include "MathStruct.h"
#include "MathTool.h"
#include "IGameEventHandler.h"

class SavePoint : public GameObj {
	public:
		SavePoint(Float3 pos, Float3 size, IGameEventHandler* gameEvent) : _gameEvent(gameEvent) {
			_pos = pos;
			_size = size;
		}
		void collide(GameObj* obj, bool is2D) override {
			if (MathTool::checkCollision(this->getBox(), obj->getBox(), is2D)) {
				_gameEvent->setSavePointPos(_pos);
			}
		}
		void draw() override {
			// •`‰æ‚µ‚È‚¢
		}

	private:
		IGameEventHandler* _gameEvent;
};