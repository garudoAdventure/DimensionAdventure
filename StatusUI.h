#pragma once

#include "Texture.h"
#include "Sprite.h"
#include "Player.h"

class StatusUI {
	public:
		StatusUI() {
			crystalTex = TEXTURE.loadTexture("./assets/UI/crystal.png");
			circleArrowTex = TEXTURE.loadTexture("./assets/UI/circleArrow.png");
			baseCircleTex = TEXTURE.loadTexture("./assets/UI/baseCircle.png");
			crystalColor[0] = Color::lightRed;
			crystalColor[1] = Color::lightGreen;
			crystalColor[2] = Color::lightBlue;
		}
		void update() {
			crystalNum = PLAYER.getCrystalNum();
			if (PLAYER.is2D()) {
				return;
			}
			rotateRadian -= 0.04f;
		}
		void draw() {
			SPRITE.drawSprite2D({ 95.0f - 640.0f, -83.0f + 360.0f }, { 108.0f, 102.0f }, baseCircleTex);
			if (!PLAYER.hasDimensionRing()) {
				SPRITE.drawSprite2D({ 90.0f - 640.0f, -82.0f + 360.0f }, { 126.0f, 119.0f }, circleArrowTex, { 0.0f, 0.0f, 0.0f, 1.0f });
			}
			else {
				SPRITE.drawSprite2DRotate({ 90.0f - 640.0f, -82.0f + 360.0f }, { 126.0f, 119.0f }, circleArrowTex, rotateRadian, { 95.0f - 640.0f, -82.0f + 360.0f });
			}
			for (int i = 0; i < crystalNum; i++) {
				SPRITE.drawSprite2DRotate(
					{ 95.0f - 640.0f, -61.0f + 360.0f }, { 22.0f, 38.0f },
					crystalTex, 2 * PI * i / 3.0f, { 95.0f - 640.0f, -83.0f + 360.0f },
					crystalColor[i]
				);
			}
		}

	private:
		unsigned int crystalTex;
		unsigned int circleArrowTex;
		unsigned int baseCircleTex;
		float rotateRadian = 0.0f;
		int crystalNum = 0;
		Float4 crystalColor[3];
};