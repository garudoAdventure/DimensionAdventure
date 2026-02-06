#pragma once

#include "IGameEvent.h"
#include "./Game/IGameEventHandler.h"
#include "./Common/MathStruct.h"

class CameraFocusEvent : public IGameEvent {
	public:
		static constexpr int CAMERA_FOCUS_TIME = 180;

		CameraFocusEvent(IGameEventHandler* gameEvent, Float3 targetPos) :
			_gameEvent(gameEvent), _targetPos(targetPos) {
		}

		void update() override {
			_gameEvent->updateField();
			_gameEvent->moveCamera(_targetPos);
			_count++;
		}

		void draw() override {
		}

		bool isEnd() override {
			return _count >= CAMERA_FOCUS_TIME;
		}

	private:
		IGameEventHandler* _gameEvent;
		Float3 _targetPos;
		int _count = 0;
};