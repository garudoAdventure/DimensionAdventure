#pragma once

#include "PlayerState.h"

class PlayerWalk : public PlayerState {
	public:
		PlayerWalk();
		void update() override;
		void draw() override;
};