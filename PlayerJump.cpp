#include "PlayerJump.h"
#include "PlayerIdle.h"
#include "PlayerWalk.h"

PlayerJump::PlayerJump() {
}

void PlayerJump::init() {
}

void PlayerJump::update() {
	player->getVel().x = 0.0f;
	player->getVel().z = 0.0f;
	player->getPlayerController()->move(player->getVel());
	if (!player->isJump()) {
		if (player->getPlayerController()->isMove()) {
			player->setState(new PlayerWalk());
		}
		else {
			player->setState(new PlayerIdle());
		}
	}
}

void PlayerJump::draw() {
}