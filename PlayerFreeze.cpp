#include "PlayerFreeze.h"

PlayerFreeze::PlayerFreeze() {
}

void PlayerFreeze::init() {
	player->getModel()->loadAnim(0);
}

void PlayerFreeze::update() {
	player->getVel().x = 0.0f;
	player->getVel().z = 0.0f;
}

void PlayerFreeze::draw() {
}