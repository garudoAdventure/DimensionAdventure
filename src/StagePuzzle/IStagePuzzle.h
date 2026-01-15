#pragma once

#include "./Player/Player.h"

class IStagePuzzle {
	public:
		virtual ~IStagePuzzle() {}
		virtual void update() = 0;
		virtual void draw() = 0;
		virtual void collide(Player* player, bool is2D) = 0;
};