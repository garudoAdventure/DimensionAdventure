#pragma once

#include "Game.h"

class Game;

class GameState {
	public:
		virtual ~GameState() = default;
		Game* game = nullptr;
		virtual void update() = 0;
		virtual void draw() = 0;
};