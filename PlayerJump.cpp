#include "PlayerJump.h"
#include "PlayerIdle.h"
#include "PlayerWalk.h"
#include "PlayerTeleport.h"

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
	if (player->getEnergy() > 0 && Keyboard_IsKeyTrigger(KK_X)) {
		player->setState(new PlayerTeleport());
	}
}

void PlayerJump::draw() {
	CUBE_RENDERER.drawCube(player->getBox(), { 0.2f, 1.0f, 0.2f, 0.8f }, -1);
}