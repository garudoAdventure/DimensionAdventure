#pragma once

#include "Dialog.h"
#include "HALKeyboard.h"

class DimensionTutorialDialog : public IDialog {
	public:
		DimensionTutorialDialog() {
			_pos = { 0.0f, 0.0f };
			_margin = { 150.0f, 80.0f };
			_size.y = 500.0f;
			_dialogWidth = 800.0f;
			_pic2DTex = TEXTURE.loadTexture("./assets/tutorial/2D.png");
			_pic3DTex = TEXTURE.loadTexture("./assets/tutorial/3D.png");
			_arrowTex = TEXTURE.loadTexture("./assets/UI/arrow.png");
			_keySpaceTex = TEXTURE.loadTexture("./assets/keySpace.png");
		}
		void dialogUpdate() override {
			if (Keyboard_IsKeyTrigger(KK_ENTER)) {
				_isEnd = true;
			}
		}
		void dialogDraw() override {
			IDialog::drawStr(L"を押して、2Dと3Dの世界を切り替えろ！", _pos);
			SPRITE.drawSprite2D({ -310.0f, 150.0f }, { 100.0f, 50.0f }, TEXTURE.getTexture(_keySpaceTex), Color::white);
			SPRITE.drawSprite2D({ -200.0f, -50.0f }, { 320.0f, 180.0f }, TEXTURE.getTexture(_pic2DTex), Color::white);
			SPRITE.drawSprite2D({ 200.0f, -50.0f }, { 320.0f, 180.0f }, TEXTURE.getTexture(_pic3DTex), Color::white);
			SPRITE.drawSprite2D({ 0.0f, 80.0f }, { 94.0f, 34.0f }, TEXTURE.getTexture(_arrowTex), Color::white);
			SPRITE.drawSprite2DRotate({ 0.0f, 180.0f }, { 94.0f, 34.0f }, TEXTURE.getTexture(_arrowTex), PI, { 0.0f, 0.0f });
		}

	private:
		unsigned int _pic2DTex;
		unsigned int _pic3DTex;
		unsigned int _arrowTex;
		unsigned int _keySpaceTex;
};