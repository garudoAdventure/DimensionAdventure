#pragma once

#include "IGameEvent.h"
#include "Item.h"
#include "Dialog.h"
#include <sstream>

class GetItemEvent : public IGameEvent {
	public:
		GetItemEvent(Item* item, Float3 playerPos) : _item(item), _playerPos(playerPos) {
			std::stringstream ss;
			std::string context;
			ss << item->getName() << "‚ðƒQƒbƒgI";
			ss >> context;
			_dialog = new GetItemDialog({context});
		}
		void update() override {
			if (count == 300) {
				_isEnd = true;
			}
			else {
				count++;
				_dialog->update();
			}
		}
		void draw() override {
			_item->draw({
				_playerPos.x,
				_playerPos.y + 3.0f,
				_playerPos.z
			});
			_dialog->draw();
		}
		bool isEnd() override {
			return _isEnd;
		}

	private:
		Item* _item;
		Dialog* _dialog;
		Float3 _playerPos;
		int count = 0;
		bool _isEnd = false;
};