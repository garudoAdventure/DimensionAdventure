#include "Game.h"
#include "GameTitle.h"

Game::Game() {
	setState(new GameTitle());
}

Game::~Game() {
	delete _state;
}

void Game::update() {
	changeState();
	_state->update();
}

void Game::draw() {
	_state->draw();
}

void Game::setState(GameState* state) {
	_newState = state;
}

void Game::changeState() {
	if (_newState != _state) {
		delete _state;
		_state = _newState;
		_state->game = this;
	}
}

