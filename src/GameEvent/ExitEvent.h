#pragma once

#include "IGameEvent.h"
#include "Sound.h"
#include "FadeCover.h"
#include "./Game/IGameEventHandler.h"
#include "./Dialog/MessageDialog.h"
#include "./Render/Shader.h"
#include "./Render/Texture.h"
#include "./Render/Sprite.h"
#include "./Utils/MathTool.h"

class ExitEvent : public IGameEvent {
	public:
		static constexpr int FADE_OUT_TIME = 240;
		
		ExitEvent(IGameEventHandler* gameEvent) : _gameEvent(gameEvent) {
			_dialog = new MessageDialog({
				{ Talker::SELF, L"やった！" },
			});
			_fadeCover = new FadeCover(FADE_OUT_TIME, Color::white);
			_exitSE = SOUND.loadSound("./assets/sound/exit.wav");
		}

		~ExitEvent() {
			delete _dialog;
			delete _fadeCover;
		}

		void update() override {
			_dialog->update();
			if (_dialog->isEnd()) {
				if (!_isPlayeSE) {
					SOUND.stopSound(_gameEvent->getBgmId());
					SOUND.playSound(_exitSE, 0);
					_isPlayeSE = true;
				}
				if (_fadeCover->fadeOut()) {
					_gameEvent->setGameEnd();
				}
			}
		}

		void draw() override {
			_dialog->draw();
			if (_dialog->isEnd()) {
				_fadeCover->draw();
			}
		}

		bool isEnd() override {
			return _isEnd;
		}

	private:
		IGameEventHandler* _gameEvent;
		IDialog* _dialog;
		FadeCover* _fadeCover;
		unsigned int _exitSE;
		bool _isEnd = false;
		bool _isPlayeSE = false;
		int _coverCount = 0;
};