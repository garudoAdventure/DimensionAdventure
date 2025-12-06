#pragma once

#include "PlayerState.h"

class PlayerEvent : public PlayerState {
	public:
		PlayerEvent();
		void init() override;
		void update() override;
		void draw() override;
};