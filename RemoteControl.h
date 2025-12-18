#pragma once

#include "Item.h"
#include "GetItemEvent.h"
#include "Player.h"

class RemoteControl : public Item {
	public:
		RemoteControl(Float3 pos, IGameEventHandler* gameEvent) : Item(pos, gameEvent) {
		}
		void getItem() override {
			_gameEvent->addEvent(new GetItemEvent(this, [=]() {
				PLAYER.getRemoteControl();
			}));
		}
		std::wstring getName() override {
			return L"出口の制御装置をゲット！";
		}
		ItemTag getTag() override {
			return ItemTag::REMOTE_CONTROL;
		}
};