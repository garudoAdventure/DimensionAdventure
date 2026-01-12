#include "GameObj.h"
#include "CubeRenderer.h"
#include "MathTool.h"

void GameObj::update() {
	_oldPos = _pos;
}

void GameObj::draw() {
	CUBE_RENDERER.drawCube({ _pos, _size }, _color, _texID);
}

void GameObj::collide(GameObj* obj, bool is2D) {
	if (MathTool::checkCollision(this->getBox(), obj->getBox(), is2D)) {
		obj->hitObj(this);
	}
}

void GameObj::setPos(Float3 pos) {
	_pos = pos;
}

void GameObj::setPosX(float posX) {
	_pos.x = posX;
}

void GameObj::setPosY(float posY) {
	_pos.y = posY;
}

void GameObj::setPosZ(float posZ) {
	_pos.z = posZ;
}

void GameObj::setColor(Float4 color) {
	_color = color;
}

Float3 GameObj::getPos() const {
	return _pos;
}

Float3 GameObj::getVel() const {
	return _vel;
}

Float3 GameObj::getSize() const {
	return _size;
}

Cube GameObj::getBox() const {
	return { _pos, _size };
}

ObjTag GameObj::getTag() const {
	return _tag;
}

bool GameObj::isActive() const {
	return _isActive;
}

void GameObj::hitObj(GameObj* obj, bool isStatic) {
	const float top = _pos.y + _size.y / 2;
	const float bottom = _pos.y - _size.y / 2;
	const float right = _pos.x + _size.x / 2;
	const float left = _pos.x - _size.x / 2;
	const float front = _pos.z - _size.z / 2;
	const float back = _pos.z + _size.z / 2;
	const float oldTop = _oldPos.y + _size.y / 2;
	const float oldBottom = _oldPos.y - _size.y / 2;
	const float oldRight = _oldPos.x + _size.x / 2;
	const float oldLeft = _oldPos.x - _size.x / 2;
	const float oldFront = _oldPos.z - _size.z / 2;
	const float oldBack = _oldPos.z + _size.z / 2;
	const float blockTop = obj->_pos.y + obj->_size.y / 2;
	const float blockBottom = obj->_pos.y - obj->_size.y / 2;
	const float blockRight = obj->_pos.x + obj->_size.x / 2;
	const float blockLeft = obj->_pos.x - obj->_size.x / 2;
	const float blockFront = obj->_pos.z - obj->_size.z / 2;
	const float blockBack = obj->_pos.z + obj->_size.z / 2;	
	if (oldRight <= blockLeft && right > blockLeft) {
		if (!isStatic) {
			obj->_pos.x = right + obj->_size.x / 2 + 0.001f;
		}
		else {
			_pos.x = blockLeft - _size.x / 2;
		}
	}
	if (oldLeft >= blockRight && left < blockRight) {
		if (!isStatic) {
			obj->_pos.x = left - obj->_size.x / 2 - 0.001f;
		}
		else {
			_pos.x = blockRight + _size.x / 2;
		}
	}
	if (oldFront >= blockBack && front < blockBack) {
		if (!isStatic) {
			obj->_pos.z = front - obj->_size.z / 2 - 0.001f;
		}
		else {
			_pos.z = blockBack + _size.z / 2;
		}
	}
	if (oldBack <= blockFront && back > blockFront) {
		if (!isStatic) {
			obj->_pos.z = back + obj->_size.z / 2 + 0.001f;
		}
		else {
			_pos.z = blockFront - _size.z / 2;
		}
	}
}