#pragma once

#include "MathStruct.h"
#include "ActivableGameObj.h"
#include "Model.h"
#include "ModelManager.h"
#include "Keyboard.h"
#include "IGameEventHandler.h"
#include "Player.h"
#include "ShowDialogEvent.h"
#include "MessageDialog.h"
#include "PasswordDialog.h"
#include "ConfirmDialog.h"

class Door : public ActivableGameObj {
  public:
		Door(Float3 pos) {
			_pos = pos;
			_color = Color::yellow;
			_model = MODEL.loadModel("./assets/model/door.fbx");
			_size = _model->getSize();
			_triggerSize = { _size.x, _size.y, _size.z + 0.5f };
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
				if (_isActive) {
					_gameEvent->addEvent(new ShowDialogEvent(
						new PasswordDialog(_gameEvent)
					));
					return;
				}
				if (!PLAYER.hasRemoteControl()) {
					_gameEvent->addEvent(new ShowDialogEvent(
						new MessageDialog({
						{ Talker::SELF, L"これが出口か......" },
						{ Talker::SELF, L"でも、まだ起動してないみたいだ。" },
					})));
				}
				else {
					_gameEvent->addEvent(new ShowDialogEvent(
						new ConfirmDialog({ L"制御装置を使いますが？" }, [=]() {
							_isActive = true;
							_color = Color::yellow;
						})
					));
					_gameEvent->setCheckpoint(CheckPoint::PASSWORD);
				}
			}
		}
	private:
		IGameEventHandler* _gameEvent = nullptr;
		bool _isActive = false;
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
				if (_nextField == 5 && _gameEvent->getCheckpoint() == CheckPoint::RED_CRYSTAL) {
					_gameEvent->setCheckpoint(CheckPoint::GREEN_HINT);
				}
				if (_nextField == 6 && _gameEvent->getCheckpoint() == CheckPoint::GREEN_CRYSTAL) {
					_gameEvent->setCheckpoint(CheckPoint::BLUE_HINT);
				}
				if (_nextField == 0 && _gameEvent->getCheckpoint() == CheckPoint::REMOTE_CONTROL) {
					_gameEvent->setCheckpoint(CheckPoint::WHITE_DOOR);
				}
				if (_nextField != 0 && _gameEvent->getCheckpoint() == CheckPoint::PASSWORD) {
					_gameEvent->setCheckpoint(CheckPoint::FINAL);
				}
				if (_nextField == 0 && _gameEvent->getCheckpoint() == CheckPoint::FINAL) {
					_gameEvent->setCheckpoint(CheckPoint::PASSWORD);
				}
			}
		}

	private:
		IGameEventHandler* _gameEvent = nullptr;
		int _nextField;
		Float3 _playerInitPos;
};

class MazeDoor : public Door {
	public:
		MazeDoor(Float3 pos, int fieldID, IGameEventHandler* gameEvent, Float3 playerInitPos) :
			Door(pos), _gameEvent(gameEvent), _nextField(fieldID), _playerInitPos(playerInitPos) {
		}
		void onTrigger(GameObj* player) override {
			if (MathTool::checkCollision(player->getBox(), this->getBox(), false)) {
				player->hitObj(this);
			}
			if (_pos.z > player->getPos().z && _pos.z - player->getPos().z < 9.0f) {
				if (Keyboard_IsKeyTrigger(KK_ENTER)) {
					_gameEvent->setNewField(_nextField, _pos, _playerInitPos);
				}
			}
		}

	private:
		IGameEventHandler* _gameEvent = nullptr;
		int _nextField;
		Float3 _playerInitPos;
};