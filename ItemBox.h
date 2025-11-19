#pragma once

#include "GameObj.h"
#include "Item.h"
#include "Keyboard.h"
#include "MathTool.h"
#include "IGameEventHandler.h"
#include "GetItemEvent.h"

class ItemBox : public GameObj {
	public:
		ItemBox(Float3 pos, Item* item, IGameEventHandler* gameEvent) : _item(item), _gameEvent(gameEvent) {
			_pos = pos;
			_size = { 2.0f, 2.0f, 2.0f };
			_color = { 1.0f, 0.0f, 0.0f, 0.8f };
			_hintDialog = new HintDialog(pos);
		}
		void trigger(Player* player) {
			_isTrigger = true;
			if (_isOpen) return;
			if (Keyboard_IsKeyTrigger(KK_SPACE)) {
				_isOpen = true;
				_playerPos = player->getPos();
				_item->get(player);
			}
		}
		Cube getTriggerBox() {
			Float3 triggerSize = { _size.x + 0.5f, _size.y + 0.5f, _size.z + 0.5f };
			return { _pos, triggerSize };
		}
		void update() {
			_isTrigger = false;
			if (_isOpen && !_isEnd) {
				if (animCount < 30) {
					animCount++;
				}
				else {
					if (appearCount < 60) {
						appearCount++;
					}
					else {
						_isEnd = true;
						_gameEvent->addEvent(new GetItemEvent(_item, _playerPos));
					}
				}
			}
		}
		void draw() override {
			GameObj::draw();
			if (_isOpen && appearCount < 60 ) {
				float posY = MathTool::easeInQuad<float>(_pos.y, _pos.y + 2.0f, animCount / 30.0f);
				_item->draw({ _pos.x, posY, _pos.z });
			}
			if (_isTrigger) {
				_hintDialog->draw();
			}
		}

	private:
		Item* _item;
		IGameEventHandler* _gameEvent;
		Float3 _playerPos;
		IDialog* _hintDialog = nullptr;
		bool _isTrigger = false;
		bool _isOpen = false;
		bool _isEnd = false;
		int animCount = 0;
		int appearCount = 0;
};