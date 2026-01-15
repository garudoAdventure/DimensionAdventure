#include "PlayerJump.h"
#include "PlayerIdle.h"
#include "PlayerWalk.h"
#include "Sound.h"

PlayerJump::PlayerJump() {
	jumpSE = SOUND.loadSound("./assets/sound/jump.wav");
	SOUND.playSound(jumpSE, 0);
	SOUND.setVolume(jumpSE, 0.5f);
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