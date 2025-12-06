#include "Player.h"
#include "Keyboard.h"
#include "PlayerIdle.h"
#include "PlayerClimb.h"
#include "PlayerHurt.h"
#include "PlayerEvent.h"
#include "Shader.h"
#include "MathTool.h"
#include "ShowDialogEvent.h"
#include "Dialog.h"

Player::Player(IGameEventHandler* gameEvent) : _gameEvent(gameEvent) {
	_pos = MathTool::getCoordPos({ 29.0f, 1.1f, 5.0f });
	_size = { 2.0f, 3.5f, 2.0f };
	_color = { 1.0f, 1.0f, 1.0f, 0.8f };
	_tag = ObjTag::PLAYER_TAG;
	_model = new Model("./assets/model/player.fbx");

	_spirit = new Spirit();
	_spirit->setPos(_pos);

	_playerController = new PlayerController2D();
	setState(new PlayerIdle());
}

void Player::update() {
	changeState();
	autoRecoverEnergy();
	invincibleUpdate();
	attackModeUpdate();
	
	GameObj::update();

	currentState->update();

	_vel.y -= GRAVITY;

	if (_vel.x > 0) {
		_dir.x = 1.0f;
	}
	else if (_vel.x < 0) {
		_dir.x = -1.0f;
	}
	if (_vel.z > 0) {
		_dir.z = 1.0f;
	}
	if (_vel.z < 0) {
		_dir.z = -1.0f;
	}

	_pos += _vel;
	
	if (_hasDimensionAbility) {
		if (Keyboard_IsKeyTrigger(KK_BACK)) {
			convertDimension();
		}
		if (_crystalNum > 0 && Keyboard_IsKeyTrigger(KK_NUMPAD0)) {
			convertLayer();
		}
	}

	 _spirit->updatePos(_pos, _dir);
	 _spirit->update();

	 _model->update();
}

void Player::draw() {
	Float3 radian;
	XMVECTOR direction;
	if (_is2D) {
		radian = { 0.0f, PI, 0.0f };
		direction = { 0.0f, -0.5f, 1.0f };
		if (_dir.x > 0) {
			radian.y -= PI / 8;
		}
		if (_dir.x < 0) {
			radian.y += PI / 8;
		}
	}
	else {
		direction = { 0.5f, -1.0f, 0.0f };
		radian = { 0.0f, PI / 2, 0.0f };
		if (_dir.x > 0) {
			radian.y += 0.0f;
		}
		if (_dir.x < 0) {
			radian.y += PI;
		}
	}

	Light light;
	light.enable = true;
	direction = XMVector3Normalize(direction);
	XMStoreFloat3(&light.direction, direction);

	SHADER.setLight(light);

	_model->draw(_pos, radian);

	// _spirit->draw();
}

void Player::setState(PlayerState* state) {
	newState = state;
}

void Player::changeState() {
	if (newState != currentState) {
		delete currentState;
		currentState = newState;
		currentState->player = this;
		currentState->init();
	}
}

void Player::jump() {
	_vel.y += JUMP_FORCE;
	_isJump = true;
}

void Player::climb(GameObj* climableObj) {
	if (!_isOnClimbableObj) {
		setState(new PlayerClimb(climableObj));
		_isOnClimbableObj = true;
	}
}

void Player::hurt(int damage) {
	if (!_isInvincible && !_isAttackMode) {
		_hp = std::max(_hp - damage, 0);
		setState(new PlayerHurt());
		_isInvincible = true;
	}
}

void Player::attackEnemy(Enemy* enemy) {
	if (_isAttackMode) {
		enemy->hurt(1);
	}
}

void Player::convertDimension() {
	_is2D = !_is2D;
	if (_is2D) {
		_playerController = new PlayerController2D();
	}
	else {
		_playerController = new PlayerController3D();
	}
	_gameEvent->transformDimension();
}

void Player::convertLayer() {
	_gameEvent->transformLayer();
}

void Player::invincibleUpdate() {
	static int count = 0;
	if (_isInvincible) {
		count++;
		if (count == 120) {
			_isInvincible = false;
			count = 0;
		}
	}
}

