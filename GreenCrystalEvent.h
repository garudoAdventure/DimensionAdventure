#pragma once

#include "IGameEvent.h"
#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"
#include "MathTool.h"
#include "Player.h"
#include "MessageDialog.h"

class GreenCrystalEvent : public IGameEvent {
	public:
		GreenCrystalEvent() {
			PLAYER.setToEventState(true);
			_dialog = new MessageDialog({
				{ Talker::SPIRIT, L"これでニコ目の水晶もゲットだね" },
				{ Talker::SPIRIT, L"今はできるはずです" },
				{ Talker::SPIRIT, L"他のレイヤーに行くこと" },
			});
		}
		~GreenCrystalEvent() {
			delete _dialog;
		}
		void update() override {
			_dialog->update();
			if (_dialog->isEnd()) {
				PLAYER.setToEventState(false);
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