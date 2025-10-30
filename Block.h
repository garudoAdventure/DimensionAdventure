#pragma once

#include "GameObj.h"

class Block : public GameObj {
	public:
		Block(Float3 pos, Float3 size, int texID = -1) {
			_pos = pos;
			_size = size * 2;
			_color = { 0.2f, 0.2f, 1.0f, 0.8f };
			if (texID != -1) {
				_color = { 1.0f, 1.0f, 1.0f, 1.0f };
			}
			_texID = texID;
		}
		virtual void update() {
		}
};

class MovingBlock : public Block {
	public:
		MovingBlock(Float3 pos, Float3 vel, Float3 size, int texID = -1) : Block(pos, size, texID) {
			_vel = vel;
		}
		void update() override {
			if (count == 300) {
				_vel.x = -_vel.x;
				count = 0;
			}
			count++;

			_pos += _vel;
		}
	
	private:
		int count = 0;
};