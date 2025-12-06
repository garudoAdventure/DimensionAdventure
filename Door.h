#pragma once

#include "MathStruct.h"
#include "ActivableGameObj.h"
#include "Model.h"
#include "Keyboard.h"
#include "IGameEventHandler.h"
#include "Player.h"
#include "ShowDialogEvent.h"

class Door : public ActivableGameObj {
  public:
		Door(Float3 pos) {
			_pos = pos;
			_color = { 1.0f, 1.0f, 0.2f, 0.8f };
			_model = new Model("./assets/model/door.fbx");
			_size = _model->getSize();
			_triggerSize = { _size.x, _size.y, _size.z + 0.5f };
		}
		virtual void update() {
			ActivableGameObj::update();
		}
		virtual void draw() {
			ActivableGameObj::drawHint({ _pos.x, _pos.y + _size.y / 2 + 1.0f, _pos.z });
			_model->updateColor(_color);
			_model->draw(_pos, { 0.0f, 0.0f, 0.0f });
		}

	protected:
		Model* _model;
};

class LockedDoor : public Door {
	public:
		LockedDoor(Float3 pos, IGameEventHandler* gameEvent) : Door(pos), _gameEvent(gameEvent) {
			_color = { 1.0f, 1.0f, 1.0f, 0.3f };
		}
		void onTrigger(GameObj* player) override {
			if (MathTool::checkCollision(player->getBox(), this->getBox(), false)) {
				player->hitObj(this);
			}
			if (Keyboard_IsKeyTrigger(KK_ENTER)) {
				IDialog* dialog = new MessageDialog({
					{ Talker::SELF, "Œ®‚ª‚©‚©‚Á‚Ä‚¢‚é" },
					{ Talker::SELF, "ŠJ‚¯‚È‚¢" }
				});
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
		void onTrigger(GameObj* player) override {
			if (MathTool::checkCollision(player->getBox(), this->getBox(), false)) {
				player->hitObj(this);
			}
			if (Keyboard_IsKeyTrigger(KK_ENTER)) {
				_gameEvent->setNewField(_nextField, _pos, _playerInitPos);
			}
		}

	private:
		IGameEventHandler* _gameEvent = nullptr;
		int _nextField;
		Float3 _playerInitPos;
};