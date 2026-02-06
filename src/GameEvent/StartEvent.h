#pragma once

#include "IGameEvent.h"
#include "Sound.h"
#include "FadeCover.h"
#include "./Game/IGameEventHandler.h"
#include "./Render/Shader.h"
#include "./Render/Texture.h"
#include "./Render/Sprite.h"
#include "./Dialog/MessageDialog.h"
#include "./Player/Player.h"

class StartEvent : public IGameEvent {
	public:
		static constexpr int FADE_IN_TIME = 180;

		StartEvent(IGameEventHandler* gameEvent) : _gameEvent(gameEvent) {
			_dialog[0] = new MessageDialog({
				{ Talker::SELF, L"......" },
				{ Talker::SELF, L"うう......。" },
			});
			_dialog[1] = new MessageDialog({
				{ Talker::SELF, L"私は一体......？" },
				{ Talker::SPIRIT, L"やっと目が覚めたか。" },
				{ Talker::SELF, L"ここは......？" },
				{ Talker::SPIRIT, L"ここは次元迷宮だ。" },
				{ Talker::SELF, L"次元......迷宮......？" },
				{ Talker::SPIRIT, L"どうやら何らかのトラブルで、君はここに迷い込んでしまったようだ。" },
				{ Talker::SELF, L"あなたは......誰ですか？" },
				{ Talker::SPIRIT, L"私はこの迷宮の守護精霊、ジーゲンだ。" },
				{ Talker::SELF, L"じいさんですね。よろしくお願いします。" },
				{ Talker::SPIRIT, L"じいさんって言うな！" },
				{ Talker::SPIRIT, L"とりあえず、この迷宮から脱出するには、三つの水晶が必要だ。" },
				{ Talker::SPIRIT, L"まず一つ目の赤い水晶を探すといい。" },
				{ Talker::SELF, L"わかりました。" },
				{ Talker::SPIRIT, L"困ったときは、いつでも相談に乗ろう。" },
			});
			_fadeCover = new FadeCover(FADE_IN_TIME);
		}

		~StartEvent() {
			delete _dialog[0];
			delete _dialog[1];
			delete _fadeCover;
		}

		void update() override {
			PLAYER.update();
			_gameEvent->updateField();
			if (!_isWakeUp) {
				_dialog[0]->update();
				if (_dialog[0]->isEnd()) {
					if (_fadeCover->fadeIn()) {
						_isWakeUp = true;
					}
				}
			}
			else {
				_dialog[1]->update();
				if (_dialog[1]->isEnd()) {
					SOUND.setVolume(_gameEvent->getBgmId(), 0.3f);
					SOUND.playSound(_gameEvent->getBgmId(), -1);
				}
			}			
		}

		void draw() override {
			if (!_isWakeUp) {
				_fadeCover->draw();
				if (!_dialog[0]->isEnd()) {
					_dialog[0]->draw();
				}
			}
			else {
				_dialog[1]->draw();
			}
		}

		bool isEnd() override {
			return _dialog[1]->isEnd();
		}

	private:
		IGameEventHandler* _gameEvent;
		IDialog* _dialog[2];
		FadeCover* _fadeCover;
		bool _isEnd = false;
		bool _isWakeUp = false;
};