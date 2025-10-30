#pragma once

#include "GameObj.h"
#include "Keyboard.h"

class ClimbableObj : public GameObj {
	public:
		ClimbableObj(Float3 pos, Float3 size) {
			_pos = pos;
			_size = size * 2;
			_color = { 0.5f, 0.2f, 0.1f, 0.8f };
		}
};