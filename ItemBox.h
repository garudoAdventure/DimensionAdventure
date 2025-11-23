#pragma once

#include "ActivableGameObj.h"
#include "Item.h"
#include "ModelManager.h"
#include "Keyboard.h"
#include "MathTool.h"
#include "IGameEventHandler.h"
#include "GetItemEvent.h"

class ItemBox : public ActivableGameObj {
	public:
		ItemBox(Float3 pos, Item* item, IGameEventHandler* gameEvent) : _item(item), _gameEvent(gameEvent) {
			_pos = pos;
			_size = { 2.0f, 2.0f, 2.0f };
			_color = { 1.0f, 0.0f, 0.0f, 0.8f };
			_triggerSize = { _size.x + 0.5f, _size.y + 0.5f, _size.z + 0.5f };
		}
		void onTrigger(Player* player) override {
			player->hitObj(this);
			if (_isOpen) return;
			if (Keyboard_IsKeyTrigger(KK_SPACE)) {
				if (_isLocked) {
					_gameEvent->addEvent(new ShowDialogEvent(
						new NormalDialog({"この宝箱はロックされた，開けない"})
					));
					return;
				}
				_isOpen = true;
				_playerPos = player->getPos();
				_item->get(player);
			}
		}
		void update() {
			ActivableGameObj::update();
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
			ActivableGameObj::draw();
			if (_isOpen && appearCount < 60 ) {
				float posY = MathTool::easeInQuad<float>(_pos.y, _pos.y + 2.0f, animCount / 30.0f);
				_item->draw3D({ _pos.x, posY, _pos.z });
			}
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
		bool _isEnd = false;
		int animCount = 0;
		int appearCount = 0;
};