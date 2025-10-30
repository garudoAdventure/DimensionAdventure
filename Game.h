#pragma once

#include "GameState.h"

class GameState;

class Game {
  public:
		Game();
		void update();
		void draw();
		void setState(GameState* state);

  private:
		GameState* state = nullptr;
		GameState* newState = nullptr;
		void changeState();
};