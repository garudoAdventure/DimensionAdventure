#include "PlayerWalk.h"
#include "PlayerIdle.h"
#include "PlayerJump.h"
#include "PlayerTeleport.h"

PlayerWalk::PlayerWalk() {
}

void PlayerWalk::init() {
	player->getModel()->loadAnim(10);
}

void PlayerWalk::update() {
	player->getVel().x = 0.0f;
	player->getVel().z = 0.0f;
	if (player->getPlayerController()->isMove()) {
		player->getPlayerController()->move(player->getVel());
	}
	else {
		player->setState(new PlayerIdle());
	}

	if (!player->isJump() && Keyboard_IsKeyTrigger(KK_Z)) {
		player->jump();
		player->setState(new PlayerJump());
	}
	if (player->getEnergy() > 0 && Keyboard_IsKeyTrigger(KK_X)) {
		player->setState(new PlayerTeleport());
	}
}

void PlayerWalk::draw() {
	CUBE_RENDERER.drawCube(player->getBox(), {0.2f, 0.2f, 1.0f, 0.8f}, -1);
}