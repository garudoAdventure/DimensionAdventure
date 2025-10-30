#pragma once

#include "MathStruct.h"
#include "GameObj.h"
#include "Keyboard.h"
#include "IGameEventHandler.h"
#include "Dialog.h"
#include "ShowDialogEvent.h"
#include "FieldFadeOutEvent.h"

class Door : public GameObj {
  public:
		Door(Float3 pos) {
			_pos = pos;
			_size = { 3.0f * 2, 3.0f * 2, 1.0f * 2 };
			_color = { 1.0f, 1.0f, 0.2f, 0.8f };
		}
		virtual void entry() = 0;
		virtual void update() = 0;
};

class LockedDoor : public Door {
	public:
		LockedDoor(Float3 pos, IGameEventHandler* gameEvent) : Door(pos), _gameEvent(gameEvent) {
		}
		void update() override {
		}
		void entry() override {
			if (Keyboard_IsKeyTrigger(KK_ENTER)) {
				Dialog* dialog = new Dialog({ "Œ®‚ª‚©‚©‚Á‚Ä‚¢‚é", "ŠJ‚¯‚È‚¢" });
				_gameEvent->addEvent(new ShowDialogEvent(dialog));
			}
		}
	private:
		IGameEventHandler* _gameEvent = nullptr;
};

class OpenedDoor : public Door {
	public:
		OpenedDoor(Float3 pos, int fieldID, IGameEventHandler* gameEvent, Float3 playerInitPos) :
			Door(pos), _gameEvent(gameEvent), _nextField(fieldID), _playerInitPos(playerInitPos) {
		}
		void update() override {
		}
		void entry() override {
			if (Keyboard_IsKeyTrigger(KK_ENTER)) {
				_gameEvent->setNewField(_nextField, _pos, _playerInitPos);
			}
		}

	private:
		IGameEventHandler* _gameEvent = nullptr;
		int _nextField;
		Float3 _playerInitPos;
};