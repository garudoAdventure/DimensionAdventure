#include "PlayerHurt.h"
#include "PlayerIdle.h"

PlayerHurt::PlayerHurt() {
}

void PlayerHurt::update() {
	player->getVel().x = 0.0f;
	player->getVel().z = 0.0f;
	if (count == 30) {
		player->setState(new PlayerIdle());
	}
	count++;
}

void PlayerHurt::draw() {
	CUBE_RENDERER.drawCube(player->getBox(), { 1.0f, 0.2f, 0.2f, 0.8f }, -1);
}