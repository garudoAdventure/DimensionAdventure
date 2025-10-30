#pragma once

#include "MathStruct.h"
#include "Keyboard.h"

class PlayerController {
	public:
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
				vel.x = 0.2f;
			}
			if (Keyboard_IsKeyDown(KK_LEFT)) {
				vel.x = -0.2f;
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
				vel.z = -0.2f;
			}
			if (Keyboard_IsKeyDown(KK_LEFT)) {
				vel.z = 0.2f;
			}
			if (Keyboard_IsKeyDown(KK_UP)) {
				vel.x = 0.2f;
			}
			if (Keyboard_IsKeyDown(KK_DOWN)) {
				vel.x = -0.2f;
			}
		}
};