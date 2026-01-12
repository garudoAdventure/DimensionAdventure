#pragma once

#include "GameObj.h"
#include "MathTool.h"

class Wall : public GameObj {
	public:
		Wall(Float3 pos, Float3 size) {
			_pos = pos;
			_size = size;
		}
		void collide(GameObj* obj, bool is2D) override {
			if (MathTool::checkCollision(this->getBox(), obj->getBox(), false)) {
				obj->hitObj(this);
			}
		}
		void draw() override {
			// 描画しない
		}
};