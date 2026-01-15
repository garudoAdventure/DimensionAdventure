#pragma once

#include "./Player/Player.h"
#include "./Render/Texture.h"
#include "./Render/Sprite.h"

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
			SHADER.setPS(PS::GENERAL);
			SPRITE.drawSprite2D({ -545.0f, 277.0f }, { 108.0f, 102.0f }, TEXTURE.getTexture(baseCircleTex), Color::white);
			if (!PLAYER.hasDimensionRing()) {
				SPRITE.drawSprite2D({ -550.0f, 278.0f }, { 126.0f, 119.0f }, TEXTURE.getTexture(circleArrowTex), Color::black);
			}
			else {
				SPRITE.drawSprite2DRotate({ -550.0f, 278.0f }, { 126.0f, 119.0f }, TEXTURE.getTexture(circleArrowTex), rotateRadian, { -545.0f, 278.0f });
			}
			for (int i = 0; i < crystalNum; i++) {
				SPRITE.drawSprite2DRotate(
					{ -545.0f, 299.0f }, { 22.0f, 38.0f },
					TEXTURE.getTexture(crystalTex), 2 * PI * i / 3.0f, { -545.0f, 277.0f },
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