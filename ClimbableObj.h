#pragma once

#include "GameObj.h"
#include "Keyboard.h"

class ClimbableObj : public GameObj {
	public:
		ClimbableObj(Float3 pos, Float3 size) {
			_pos = pos;
			_size = size * 2;
			_color = { 0.5f, 0.2f, 0.1f, 0.8f };
			_tag = ObjTag::LADDER;
		}
		void update() override {
			
		}
		void draw() override {
			GameObj::draw();
		}
		void collide(GameObj* gameObj, bool is2D) override {
			Cube playerBox = gameObj->getBox();
			bool triggerClimb = false;
			if (Keyboard_IsKeyTrigger(KK_UP)) {
				playerBox.pos.y += MOVE_VEL;
				triggerClimb = true;
			}
			if (Keyboard_IsKeyTrigger(KK_DOWN)) {
				playerBox.pos.y -= MOVE_VEL;
				triggerClimb = true;
			}
			if (MathTool::checkCollision(playerBox, this->getBox(), is2D)) {
				if (triggerClimb) {
					gameObj->hitObj(this);
				}
			}
		}
};