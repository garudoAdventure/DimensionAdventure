#pragma once

#include "HALKeyboard.h"
#include "./Common/MathStruct.h"

class PlayerController {
	public:
		static constexpr float MOVE_VEL = 0.2f;
		virtual bool isMove() = 0;
		virtual void move(Float3& vel) = 0;
};

class PlayerController2D : public PlayerController {
	public:
		PlayerController2D() = default;
		bool isMove() override {
			return 
				Keyboard_IsKeyDown(KK_RIGHT) ||
				Keyboard_IsKeyDown(KK_LEFT);
		}
		void move(Float3& vel) override {
			if (Keyboard_IsKeyDown(KK_RIGHT)) {
				vel.x = MOVE_VEL;
			}
			if (Keyboard_IsKeyDown(KK_LEFT)) {
				vel.x = -MOVE_VEL;
			}
		}
};

class PlayerController3D : public PlayerController {
	public:
		PlayerController3D() = default;
		bool isMove() override {
			return
				Keyboard_IsKeyDown(KK_RIGHT) ||
				Keyboard_IsKeyDown(KK_LEFT) ||
				Keyboard_IsKeyDown(KK_UP) ||
				Keyboard_IsKeyDown(KK_DOWN);
		}
		void move(Float3& vel) override {
			if (Keyboard_IsKeyDown(KK_RIGHT)) {
				vel.z = -MOVE_VEL;
			}
			if (Keyboard_IsKeyDown(KK_LEFT)) {
				vel.z = MOVE_VEL;
			}
			if (Keyboard_IsKeyDown(KK_UP)) {
				vel.x = MOVE_VEL;
			}
			if (Keyboard_IsKeyDown(KK_DOWN)) {
				vel.x = -MOVE_VEL;
			}
		}
};