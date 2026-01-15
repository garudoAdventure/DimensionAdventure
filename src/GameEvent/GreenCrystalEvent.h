#pragma once

#include "IGameEvent.h"
#include "./Dialog/MessageDialog.h"

class GreenCrystalEvent : public IGameEvent {
	public:
		GreenCrystalEvent() {
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