#pragma once

#include "MathStruct.h"
#include "CubeRenderer.h"

class GameObj {
	public:
		virtual void draw() {
			CUBE_RENDERER.drawCube({ _pos, _size }, _color, _texID);
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

	protected:
		Float3 _pos;
		Float3 _vel = { 0.0f, 0.0f, 0.0f };
		Float3 _size;
		Float4 _color;
		int _texID = -1;
};