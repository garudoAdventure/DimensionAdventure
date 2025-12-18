#pragma once

#include "IGameEvent.h"
#include "IGameEventHandler.h"
#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"
#include "MathTool.h"
#include "Player.h"
#include "MessageDialog.h"

class StartEvent : public IGameEvent {
	public:
		StartEvent(IGameEventHandler* gameEvent) : _gameEvent(gameEvent) {
			PLAYER.setToEventState(true);
			_dialog[0] = new MessageDialog({
				{ Talker::SELF, L"。。。。。。" },
				{ Talker::SELF, L"ううう。。。" },
			});
			_dialog[1] = new MessageDialog({
				{ Talker::SELF, L"私は一体。。。？" },
				{ Talker::SPIRIT, L"やっと起きたね" },
				{ Talker::SELF, L"ここは。。。？" },
				{ Talker::SPIRIT, L"ここは次元迷宮ですよ" },
				{ Talker::SELF, L"次元。。。迷宮。。。？" },
				{ Talker::SPIRIT, L"どうやら何らかのトラブルでキミをここに迷い込んでしまったね" },
				{ Talker::SELF, L"あなたは誰ですが？" },
				{ Talker::SPIRIT, L"私はこの迷宮の守護精霊、ジーゲンです" },
				{ Talker::SELF, L"じいさんですね、よろしくお願いします" },
				{ Talker::SPIRIT, L"じいさんで言うな！" },
				{ Talker::SPIRIT, L"とりあえず、ここに脱出するには、三つの水晶が必要なんだ" },
				{ Talker::SPIRIT, L"まず一つ目の赤い水晶を探すのはおすすめです" },
				{ Talker::SELF, L"わかった" },
				{ Talker::SPIRIT, L"困った時はいつでも相談に乗りますよ" },
			});
		}
		~StartEvent() {
			delete _dialog[0];
			delete _dialog[1];
		}
		void update() override {
			_gameEvent->updatePlayerAct();
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
					break;
			}
			
		}
		void draw() override {
			switch (_eventPhase) {
				case 0:
					SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, _coverColor);
					_dialog[0]->draw();
					break;
				case 1:
					_dialog[1]->draw();
					if (_dialog[1]->isEnd()) {
						PLAYER.setToEventState(false);
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