#pragma once

#include "PlayerState.h"

class PlayerWalk : public PlayerState {
	public:
		PlayerWalk();
		void init() override;
		void update() override;
		void draw() override;
};