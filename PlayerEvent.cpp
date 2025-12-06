#include "PlayerEvent.h"

PlayerEvent::PlayerEvent() {
}

void PlayerEvent::init() {
	player->getModel()->loadAnim(0);
}

void PlayerEvent::update() {
	player->getVel().x = 0.0f;
	player->getVel().z = 0.0f;
}

void PlayerEvent::draw() {
}