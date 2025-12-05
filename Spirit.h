#pragma once

#include "MathStruct.h"
#include "Particle.h"
#include "MathTool.h"

class Spirit {
	public:
		Spirit() {
			emitter = new Emitter();
		}
		void update() {
			_pos = MathTool::lerp<Float3>(_pos, _newPos, 0.05f);
			_pos.y += sinf(time * 0.05f) * 0.01f;
			emitter->update(_pos);
			time++;
		}
		void updatePos(Float3 playerPos, Float3 playerDir) {
			_newPos = {
				playerPos.x - 3.0f * playerDir.x,
				playerPos.y + 2.0f,
				playerPos.z
			};
		}
		void setPos(Float3 playerPos) {
			_pos = {
				playerPos.x - 3.0f,
				playerPos.y + 2.0f,
				playerPos.z
			};
		}
		void draw() {
			emitter->draw(_pos);
		}

	private:
		Float3 _pos = { 0.0f, 0.0f, 0.0f };
		Float3 _newPos = { 0.0f, 0.0f, 0.0f };
		Emitter* emitter;
		int time = 0;
};