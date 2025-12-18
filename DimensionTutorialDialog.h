#pragma once

#include "Dialog.h"
#include "Keyboard.h"

class DimensionTutorialDialog : public IDialog {
	public:
		DimensionTutorialDialog() {
			_pos = { 0.0f, 0.0f };
			_size = { 800.0f, 500.0f };
			_margin.y = 80.0f;
			_margin.x = 150.0f;
			_pic2DTex = TEXTURE.loadTexture("./assets/tutorial/2D.png");
			_pic3DTex = TEXTURE.loadTexture("./assets/tutorial/3D.png");
			_arrowTex = TEXTURE.loadTexture("./assets/UI/arrow.png");
			_keySpaceTex = TEXTURE.loadTexture("./assets/keySpace.png");
		}
		void update() override {
			if (Keyboard_IsKeyTrigger(KK_ENTER)) {
				_isEnd = true;
			}
		}
		void draw() override {
			IDialog::drawMessageBox();
			IDialog::drawStr(L"を押して、2Dと3D世界の切り替えができる！", _pos);
			SPRITE.drawSprite2D({ -310.0f, 150.0f }, { 100.0f, 50.0f }, _keySpaceTex);
			SPRITE.drawSprite2D({ -200.0f, -50.0f }, { 320.0f, 180.0f }, _pic2DTex);
			SPRITE.drawSprite2D({ 200.0f, -50.0f }, { 320.0f, 180.0f }, _pic3DTex);
			SPRITE.drawSprite2D({ 0.0f, 80.0f }, { 94.0f, 34.0f }, _arrowTex);
			SPRITE.drawSprite2DRotate({ 0.0f, 180.0f }, { 94.0f, 34.0f }, _arrowTex, PI, { 0.0f, 0.0f });
		}

	private:
		unsigned int _pic2DTex;
		unsigned int _pic3DTex;
		unsigned int _arrowTex;
		unsigned int _keySpaceTex;
};