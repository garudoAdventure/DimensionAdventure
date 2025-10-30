#pragma once

#include "GameObj.h"
#include "Player.h"

class ObjManager {
	public:
		virtual void update() = 0;
		virtual void draw() = 0;
		virtual void collide(Player* player, bool is2DMode) = 0;
};