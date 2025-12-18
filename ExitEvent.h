#pragma once

#include "IGameEvent.h"
#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"
#include "MathTool.h"
#include "Player.h"
#include "MessageDialog.h"
#include "IGameEventHandler.h"

class ExitEvent : public IGameEvent {
	public:
		ExitEvent(IGameEventHandler* gameEvent) : _gameEvent(gameEvent) {
			PLAYER.setToEventState(true);
			_dialog = new MessageDialog({
				{ Talker::SELF, L"やった！" },
			});
			_coverColor.a = 0.0f;
		}
		~ExitEvent() {
			delete _dialog;
		}
		void update() override {
			_dialog->update();
			if (_dialog->isEnd()) {
				_coverCount++;
				_coverColor.a = MathTool::lerp<float>(0.0f, 1.0f, _coverCount / 240.0f);
				if (_coverCount == 240) {
					_gameEvent->setGameEnd();
				}
			}
		}
		void draw() override {
			_dialog->draw();
			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, _coverColor);
		}
		bool isEnd() override {
			return _isEnd;
		}

	private:
		IGameEventHandler* _gameEvent;
		IDialog* _dialog;
		bool _isEnd = false;
		int _coverCount = 0;
		Float4 _coverColor = Color::white;
};