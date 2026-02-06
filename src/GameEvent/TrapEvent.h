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
			if (!_isStartEarthquake) {
				_dialog[0]->update();
				if (_dialog[0]->isEnd()) {
					if (_startVibrationCount == 0) {
						SOUND.stopSound(_gameEvent->getBgmId());
						SOUND.playSound(_earthquakeSE, -1);
					}
					_gameEvent->cameraVibration(true);
					_startVibrationCount++;
					if (_startVibrationCount > 120) {
						_isStartEarthquake = true;
					}
				}
			}
			else {
				_gameEvent->cameraVibration(true);
				_dialog[1]->update();
			}
		}

		void draw() override {
			if (!_isStartEarthquake) {
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
		IDialog* _dialog[2];
		IGameEventHandler* _gameEvent;
		unsigned int _earthquakeSE;
		int _startVibrationCount = 0;
		bool _isStartEarthquake = false;
};

class TrapEventSH : public IGameEvent {
	public:
		static constexpr int FADE_IN_TIME = 120;

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
			_fadeCover = new FadeCover(FADE_IN_TIME);
		}

		~TrapEventSH() {
			delete _dialog;
			delete _fadeCover;
		}

		void update() override {
			PLAYER.update();
			_gameEvent->updateField();
			if (_fadeCover->fadeIn()) {
				_dialog->update();
			}
			if (_dialog->isEnd()) {
				SOUND.playSound(_gameEvent->getBgmId(), -1);
			}
		}

		void draw() override {
			_dialog->draw();
			_fadeCover->draw();
		}

		bool isEnd() override {
			return _dialog->isEnd();
		}

	private:
		IDialog* _dialog;
		IGameEventHandler* _gameEvent;
		FadeCover* _fadeCover;
};