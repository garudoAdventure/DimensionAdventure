#pragma once

#include "Item.h"
#include "GetItemEvent.h"
#include "Player.h"
#include "TrapEvent.h"
#include "GreenCrystalEvent.h"

class RedCrystal : public Item {
	public:
		RedCrystal(Float3 pos, IGameEventHandler* gameEvent) : Item(pos, gameEvent) {
		}
		void getItem() override {
			_gameEvent->addEvent(new GetItemEvent(this, [=]() {
				PLAYER.addCrystalNum();
				if (!PLAYER.is2D()) {
					PLAYER.convertDimension();
				}
				_gameEvent->addEvent(new TrapEvent(_gameEvent));
			}));
		}
		std::string getName() override {
			return "次元水晶（赤）";
		}
		ItemTag getTag() override {
			return ItemTag::CRYSTAL;
		}
};

class GreenCrystal : public Item {
	public:
		GreenCrystal(Float3 pos, IGameEventHandler* gameEvent) : Item(pos, gameEvent) {
		}
		void getItem() override {
			_gameEvent->addEvent(new GetItemEvent(this, [=]() {
				PLAYER.addCrystalNum();
				//if (!PLAYER.is2D()) {
				//	PLAYER.convertDimension();
				//}
				//_gameEvent->addEvent(new GreenCrystalEvent());
			}));
		}
		std::string getName() override {
			return "次元水晶（緑）";
		}
		ItemTag getTag() override {
			return ItemTag::CRYSTAL;
		}
};

class BlueCrystal : public Item {
	public:
		BlueCrystal(Float3 pos, IGameEventHandler* gameEvent) : Item(pos, gameEvent) {
		}
		void getItem() override {
			_gameEvent->addEvent(new GetItemEvent(this, [=]() {
				PLAYER.addCrystalNum();
			}));
		}
		std::string getName() override {
			return "次元水晶（青）";
		}
		ItemTag getTag() override {
			return ItemTag::CRYSTAL;
		}
};