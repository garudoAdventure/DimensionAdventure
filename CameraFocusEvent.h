#pragma once

#include "IGameEvent.h"
#include "MathStruct.h"
#include "IGameEventHandler.h"
#include "Player.h"
#include "PlayerIdle.h"

class CameraFocusEvent : public IGameEvent {
	public:
		CameraFocusEvent(IGameEventHandler* gameEvent, Float3 targetPos) : _gameEvent(gameEvent), _targetPos(targetPos) {
		}
		void update() override {
			_gameEvent->updateField();
			_gameEvent->moveCamera(_targetPos);
			_count++;
			if (_count == 180) {
				PLAYER.setState(new PlayerIdle());
			}
		}
		void draw() override {
		}
		bool isEnd() override {
			return _count > 180;
		}

	private:
		IGameEventHandler* _gameEvent;
		Float3 _targetPos;
		int _count = 0;
};