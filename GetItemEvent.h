#pragma once

#include "IGameEvent.h"
#include "Item.h"
#include "SystemDialog.h"
#include <functional>

class GetItemEvent : public IGameEvent {
	public:
		GetItemEvent(Item* item, std::function<void()> callback) : _item(item), _callback(callback) {
			_dialog = new SystemDialog({ item->getName() });
		}
		~GetItemEvent() {
			delete _dialog;
		}
		void update() override {
			if (count == 180) {
				_isEnd = true;
				_callback();
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
		std::function<void()> _callback;
		int count = 0;
		bool _isEnd = false;
};