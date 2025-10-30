#include "PlayerClimb.h"
#include "PlayerIdle.h"
#include "PlayerJump.h"

PlayerClimb::PlayerClimb(GameObj* climbableObj) : _climbableObj(climbableObj) {
}

void PlayerClimb::update() {
	player->getVel().x = 0.0f;
	player->getVel().y = 0.0f;
	player->getVel().z = 0.0f;
	Float3 playerPos = player->getPos();
	const Float3 playerSize = player->getSize();
	float playerBottom = playerPos.y - playerSize.y / 2;
	const float ladderTop = _climbableObj->getPos().y + _climbableObj->getSize().y / 2;
	const float ladderBottom = _climbableObj->getPos().y - _climbableObj->getSize().y / 2;
	if (Keyboard_IsKeyDown(KK_UP)) {
		playerBottom += MOVE_VEL;
	}
	if (Keyboard_IsKeyDown(KK_DOWN)) {
		playerBottom -= MOVE_VEL;
	}
	if (!player->isJump() && Keyboard_IsKeyTrigger(KK_Z)) {
		player->jump();
		player->setIsClimbing(false);
		player->setState(new PlayerJump());
		return;
	}
	if (playerBottom > ladderBottom && playerBottom <= ladderTop) {
		playerPos.y = playerBottom + playerSize.y / 2 + GRAVITY;
		player->setPos(playerPos);
		player->setJump(false);
		return;
	}
	if (playerBottom > ladderTop) {
		playerPos.y = ladderTop + playerSize.y / 2 + GRAVITY;
		player->setPos(playerPos);
	}
	player->setState(new PlayerIdle());
	player->setIsClimbing(false);
}

void PlayerClimb::draw() {
	CUBE_RENDERER.drawCube(player->getBox(), { 0.8f, 0.5f, 0.1f, 0.8f }, -1);
}