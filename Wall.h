#pragma once

#include "GameObj.h"
#include "CubeRenderer.h"
#include "Color.h"

class Wall : public GameObj {
	public:
		Wall(Float3 pos, Float3 size) {
			_pos = pos;
			_size = size;
		}
		void draw() override {
			// 描画しない
		}
};