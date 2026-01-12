#pragma once

#include "IGameEvent.h"
#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"
#include "MathTool.h"
#include "Player.h"
#include "PlayerIdle.h"
#include "PlayerFreeze.h"
#include "MessageDialog.h"

class GreenCrystalEvent : public IGameEvent {
	public:
		GreenCrystalEvent() {
			PLAYER.setState(new PlayerFreeze());
			_dialog = new MessageDialog({
				{ Talker::SPIRIT, L"これで二つ目の水晶も手にいれだな。" },
				{ Talker::SPIRIT, L"今なら、別のレイヤーへ切り替えられるはずだ。" },
			});
		}
		~GreenCrystalEvent() {
			delete _dialog;
		}
		void update() override {
			_dialog->update();
			if (_dialog->isEnd()) {
				PLAYER.setState(new PlayerIdle());
			}
		}
		void draw() override {
			_dialog->draw();
		}
		bool isEnd() override {
			return _dialog->isEnd();
		}

	private:
		IDialog* _dialog;
		bool _isEnd = false;
};