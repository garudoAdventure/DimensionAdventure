#pragma once

#include "ActivableGameObj.h"
#include "Item.h"
#include "ModelManager.h"
#include "Keyboard.h"
#include "MathTool.h"
#include "IGameEventHandler.h"
#include "GetItemEvent.h"
#include "ShowDialogEvent.h"

class ItemBox : public ActivableGameObj {
	public:
		ItemBox(Float3 pos, Item* item, IGameEventHandler* gameEvent) : _item(item), _gameEvent(gameEvent) {
			_pos = pos;
			_size = { 2.0f, 2.0f, 2.0f };
			_color = { 1.0f, 0.0f, 0.0f, 0.8f };
			_triggerSize = { _size.x + 0.5f, _size.y + 0.5f, _size.z + 0.5f };
		}
		void onTrigger(GameObj* player) override {
			player->hitObj(this);
			if (_isOpen) return;
			if (Keyboard_IsKeyTrigger(KK_SPACE)) {
				if (_isLocked) {
					_gameEvent->addEvent(new ShowDialogEvent(
						new MessageDialog({"この宝箱はロックされた，開けない"})
					));
					return;
				}
				_isOpen = true;
				_playerPos = player->getPos();
				_gameEvent->addEvent(new GetItemEvent(_item, _playerPos));
			}
		}
		void update() {
			ActivableGameObj::update();
		}
		void draw() override {
			GameObj::draw();
			ActivableGameObj::drawHint({ _pos.x, _pos.y + _size.y / 2 + 1.0f, _pos.z });
		}
		void setLocked(bool locked) {
			_isLocked = locked;
		}

	private:
		Item* _item;
		IGameEventHandler* _gameEvent;
		Float3 _playerPos;
		bool _isOpen = false;
		bool _isLocked = false;
};