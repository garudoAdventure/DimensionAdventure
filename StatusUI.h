#pragma once

#include "Texture.h"
#include "Sprite.h"
#include "Player.h"

class StatusUI {
	public:
		StatusUI() {
			heartTex = TEXTURE.loadTexture("./assets/UI/heart.png");
			energyTex = TEXTURE.loadTexture("./assets/UI/energy.png");
			crystalTex = TEXTURE.loadTexture("./assets/UI/crystal.png");
			circleArrowTex = TEXTURE.loadTexture("./assets/UI/circleArrow.png");
			baseCircleTex = TEXTURE.loadTexture("./assets/UI/baseCircle.png");
			crystalColor[0] = { 255 / 255.0f, 66 / 255.0f,	60 / 255.0f,	1.0f };
			crystalColor[1] = { 0 / 255.0f,		219 / 255.0f,	0 / 255.0f,		1.0f };
			crystalColor[2] = { 0 / 255.0f,		124 / 255.0f, 255 / 255.0f, 1.0f };
		}
		void update() {
			hpNum = PLAYER.getHp();
			energyNum = PLAYER.getEnergy();
			crystalNum = PLAYER.getCrystalNum();
			if (PLAYER.is2D()) {
				count = 0;
				return;
			}
			rotateRadian -= 0.04f;
			count++;
			
			if (count % 300 == 0) {
				energyNum -= 1;
				count = 0;
			}
		}
		void draw() {
			SPRITE.drawSprite2D({ 95.0f - 640.0f, -83.0f + 360.0f }, { 108.0f, 102.0f }, baseCircleTex);
			SPRITE.drawSprite2DRotate({ 90.0f - 640.0f, -82.0f + 360.0f }, { 126.0f, 119.0f }, circleArrowTex, rotateRadian, { 95.0f - 640.0f, -82.0f + 360.0f });
			for (int i = 0; i < crystalNum; i++) {
				SPRITE.drawSprite2DRotate(
					{ 95.0f - 640.0f, -61.0f + 360.0f }, { 22.0f, 38.0f },
					crystalTex, 2 * PI * i / 3.0f, { 95.0f - 640.0f, -83.0f + 360.0f },
					crystalColor[i]
				);
			}
			for (int i = 0; i < hpNum; i++) {
				SPRITE.drawSprite2D({ 181.0f - 640.0f + 45.0f * i, -60.0f + 360.0f }, { 38.0f, 37.0f }, heartTex);
			}
			for (int i = 0; i < energyNum; i++) {
				if (!(i == energyNum - 1 && count % 60 > 30)) {
					SPRITE.drawSprite2D({ 181.0f - 640.0f + 45.0f * i, -105.0f + 360.0f }, { 52.0f, 20.0f }, energyTex);
				}
			}
		}

	private:
		unsigned int heartTex;
		unsigned int energyTex;
		unsigned int crystalTex;
		unsigned int circleArrowTex;
		unsigned int baseCircleTex;
		float rotateRadian = 0.0f;
		int hpNum = 5;
		int energyNum = 5;
		int crystalNum = 0;
		int count = 0;
		Float4 crystalColor[3];
};