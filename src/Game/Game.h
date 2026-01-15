#pragma once

#include "GameState.h"

class GameState;

class Game {
  public:
		Game();
		~Game();
		void update();
		void draw();
		void setState(GameState* state);

  private:
		GameState* _state = nullptr;
		GameState* _newState = nullptr;
		void changeState();
};