#pragma once

#include "PlayerState.h"

class PlayerHurt : public PlayerState {
	public:
		PlayerHurt();
		void update() override;
		void draw() override;

	private:
		int count = 0;
};