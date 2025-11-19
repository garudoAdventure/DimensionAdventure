#pragma once

#include "MathStruct.h"
#include "GameObj.h"
#include "Model.h"
#include "Keyboard.h"
#include "IGameEventHandler.h"
#include "Dialog.h"
#include "Player.h"
#include "ShowDialogEvent.h"
#include "FieldFadeOutEvent.h"

class Door : public GameObj {
  public:
		Door(Float3 pos) {
			_pos = pos;
			_color = { 1.0f, 1.0f, 0.2f, 0.8f };
			_model = new Model("./assets/model/door.fbx");
			_size = _model->getSize();
		}
		virtual void trigger(Player* player) = 0;
		virtual void update() = 0;

	protected:
		Model* _model;
};

class LockedDoor : public Door {
	public:
		LockedDoor(Float3 pos, IGameEventHandler* gameEvent) : Door(pos), _gameEvent(gameEvent) {
		}
		void update() override {
		}
		void trigger(Player* player) override {
			if (Keyboard_IsKeyTrigger(KK_ENTER)) {
				IDialog* dialog = new NormalDialog({ "Œ®‚ª‚©‚©‚Á‚Ä‚¢‚é", "ŠJ‚¯‚È‚¢" });
				_gameEvent->addEvent(new ShowDialogEvent(dialog));
			}
		}
	private:
		IGameEventHandler* _gameEvent = nullptr;
};

class OpenedDoor : public Door {
	public:
		OpenedDoor(Float3 pos, int fieldID, IGameEventHandler* gameEvent, Float3 playerInitPos) :
			Door(pos), _gameEvent(gameEvent), _nextField(fieldID), _playerInitPos(playerInitPos)
		{
			_hintDialog = new HintDialog(pos);
		}
		void update() override {
			isTrigger = false;
		}
		void draw() override {
			_model->draw(_pos, { 0.0f, 0.0f, 0.0f });
			if (isTrigger) {
				_hintDialog->draw();
			}
		}
		void trigger(Player* player) override {
			isTrigger = true;
			if (Keyboard_IsKeyTrigger(KK_ENTER)) {
				_gameEvent->setNewField(_nextField, _pos, _playerInitPos);
			}
		}

	private:
		IGameEventHandler* _gameEvent = nullptr;
		int _nextField;
		Float3 _playerInitPos;
		bool isTrigger = false;
		IDialog* _hintDialog = nullptr;
};