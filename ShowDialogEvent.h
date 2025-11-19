#pragma once

#include "IGameEvent.h"
#include "Dialog.h"

class ShowDialogEvent : public IGameEvent {
	public:
		ShowDialogEvent(IDialog* dialog) : _dialog(dialog) {
		}
		~ShowDialogEvent() {
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
};