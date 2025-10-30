#include "Game.h"
#include "GameMain.h"

Game::Game() {
	setState(new GameMain());
}

void Game::update() {
	changeState();
	state->update();
}

void Game::draw() {
	state->draw();
}

void Game::setState(GameState* state) {
	newState = state;
}

void Game::changeState() {
	if (newState != state) {
		delete state;
		state = newState;
		state->game = this;
	}
}

