#pragma once

#include "Player.h"

class Player;

class PlayerState {
	public:
		virtual ~PlayerState() = default;
		Player* player = nullptr;
		virtual void init() = 0;
		virtual void update() = 0;
		virtual void draw() = 0;
};