void Player::attackModeUpdate() {
	static int count = 0;
	if (_isAttackMode) {
		count++;
		if (count == 10) {
			_isAttackMode = false;
			count = 0;
		}
	}
}

void Player::autoRecoverEnergy() {
	if (_energy == 5) return;
	if (_recoverEnergyCount < 300) {
		_recoverEnergyCount++;
	}
	else {
		_recoverEnergyCount = 0;
		_energy += 1;
	}
}

void Player::getDimensionAbility() {
	_hasDimensionAbility = true;
}

void Player::addCrystalNum() {
	_crystalNum += 1;
}

void Player::setToEventState(bool isEventState) {
	if (isEventState) {
		setState(new PlayerEvent());
	}
	else {
		setState(new PlayerIdle());
	}
}

void Player::hitObj(GameObj* gameObj, bool isStatic) {
	if (gameObj->getTag() == ObjTag::LADDER) {
		climb(gameObj);
		return;
	}
	const float playerTop = _pos.y + _size.y / 2;
	const float playerBottom = _pos.y - _size.y / 2;
	const float playerRight = _pos.x + _size.x / 2;
	const float playerLeft = _pos.x - _size.x / 2;
	const float playerFront = _pos.z - _size.z / 2;
	const float playerBack = _pos.z + _size.z / 2;
	const float oldPlayerTop = _oldPos.y + _size.y / 2;
	const float oldPlayerBottom = _oldPos.y - _size.y / 2;
	const float oldPlayerRight = _oldPos.x + _size.x / 2;
	const float oldPlayerLeft = _oldPos.x - _size.x / 2;
	const float oldPlayerFront = _oldPos.z - _size.z / 2;
	const float oldPlayerBack = _oldPos.z + _size.z / 2;
	const float objTop = gameObj->getPos().y + gameObj->getSize().y / 2;
	const float objBottom = gameObj->getPos().y - gameObj->getSize().y / 2;
	const float objRight = gameObj->getPos().x + gameObj->getSize().x / 2;
	const float objLeft = gameObj->getPos().x - gameObj->getSize().x / 2;
	const float objFront = gameObj->getPos().z - gameObj->getSize().z / 2;
	const float objBack = gameObj->getPos().z + gameObj->getSize().z / 2;
	if (
		oldPlayerRight <= objLeft &&
		playerRight > objLeft &&
		objTop - playerBottom > 0.031f
	) {
		if (!isStatic) {
			gameObj->setPosX(playerRight + gameObj->getSize().x / 2 + 0.001f);
		}
		else {
			_pos.x = objLeft - _size.x / 2;
		}
	}
	if (
		oldPlayerLeft >= objRight &&
		playerLeft < objRight &&
		objTop - playerBottom > 0.031f
		) {
		if (!isStatic) {
			gameObj->setPosX(playerLeft - gameObj->getSize().x / 2 - 0.001f);
		}
		else {
			_pos.x = objRight + _size.x / 2;
		}
	}
	if (
		oldPlayerFront >= objBack &&
		playerFront < objBack &&
		objTop - playerBottom > 0.031f
	) {
		if (!isStatic) {
			gameObj->setPosZ(playerFront - gameObj->getSize().z / 2 - 0.001f);
		}
		else {
			_vel.z = 0.0f;
			_pos.z = objBack + _size.z / 2;
		}
	}
	if (
		oldPlayerBack <= objFront &&
		playerBack > objFront &&
		objTop - playerBottom > 0.031f
		) {
		if (!isStatic) {
			gameObj->setPosZ(playerBack + gameObj->getSize().z / 2 + 0.001f);
		}
		else {
			_vel.z = 0.0f;
			_pos.z = objFront - _size.z / 2;
		}
	}
	if (
		gameObj->getTag() == ObjTag::MOVING_FLOOR ||
		!_isOnClimbableObj &&
		oldPlayerBottom >= objTop &&
		playerBottom < objTop &&
		std::min(playerRight - objLeft, objRight - playerLeft) > 0.21f
	) {
		_vel.y = 0.0f;
		_pos.y = objTop + _size.y / 2;
		_isJump = false;
	}

	// Šµ«
	_pos += gameObj->getVel();
}