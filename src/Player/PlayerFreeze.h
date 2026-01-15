#pragma once

#include "PlayerState.h"

class PlayerFreeze : public PlayerState {
	public:
		PlayerFreeze();
		void init() override;
		void update() override;
		void draw() override;
};