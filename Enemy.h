#pragma once

#include "GameObj.h"

class Enemy : public GameObj {
	public:
		Enemy(Float3 pos) {
			_pos = pos;
			_size = { 2.0f, 2.0f, 2.0f };
			_color = { 0.3f, 0.0f, 0.5f, 0.8f };
			_vel.x = 0.1f;
		}
		void update() {
			if (count % 180 == 0) {
				_vel.x = -_vel.x;
				count = 0;
			}
			count++;

			_pos.x += _vel.x;
			_pos.y += _vel.y;
			_pos.z += _vel.z;
		}
		void hurt(int damage) {
			_hp = std::max(_hp - damage, 0);
		}
		bool isDead() {
			return _hp <= 0;
		}

	private:
		Float3 _vel = { 0.0f, 0.0f, 0.0f };
		int _hp = 1;
		int count = 0;
};