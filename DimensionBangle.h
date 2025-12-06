#pragma once

#include "Item.h"
#include "GetItemEvent.h"
#include "Player.h"
#include "Dialog.h"

class DimensionBangle : public Item {
	public:
		DimensionBangle(Float3 pos, IGameEventHandler* gameEvent) : Item(pos, gameEvent) {
		}
		void getItem() override {
			_gameEvent->addEvent(new GetItemEvent(this, [=]() {
				PLAYER.getDimensionAbility();
				_gameEvent->addEvent(new ShowDialogEvent(
					new DimensionTutorialDialog()
				));
			}));
		}
		std::string getName() override {
			return "ŽŸŒ³ƒoƒ“ƒOƒ‹";
		}
		ItemTag getTag() override {
			return ItemTag::BANGLE;
		}
};