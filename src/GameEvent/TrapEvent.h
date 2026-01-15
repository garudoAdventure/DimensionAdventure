#pragma once

#include "IGameEvent.h"
#include "./Game/IGameEventHandler.h"
#include "./Dialog/MessageDialog.h"
#include "./Player/Player.h"
#include "./Common/Color.h"

class TrapEventFH : public IGameEvent {
	public:
		TrapEventFH(IGameEventHandler* gameEvent, unsigned int earthquakeSE) : _gameEvent(gameEvent), _earthquakeSE(earthquakeSE) {
			_dialog[0] = new MessageDialog({
				{ Talker::SPIRIT, L"やったな！" },
				{ Talker::SPIRIT, L"これで残りの水晶は二つだ。" },
				{ Talker::SPIRIT, L"実はこの水晶には、不思議な力が......" },
			});
			_dialog[1] = new MessageDialog({
				{ Talker::SELF, L"何が起きた！？" },
				{ Talker::SPIRIT, L"これは......罠だ！" },
				{ Talker::SELF, L"罠！？" },
				{ Talker::SPIRIT, L"まさか、ここで仕掛けてくるとは......" },
				{ Talker::SPIRIT, L"飛ばされる！私の近くにいろ！" },
				{ Talker::SELF, L"ううわわわわわわわわわわわ！！" },
			});
		}
		~TrapEventFH() {
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
						if (_startVibrationCount == 0) {
							SOUND.stopSound(_gameEvent->getBgmId());
							SOUND.playSound(_earthquakeSE, -1);
						}
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
		unsigned int _earthquakeSE;
		int _startVibrationCount = 0;
		int _eventPhase = 0;
		int _startCount = 0;
};

class TrapEventSH : public IGameEvent {
	public:
		TrapEventSH(IGameEventHandler* gameEvent) : _gameEvent(gameEvent) {
			_dialog = new MessageDialog({
				{ Talker::SELF, L"ううう......" },
				{ Talker::SPIRIT, L"大丈夫か？" },
				{ Talker::SELF, L"ここは.......？" },
				{ Talker::SPIRIT, L"どうやら、レッドレイヤーに飛ばされてしまったようだ。" },
				{ Talker::SELF, L"レッドレイヤー......？" },
				{ Talker::SPIRIT, L"この次元迷宮には、四つのレイヤーが存在している。" },
				{ Talker::SPIRIT, L"私たちが最初にいたのは、ホワイトレイヤーだ。" },
				{ Talker::SPIRIT, L"あそこは、唯一外の世界と繋がっているレイヤーだ。" },
				{ Talker::SELF, L"どうすれば戻れるんですか？" },
				{ Talker::SPIRIT, L"先ほど手に入れた赤い水晶......あれは次元水晶と呼ばれている。"},
				{ Talker::SPIRIT, L"レイヤーの間を自由に移動する力を持っているんだ。"},
				{ Talker::SPIRIT, L"三つの水晶をすべて集めれば、元のレイヤーに戻れるはずだ。" },
				{ Talker::SELF, L"わかりました。とりあえず探しに行きましょう。" },
			});
		}
		~TrapEventSH() {
			delete _dialog;
		}
		void update() override {
			PLAYER.update();
			_gameEvent->updateField();
			if (startCount < 120) {
				startCount++;
				return;
			}
			_dialog->update();
			if (_dialog->isEnd()) {
				SOUND.playSound(_gameEvent->getBgmId(), -1);
			}
		}
		void draw() override {
			_dialog->draw();

			Float4 coverColor = Color::black;
			coverColor.a = 1.0f - startCount / 120.0f;
			SHADER.setPS(PS::NO_TEX);
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