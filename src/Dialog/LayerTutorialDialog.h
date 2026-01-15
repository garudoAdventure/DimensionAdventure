#pragma once

#include "Dialog.h"
#include "HALKeyboard.h"

class LayerTutorialDialog : public IDialog {
	public:
		LayerTutorialDialog() {
			_pos = { 0.0f, 0.0f };
			_size = { 0.0f, 500.0f };
			_margin = { 150.0f, 80.0f };
			_dialogWidth = 800.0f;
			_layerTex = TEXTURE.loadTexture("./assets/tutorial/layer.png");
			_spaceTex = TEXTURE.loadTexture("./assets/keySpace.png");
		}
		void dialogUpdate() override {
			if (Keyboard_IsKeyTrigger(KK_ENTER)) {
				_isEnd = true;
			}
		}
		void dialogDraw() override {
			IDialog::drawStr(L"を長押し、レイヤーの間で切り替えができる！", _pos);
			SPRITE.drawSprite2D({ -310.0f, 150.0f }, { 100.0f, 50.0f }, TEXTURE.getTexture(_spaceTex), Color::white);
			SPRITE.drawSprite2D({ 0.0f, -50.0f }, { 320.0f, 180.0f }, TEXTURE.getTexture(_layerTex), Color::white);
		}

	private:
		unsigned int _layerTex;
		unsigned int _spaceTex;
};