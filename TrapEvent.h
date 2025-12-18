#pragma once

#include "IGameEventHandler.h"
#include "IGameEvent.h"
#include "MessageDialog.h"
#include "Color.h"
#include "Player.h"

class TrapEventFH : public IGameEvent {
	public:
		TrapEventFH(IGameEventHandler* gameEvent) : _gameEvent(gameEvent) {
			_dialog[0] = new MessageDialog({
				{ Talker::SPIRIT, L"やったね！" },
				{ Talker::SPIRIT, L"これで残りの水晶は二コです。" },
				{ Talker::SPIRIT, L"あと実はね、この水晶は不思議な力が..." },
			});
			_dialog[1] = new MessageDialog({
				{ Talker::SELF, L"何があった？！" },
				{ Talker::SPIRIT, L"これは。。。罠です！" },
				{ Talker::SELF, L"罠？！" },
				{ Talker::SPIRIT, L"まさかここで仕掛けるなんで。。。" },
				{ Talker::SPIRIT, L"飛ばされる！私の近くに居て！" },
				{ Talker::SELF, L"ううわわわわわわわわわわわ" },
			});
		}
		~TrapEventFH() {
			delete _dialog[0];
			delete _dialog[1];
		}
		void update() override {
			PLAYER.setToEventState(true);
			_gameEvent->updatePlayerAct();
			switch (_eventPhase) {
				case 0:
					_dialog[0]->update();
					if (_dialog[0]->isEnd()) {
						_gameEvent->cameraVibration(true);
						_startVibrationCount++;
						if (_startVibrationCount > 120) {
							_eventPhase = 1;
						}
					}
					break;
				
				case 1:
					_gameEvent->cameraVibration(true);
					_dialog[1]->update();
					break;
			}
		}
		void draw() override {
			_dialog[_eventPhase]->draw();
		}
		bool isEnd() override {
			return _dialog[1]->isEnd();
		}

	private:
		IDialog* _dialog[2];
		IGameEventHandler* _gameEvent;
		int _startVibrationCount = 0;
		int _eventPhase = 0;
		int _startCount = 0;
};

class TrapEventSH : public IGameEvent {
	public:
		TrapEventSH(IGameEventHandler* gameEvent) : _gameEvent(gameEvent) {
			_dialog = new MessageDialog({
				{ Talker::SELF, L"うう。。。" },
				{ Talker::SPIRIT, L"大丈夫ですか？" },
				{ Talker::SELF, L"ここは。。。？" },
				{ Talker::SPIRIT, L"どうやらレッドレイヤーに飛ばされてしまったね" },
				{ Talker::SELF, L"レッドレイヤー。。。？" },
				{ Talker::SPIRIT, L"この次元迷宮は四つのレイヤーが存在しているです" },
				{ Talker::SPIRIT, L"私たち最初に居たのはホワイトレイヤー" },
				{ Talker::SPIRIT, L"あそこは唯一外と繋がるレイヤーです" },
				{ Talker::SELF, L"どうすれば戻れるですが？" },
				{ Talker::SPIRIT, L"先手に入れた赤い水晶" },
				{ Talker::SPIRIT, L"あれは次元水晶と呼ばれ、レイヤーの間に自由に移動する力が持っている" },
				{ Talker::SPIRIT, L"三つの水晶を全部集めれば、元のレイヤーに戻れるはずです" },
				{ Talker::SELF, L"分かった、とりあえず探しに行きましょう" },
			});
		}
		~TrapEventSH() {
			delete _dialog;
		}
		void update() override {
			_gameEvent->updatePlayerAct();
			if (startCount < 120) {
				startCount++;
				return;
			}
			_dialog->update();
			if (_dialog->isEnd()) {
				PLAYER.setToEventState(false);
			}
		}
		void draw() override {
			_dialog->draw();

			Float4 coverColor = Color::black;
			coverColor.a = 1.0f - startCount / 120.0f;
			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, coverColor);
		}
		bool isEnd() override {
			return _dialog->isEnd();
		}

	private:
		IDialog* _dialog;
		IGameEventHandler* _gameEvent;
		int startCount = 0;
};