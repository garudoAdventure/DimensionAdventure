#include "PlayerIdle.h"
#include "Keyboard.h"
#include "PlayerWalk.h"
#include "PlayerJump.h"
#include "PlayerTeleport.h"

PlayerIdle::PlayerIdle() {
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
	if (player->getEnergy() > 0 && Keyboard_IsKeyTrigger(KK_X)) {
		player->setState(new PlayerTeleport());
	}
}

void PlayerIdle::draw() {
	CUBE_RENDERER.drawCube(player->getBox(), { 1.0f, 1.0f, 1.0f, 0.8f }, -1);
}