#pragma once

#include "IGameEventHandler.h"
#include "IGameEvent.h"
#include "Dialog.h"
#include "Color.h"
#include "Player.h"

class TrapEvent : public IGameEvent {
	public:
		TrapEvent(IGameEventHandler* gameEvent) : _gameEvent(gameEvent) {
			PLAYER.setToEventState(true);
			_dialog[0] = new MessageDialog({
				{ Talker::SPIRIT, "やったね！" },
				{ Talker::SPIRIT, "これで残りの水晶は二コです。" },
				{ Talker::SPIRIT, "あと実はね、この水晶は不思議な力が..." },
			});
			_dialog[1] = new MessageDialog({
				{ Talker::SELF, "何があった？！" },
				{ Talker::SPIRIT, "これは。。。罠です！" },
				{ Talker::SELF, "罠？！" },
				{ Talker::SPIRIT, "まさかここで仕掛けるなんで。。。" },
				{ Talker::SPIRIT, "飛ばされる！私の近くに居て！" },
				{ Talker::SELF, "ううわわわわわわわわわわわ" },
			});
			_dialog[2] = new MessageDialog({
				{ Talker::SELF, "うう。。。" },
				{ Talker::SPIRIT, "大丈夫ですか？" },
				{ Talker::SELF, "ここは。。。？" },
				{ Talker::SPIRIT, "どうやらレッドレイヤーに飛ばされてしまったね" },
				{ Talker::SELF, "レッドレイヤー。。。？" },
				{ Talker::SPIRIT, "この次元迷宮は四つのレイヤーが存在しているです" },
				{ Talker::SPIRIT, "私たち最初に居たのはホワイトレイヤー" },
				{ Talker::SPIRIT, "あそこは唯一外と繋がるレイヤーです" },
				{ Talker::SELF, "どうすれば戻れるですが？" },
				{ Talker::SPIRIT, "先手に入れた赤い水晶" },
				{ Talker::SPIRIT, "あれは次元水晶と呼ばれ、レイヤーの間に自由に移動する力が持っている" },
				{ Talker::SPIRIT, "三つの水晶を全部集めれば、元のレイヤーに戻れるはずです" },
				{ Talker::SELF, "分かった、とりあえず探しに行きましょう" },
			});
			coverColor.a = 0.0f;
		}
		void update() override {
			_gameEvent->updatePlayerAct();
			switch (eventPhase) {
				case -1:
					startCount++;
					if (startCount == 120) {
						eventPhase = 0;
					}
					break;
				case 0:
					_dialog[0]->update();
					if (_dialog[0]->isEnd()) {
						_gameEvent->setCameraVibration(true);
						startVibrationCount++;
						if (startVibrationCount > 120) {
							eventPhase = 1;
						}
					}
					break;
				
				case 1:
					_gameEvent->setCameraVibration(true);
					_dialog[1]->update();
					if (_dialog[1]->isEnd()) {
						coverCount = std::min(coverCount + 1, 240);
						coverColor.a = MathTool::lerp<float>(0.0f, 1.0f, coverCount / 240.0f);
						if (coverCount == 240) {
							PLAYER.setLayer(0);
							_gameEvent->setCameraVibration(false);
							eventPhase = 2;
						}
					}
					break;

				case 2:
					coverCount = std::max(coverCount - 1, 0);
					coverColor.a = MathTool::lerp<float>(0.0f, 1.0f, coverCount / 240.0f);
					if (coverCount == 0) {
						eventPhase = 3;
					}
					break;

				case 3:
					_dialog[2]->update();
					if (_dialog[2]->isEnd()) {
						PLAYER.setToEventState(false);
					}
					break;
			}
		}
		void draw() override {
			switch (eventPhase) {
				case 0:
					_dialog[0]->draw();
					break;
				case 1:
					_dialog[1]->draw();
					break;
				case 3:
					_dialog[2]->draw();
					break;
			}
			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, coverColor);
		}
		bool isEnd() override {
			return _dialog[2]->isEnd();
		}

	private:
		IDialog* _dialog[3];
		IGameEventHandler* _gameEvent;
		int startVibrationCount = 0;
		Float4 coverColor = Color::white;
		int eventPhase = -1;
		int coverCount = 0;
		int startCount = 0;
};