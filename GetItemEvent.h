#pragma once

#include "IGameEvent.h"
#include "Item.h"
#include "Dialog.h"
#include <sstream>

class GetItemEvent : public IGameEvent {
	public:
		GetItemEvent(Item* item) : _item(item) {
			std::stringstream ss;
			std::string context;
			ss << item->getName() << "‚ðƒQƒbƒgI";
			ss >> context;
			_dialog = new GetItemDialog({context});
		}
		void update() override {
			if (count == 180) {
				_isEnd = true;
			}
			else {
				count++;
				_dialog->update();
			}
		}
		void draw() override {
			_dialog->draw();
		}
		bool isEnd() override {
			return _isEnd;
		}

	private:
		Item* _item;
		IDialog* _dialog;
		int count = 0;
		bool _isEnd = false;
};