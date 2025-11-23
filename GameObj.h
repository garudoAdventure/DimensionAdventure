#pragma once

#include "MathStruct.h"
#include "CubeRenderer.h"

class GameObj {
	public:
		virtual void update() {
			_oldPos = _pos;
		}
		virtual void draw() {
			CUBE_RENDERER.drawCube({ _pos, _size }, _color, _texID);
		}
		void setPos(Float3 pos) {
			_pos = pos;
		}
		void setPosX(float posX) {
			_pos.x = posX;
		}
		void setPosY(float posY) {
			_pos.y = posY;
		}
		void setPosZ(float posZ) {
			_pos.z = posZ;
		}
		Float3 getPos() const {
			return _pos;
		}
		Float3 getVel() const {
			return _vel;
		}
		Float3 getSize() const {
			return _size;
		}
		Cube getBox() const {
			return { _pos, _size };
		}
		virtual void hitObj(GameObj* obj, bool isStatic = true) {
			const float top =	   _pos.y + _size.y / 2;
			const float bottom = _pos.y - _size.y / 2;
			const float right =  _pos.x + _size.x / 2;
			const float left =   _pos.x - _size.x / 2;
			const float front =  _pos.z - _size.z / 2;
			const float back =   _pos.z + _size.z / 2;
			const float oldTop =    _oldPos.y + _size.y / 2;
			const float oldBottom = _oldPos.y - _size.y / 2;
			const float oldRight =  _oldPos.x + _size.x / 2;
			const float oldLeft =   _oldPos.x - _size.x / 2;
			const float oldFront =  _oldPos.z - _size.z / 2;
			const float oldBack =   _oldPos.z + _size.z / 2;
			const float blockTop =    obj->_pos.y + obj->_size.y / 2;
			const float blockBottom = obj->_pos.y - obj->_size.y / 2;
			const float blockRight =  obj->_pos.x + obj->_size.x / 2;
			const float blockLeft =   obj->_pos.x - obj->_size.x / 2;
			const float blockFront =  obj->_pos.z - obj->_size.z / 2;
			const float blockBack =   obj->_pos.z + obj->_size.z / 2;
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

	protected:
		Float3 _pos;
		Float3 _oldPos;
		Float3 _vel = { 0.0f, 0.0f, 0.0f };
		Float3 _size;
		Float4 _color = { 0.3f, 0.3f, 0.3f, 0.7f };
		int _texID = -1;
};