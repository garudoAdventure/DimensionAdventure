#include "PlayerWalk.h"
#include "PlayerIdle.h"
#include "PlayerJump.h"
#include "Sound.h"

PlayerWalk::PlayerWalk() {
	walkSE = SOUND.loadSound("./assets/sound/walk.wav");
}

PlayerWalk::~PlayerWalk() {
	SOUND.stopSound(walkSE);
}

void PlayerWalk::init() {
	player->getModel()->loadAnim(1);
	SOUND.playSound(walkSE, -1);
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
}

void PlayerWalk::draw() {
}