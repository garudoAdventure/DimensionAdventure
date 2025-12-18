#pragma once

#include "GameObj.h"
#include "MathStruct.h"
#include "MathTool.h"

class PlayerFallPoint : public GameObj {
	public:
		PlayerFallPoint(Float3 pos, Float3 size, Float3 playerReturnPos) : _playerReturnPos(playerReturnPos) {
			_pos = pos;
			_size = size;
			_tag = ObjTag::PLAYER_FALL_POINT;
		}
		void collide(GameObj* obj, bool is2D) override {
			if (MathTool::checkCollision(this->getBox(), obj->getBox(), is2D)) {
				obj->hitObj(this);
				obj->setPos(_playerReturnPos);
			}
		}
		void draw() override {
			// 描画しない
		}

	private:
		Float3 _playerReturnPos;
};