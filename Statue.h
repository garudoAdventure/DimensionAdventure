#pragma once

#include "ActivableGameObj.h"
#include "Player.h"
#include "Item.h"
#include "Keyboard.h"
#include "IGameEventHandler.h"
#include "ShowDialogEvent.h"
#include "Dialog.h"
#include <functional>

class Statue : public ActivableGameObj {
	public:
		Statue(Float3 pos, IGameEventHandler* gameEvent) : _gameEvent(gameEvent) {
			_pos = pos;
			_size = { 3.0f, 7.0f, 3.0f };
			_triggerSize = { _size.x + 0.5f, _size.y + 0.5f, _size.z + 0.5f };
		}
		void update() override {
			ActivableGameObj::update();
		}
		void draw() override {
			GameObj::draw();
			Float3 hintPos = { _pos.x, _pos.y + _size.y / 2 + 1.0f, _pos.z };
			if (statueItem) {
				statueItem->draw3D({ _pos.x, _pos.y + _size.y / 2 + 1.0f, _pos.z });
				hintPos.y += 2.0f;
			}
			ActivableGameObj::drawHint(hintPos);
		}
		void onTrigger(GameObj* player) {
			player->hitObj(this);

			if (Keyboard_IsKeyTrigger(KK_SPACE)) {
				if (statueItem) {
					_gameEvent->addEvent(new ShowDialogEvent(
						new ConfirmDialog({ "アイテムを台座から取りますが？" }, [=]() {
							_gameEvent->getItemList()->addItem(statueItem);
							this->removeStatueItem();
						})
					));
					return;
				}
				Item* item = _gameEvent->getItemList()->getCurrentItem();
				if (item->getTag() == ItemTag::STATUE) {
					_gameEvent->addEvent(new ShowDialogEvent(
						new ConfirmDialog({ "アイテムを台座に置きますが？" }, [=]() {
							this->setStatueItem(item);
							_gameEvent->getItemList()->removeItem();
						})
					));
					return;
				}
				_gameEvent->addEvent(new ShowDialogEvent(
					new MessageDialog({ "何らかの台座らしいです。" })
				));
			}
		}
		virtual void setStatueItem(Item* item) {
			statueItem = item;
		}
		virtual void removeStatueItem() {
			statueItem = nullptr;
		}

	protected:
		IGameEventHandler* _gameEvent = nullptr;
		Item* statueItem = nullptr;
};

class RedBirdStatue : public Statue {
	public:
		RedBirdStatue(Float3 pos, IGameEventHandler* gameEvent) : Statue(pos, gameEvent) {
		}
		void setStatueItem(Item* item) override {
			Statue::setStatueItem(item);
			if (item->getName() == "朱雀の像") {
				_gameEvent->setFourGodCorrect(0, true);
			}
		}
		void removeStatueItem() override {
			Statue::removeStatueItem();
			_gameEvent->setFourGodCorrect(0, false);
		}
};

class BlueDragonStatue : public Statue {
	public:
		BlueDragonStatue(Float3 pos, IGameEventHandler* gameEvent) : Statue(pos, gameEvent) {
		}
		void setStatueItem(Item* item) override {
			Statue::setStatueItem(item);
			if (item->getName() == "青龍の像") {
				_gameEvent->setFourGodCorrect(1, true);
			}
		}
		void removeStatueItem() override {
			Statue::removeStatueItem();
			_gameEvent->setFourGodCorrect(1, false);
		}
};

class WhiteTigerStatue : public Statue {
	public:
		WhiteTigerStatue(Float3 pos, IGameEventHandler* gameEvent) : Statue(pos, gameEvent) {
		}
		void setStatueItem(Item* item) override {
			Statue::setStatueItem(item);
			if (item->getName() == "白虎の像") {
				_gameEvent->setFourGodCorrect(2, true);
			}
		}
		void removeStatueItem() override {
			Statue::removeStatueItem();
			_gameEvent->setFourGodCorrect(2, false);
		}
};

class GreenTurtleStatue : public Statue {
	public:
		GreenTurtleStatue(Float3 pos, IGameEventHandler* gameEvent) : Statue(pos, gameEvent) {
		}
		void setStatueItem(Item* item) override {
			Statue::setStatueItem(item);
			if (item->getName() == "玄武の像") {
				_gameEvent->setFourGodCorrect(3, true);
			}
		}
		void removeStatueItem() override {
			Statue::removeStatueItem();
			_gameEvent->setFourGodCorrect(3, false);
		}
};
