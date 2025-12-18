#include "Player.h"
#include "Keyboard.h"
#include "PlayerIdle.h"
#include "PlayerEvent.h"
#include "Shader.h"
#include "MathTool.h"
#include "ShowDialogEvent.h"
#include "Dialog.h"

Player::Player(IGameEventHandler* gameEvent) : _gameEvent(gameEvent) {
	_pos = MathTool::getCoordPos({ 5.0f, 1.1f, 5.0f });
	_size = { 2.0f, 3.5f, 2.0f };
	_color = { 1.0f, 1.0f, 1.0f, 0.8f };
	_tag = ObjTag::PLAYER_TAG;
	_model = new Model("./assets/model/player.fbx");

	_spirit = new Spirit(gameEvent);
	_spirit->setPos(_pos);

	_playerController = new PlayerController2D();
	setState(new PlayerIdle());
}

Player::~Player() {
	delete _spirit;
	delete _model;
}

void Player::update() {
	changeState();
	
	GameObj::update();

	_currentState->update();

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
	else if (_vel.z < 0) {
		_dir.z = -1.0f;
	}

	_pos += _vel;
	
	if (_hasDimensionAbility) {
		if (Keyboard_IsKeyDown(KK_SPACE)) {
			_keydownTime++;
		}
		if (_crystalNum > 1) {
			if (_keydownTime > 40) {
				_keydownTime = 0;
				convertLayer();
			}
			else {
				if (_keydownTime > 1 && Keyboard_IsKeyUp(KK_SPACE)) {
					_keydownTime = 0;
					convertDimension();
				}
			}
		}
		else {
			if (_keydownTime > 0) {
				_keydownTime = 0;
				convertDimension();
			}
		}
	}

	 _spirit->updatePos(_pos, _dir);
	 _spirit->update();

	 _model->update();
}

void Player::draw() {
	Float3 radian;
	XMVECTOR lightDirection;

	if (_is2D) {
		radian = { 0.0f, PI, 0.0f };
		lightDirection = { 0.0f, -0.5f, 1.0f };
		if (_dir.x > 0) {
			radian.y -= PI / 8;
		}
		if (_dir.x < 0) {
			radian.y += PI / 8;
		}
	}
	else {
		radian = { 0.0f, PI / 2, 0.0f };
		lightDirection = { 0.5f, -1.0f, 0.0f };
		if (_dir.x > 0) {
			radian.y += 0.0f;
		}
		if (_dir.x < 0) {
			radian.y += PI;
		}
	}

	Light light;
	light.enable = true;
	lightDirection = XMVector3Normalize(lightDirection);
	XMStoreFloat3(&light.direction, lightDirection);
	SHADER.setLight(light);

	_model->draw(_pos, radian);
}

void Player::setState(PlayerState* state) {
	_newState = state;
}

void Player::changeState() {
	if (_newState != _currentState) {
		delete _currentState;
		_currentState = _newState;
		_currentState->player = this;
		_currentState->init();
	}
}

void Player::jump() {
	_vel.y += JUMP_FORCE;
	_isJump = true;
	if (_gameEvent->getCheckpoint() == CheckPoint::JUMP) {
		_gameEvent->setCheckpoint(CheckPoint::FIND_FIRST_CRYSTAL);
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
	if (gameObj->getTag() == ObjTag::PLAYER_FALL_POINT) {
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
			_pos.x = objLeft - _size.x / 2 - 0.01f;
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
			_pos.x = objRight + _size.x / 2 + 0.01f;
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
			_pos.z = objBack + _size.z / 2 + 0.01f;
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
			_pos.z = objFront - _size.z / 2 - 0.01f;
		}
	}
	if (
		oldPlayerTop <= objBottom &&
		playerTop > objBottom &&
		std::min(playerRight - objLeft, objRight - playerLeft) > 0.21f &&
		(_is2D || std::min(playerBack - objFront, objBack - playerFront) > 0.21f)
	) {
		_vel.y = 0.0f;
		_pos.y = objBottom - _size.y / 2;
	}
	if (
		gameObj->getTag() == ObjTag::MOVING_FLOOR ||
		oldPlayerBottom >= objTop &&
		playerBottom < objTop &&
		std::min(playerRight - objLeft, objRight - playerLeft) > 0.21f &&
		(_is2D || std::min(playerBack - objFront, objBack - playerFront) > 0.21f)
	) {
		_vel.y = 0.0f;
		_pos.y = objTop + _size.y / 2;
		_isJump = false;
	}

	// 慣性
	_pos += gameObj->getVel();
}