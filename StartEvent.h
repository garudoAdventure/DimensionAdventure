#pragma once

#include "IGameEvent.h"
#include "IGameEventHandler.h"
#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"
#include "MathTool.h"
#include "Player.h"
#include "PlayerIdle.h"
#include "MessageDialog.h"
#include "Sound.h"

class StartEvent : public IGameEvent {
	public:
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
		}
		~StartEvent() {
			delete _dialog[0];
			delete _dialog[1];
		}
		void update() override {
			PLAYER.update();
			_gameEvent->updateField();
			switch (_eventPhase) {
				case 0:
					_dialog[0]->update();
					if (_dialog[0]->isEnd()) {
						_coverCount++;
						_coverColor.a = 1 - (_coverCount / 180.0f);
						if (_coverCount == 180) {
							_eventPhase = 1;
						}
					}
					break;
				case 1:
					_dialog[1]->update();
					if (_dialog[1]->isEnd()) {
						SOUND.setVolume(_gameEvent->getBgmId(), 0.5f);
						SOUND.playSound(_gameEvent->getBgmId(), -1);
					}
					break;
			}
			
		}
		void draw() override {
			switch (_eventPhase) {
				case 0:
					SHADER.setPS(PS::NO_TEX);
					SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, _coverColor);
					_dialog[0]->draw();
					break;
				case 1:
					_dialog[1]->draw();
					if (_dialog[1]->isEnd()) {
						PLAYER.setState(new PlayerIdle());
					}
					break;
			}
		}
		bool isEnd() override {
			return _dialog[1]->isEnd();
		}

	private:
		IGameEventHandler* _gameEvent;
		IDialog* _dialog[2];
		bool _isEnd = false;
		int _coverCount = 0;
		Float4 _coverColor = Color::black;
		int _eventPhase = 0;
};