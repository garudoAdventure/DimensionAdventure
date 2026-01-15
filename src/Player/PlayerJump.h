#pragma once

#include "PlayerState.h"

class PlayerJump : public PlayerState {
	public:
		PlayerJump();
		void init() override;
		void update() override;
		void draw() override;

	private:
		unsigned int jumpSE;
};