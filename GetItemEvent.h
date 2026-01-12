#pragma once

#include "IGameEvent.h"
#include "Item.h"
#include "SystemDialog.h"
#include "Sound.h"
#include <functional>

class GetItemEvent : public IGameEvent {
	public:
		GetItemEvent(IGameEventHandler* gameEvent, Item* item, std::function<void()> callback) : _gameEvent(gameEvent), _item(item), _callback(callback) {
			_dialog = new SystemDialog({ item->getName() });
			_getItemSE = SOUND.loadSound("./assets/sound/getItem.wav");
			SOUND.playSound(_getItemSE, 0);
			SOUND.setVolume(gameEvent->getBgmId(), 0);
		}
		~GetItemEvent() {
			delete _dialog;
		}
		void update() override {
			if (_count == 300) {
				_isEnd = true;
				SOUND.setVolume(_gameEvent->getBgmId(), 0.5);
				_callback();
			}
			else {
				_count++;
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
		unsigned int _getItemSE;
		IGameEventHandler* _gameEvent;
		Item* _item;
		IDialog* _dialog;
		std::function<void()> _callback;
		int _count = 0;
		bool _isEnd = false;
};