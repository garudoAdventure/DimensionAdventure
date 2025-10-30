#include "PlayerTeleport.h"
#include "PlayerWalk.h"
#include "MathTool.h"

PlayerTeleport::PlayerTeleport() {

}

void PlayerTeleport::update() {
	if (!_isInit) {
		_teleportDistance.x = player->getDir().x > 0 ? _maxDistance : _minDistance;
		_isInit = true;
	}
	if (_isTeleport) {
		_teleportCount++;
		if (_teleportCount == 10) {
			player->setPos(_teleportPos);
			_isTeleport = false;
			_teleportCount = 0;
			player->setAttackMode(true);
			player->setState(new PlayerWalk());
		}
		return;
	}
	if (_prepareTeleportCount > 10) {
		player->getVel().x = 0.0f;
		player->getVel().z = 0.0f;
		if (Keyboard_IsKeyDown(KK_RIGHT)) {
			_teleportDistance.x += 0.2f;
		}
		if (Keyboard_IsKeyDown(KK_LEFT)) {
			_teleportDistance.x -= 0.2f;
		}
		if (Keyboard_IsKeyDown(KK_UP)) {
			_teleportDistance.y += 0.2f;
		}
		if (Keyboard_IsKeyDown(KK_DOWN)) {
			_teleportDistance.y -= 0.2f;
		}
		_teleportDistance.x = MathTool::getInRange(_teleportDistance.x, _minDistance, _maxDistance);
		_teleportDistance.y = MathTool::getInRange(_teleportDistance.y, 0.0f, _maxDistance);
	}
	if (Keyboard_IsKeyUp(KK_X)) {
		_isTeleport = true;
		_prepareTeleportCount = 0;
		// _energy -= 1;
		return;
	}
	_prepareTeleportCount++;

	_teleportPos = player->getPos() + _teleportDistance;
}

void PlayerTeleport::draw() {
	const Float3 pos = player->getPos();
	Float3 destPos = pos;
	float alpha = 1.0f;
	if (_isTeleport) {
		destPos.y = MathTool::lerp<float>(pos.y, pos.y + 1.5f, _teleportCount / 6.0f);
		alpha = MathTool::lerp<float>(1.0f, 0.0f, _teleportCount / 6.0f);
		if (_teleportCount == 10) {
			destPos = _teleportPos;
			alpha = 1.0f;
		}
	}
	CUBE_RENDERER.drawCube({ destPos, player->getSize() }, {1.0f, 0.5f, 0.2f, alpha}, -1);
	if (_prepareTeleportCount > 10) {
		CUBE_RENDERER.drawCube({ _teleportPos, player->getSize()}, {1.0f, 0.5f, 0.2f, 0.5f}, -1);
	}
}