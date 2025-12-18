#include "PlayerIdle.h"
#include "Keyboard.h"
#include "PlayerWalk.h"
#include "PlayerJump.h"

PlayerIdle::PlayerIdle() {
}

void PlayerIdle::init() {
	player->getModel()->loadAnim(0);
}

void PlayerIdle::update() {
	player->getVel().x = 0.0f;
	player->getVel().z = 0.0f;

	if (player->getPlayerController()->isMove()) {
		player->setState(new PlayerWalk());
	}
	if (!player->isJump() && Keyboard_IsKeyDown(KK_Z)) {
		player->jump();
		player->setState(new PlayerJump());
	}
}

void PlayerIdle::draw() {
}