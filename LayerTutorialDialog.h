#pragma once

#include "Dialog.h"
#include "Keyboard.h"
#include "Player.h"

class LayerTutorialDialog : public IDialog {
	public:
		LayerTutorialDialog() {
			_pos = { 0.0f, 0.0f };
			_size = { 800.0f, 500.0f };
			_margin.y = 80.0f;
			_margin.x = 150.0f;
			_layerTex = TEXTURE.loadTexture("./assets/tutorial/layer.png");
			_spaceTex = TEXTURE.loadTexture("./assets/keySpace.png");
			PLAYER.setToEventState(true);
		}
		void update() override {
			if (Keyboard_IsKeyTrigger(KK_ENTER)) {
				_isEnd = true;
				PLAYER.setToEventState(false);
			}
		}
		void draw() override {
			IDialog::drawMessageBox();
			IDialog::drawStr(L"を長押し、レイヤーの間で切り替えができる！", _pos);
			SPRITE.drawSprite2D({ -310.0f, 150.0f }, { 100.0f, 50.0f }, _spaceTex);
			SPRITE.drawSprite2D({ 0.0f, -50.0f }, { 320.0f, 180.0f }, _layerTex);
		}

	private:
		unsigned int _layerTex;
		unsigned int _spaceTex;
};