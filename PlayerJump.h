#pragma once

#include "PlayerState.h"

class PlayerJump : public PlayerState {
	public:
		PlayerJump();
		void update() override;
		void draw() override;
};