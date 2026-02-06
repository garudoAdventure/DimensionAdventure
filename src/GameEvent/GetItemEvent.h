#pragma once

#include "IGameEvent.h"
#include "Sound.h"
#include "./Game/IGameEventHandler.h"
#include "./Dialog/SystemDialog.h"
#include "./Item/Item.h"
#include <functional>

class GetItemEvent : public IGameEvent {
	public:
		static constexpr int SHOW_GET_ITEM_TIME = 300;

		GetItemEvent(IGameEventHandler* gameEvent, Item* item, std::function<void()> callback) :
			_gameEvent(gameEvent), _item(item), _triggerGetItemFn(callback)
		{
			_dialog = new SystemDialog({ item->getName() + L"をゲット！" });
			_getItemSE = SOUND.loadSound("./assets/sound/getItem.wav");
			SOUND.playSound(_getItemSE, 0);
			SOUND.setVolume(gameEvent->getBgmId(), 0);
		}
		~GetItemEvent() {
			delete _dialog;
		}

		void update() override {
			if (_count < SHOW_GET_ITEM_TIME) {
				_count++;
				_dialog->update();
			} else {
				_isEnd = true;
				SOUND.setVolume(_gameEvent->getBgmId(), 0.3);
				_triggerGetItemFn();
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
		std::function<void()> _triggerGetItemFn;
		int _count = 0;
		bool _isEnd = false;
};