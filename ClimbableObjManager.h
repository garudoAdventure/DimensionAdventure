#pragma once

#include "ObjManager.h"
#include "ClimbableObj.h"
#include "MathTool.h"
#include <vector>

class ClimbableObjManager : public ObjManager {
public:
	ClimbableObjManager() = default;
	~ClimbableObjManager() {
		for (ClimbableObj* climbableObj : climbableObjs) {
			delete climbableObj;
		}
	}
	void update() override {
	}
	void draw() override {
		for (ClimbableObj* climbableObj : climbableObjs) {
			climbableObj->draw();
		}
	}
	void collide(Player* player, bool is2D) override {
		Cube playerBox = player->getBox();
		bool triggerClimb = false;
		if (Keyboard_IsKeyTrigger(KK_UP)) {
			playerBox.pos.y += MOVE_VEL;
			triggerClimb = true;
		}
		if (Keyboard_IsKeyTrigger(KK_DOWN)) {
			playerBox.pos.y -= MOVE_VEL;
			triggerClimb = true;
		}
		for (ClimbableObj* climbableObj : climbableObjs) {
			if (MathTool::checkCollision(playerBox, climbableObj->getBox(), is2D)) {
				if (triggerClimb) {
					player->climb(climbableObj);
				}
			}
		}
	}
	void add(ClimbableObj* climbableObj) {
		climbableObjs.emplace_back(climbableObj);
	}

private:
	std::vector<ClimbableObj*> climbableObjs;
};