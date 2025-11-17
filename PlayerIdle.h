#pragma once

#include "PlayerState.h"

class PlayerIdle : public PlayerState {
	public:
		PlayerIdle();
		void init() override;
		void update() override;
		void draw() override;
};