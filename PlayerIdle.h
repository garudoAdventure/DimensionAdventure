#pragma once

#include "PlayerState.h"

class PlayerIdle : public PlayerState {
	public:
		PlayerIdle();
		void update() override;
		void draw() override;
};