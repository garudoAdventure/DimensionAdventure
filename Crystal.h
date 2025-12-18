#pragma once

#include "Item.h"
#include "GetItemEvent.h"
#include "Player.h"
#include "TrapEvent.h"
#include "LayerTrapEvent.h"
#include "GreenCrystalEvent.h"
#include "LayerTutorialDialog.h"
#include "MessageDialog.h"
#include "ModelManager.h"
#include "Color.h"

class Crystal : public Item {
	public:
		Crystal(Float3 pos, IGameEventHandler* gameEvent) : Item(pos, gameEvent) {
			_model = MODEL.loadModel("./assets/model/crystal.fbx");
		}
		void update() override {
			Item::update();
			rotate += 0.01f;
		}
		void draw() override {
			Light light;
			light.enable = true;
			light.ambient = ambientColor;
			XMVECTOR direction = { 0.5f, -1.0f, 0.5f };
			direction = XMVector3Normalize(direction);
			XMStoreFloat3(&light.direction, direction);
			SHADER.setLight(light);

			_model->draw(_pos, { 0.0f, rotate, 0.0f });
		}
		ItemTag getTag() override {
			return ItemTag::CRYSTAL;
		}

	protected:
		XMFLOAT4 ambientColor;

	private:
		float rotate = 0.0f;
};

class RedCrystal : public Crystal {
	public:
		RedCrystal(Float3 pos, IGameEventHandler* gameEvent) : Crystal(pos, gameEvent) {
			_model->updateColor({ 1.0f, 0.43f, 0.54f, 1.0f });
			ambientColor = { 0.8f, 0.4f, 0.4f, 1.0f };
		}
		void getItem() override {
			_gameEvent->addEvent(new GetItemEvent(this, [=]() {
				PLAYER.addCrystalNum();
				if (!PLAYER.is2D()) {
					PLAYER.convertDimension();
				}
				_gameEvent->addEvent(new TrapEventFH(_gameEvent));
				_gameEvent->addEvent(new LayerTrapEvent(_gameEvent));
				_gameEvent->addEvent(new TrapEventSH(_gameEvent));
				_gameEvent->setCheckpoint(CheckPoint::RED_CRYSTAL);
			}));
		}
		std::wstring getName() override {
			return L"次元水晶（赤）をゲット！";
		}
};

class GreenCrystal : public Crystal {
	public:
		GreenCrystal(Float3 pos, IGameEventHandler* gameEvent) : Crystal(pos, gameEvent) {
			_model->updateColor(Color::lightGreen);
			ambientColor = { 0.4f, 0.8f, 0.4f, 1.0f };
		}
		void getItem() override {
			_gameEvent->addEvent(new GetItemEvent(this, [=]() {
				PLAYER.addCrystalNum();
				if (!PLAYER.is2D()) {
					PLAYER.convertDimension();
				}
				_gameEvent->addEvent(new GreenCrystalEvent());
				_gameEvent->addEvent(new ShowDialogEvent(new LayerTutorialDialog()));
				_gameEvent->addEvent(new ShowDialogEvent(
					new MessageDialog({
						{ Talker::SPIRIT, L"それぞれのレイヤー空間は、大体同じだけど、実は微妙の差があるんです" },
						{ Talker::SPIRIT, L"このレイヤーで行けない場所は、他のレイヤーで行けるかもしれません" },
						{ Talker::SPIRIT, L"困った時は他のレイヤーに探索してもいい" },
						{ Talker::SELF, L"わかった、最後の水晶を探して行こう" }
					})
				));
				_gameEvent->setCheckpoint(CheckPoint::GREEN_CRYSTAL);
			}));
		}
		std::wstring getName() override {
			return L"次元水晶（緑）をゲット！";
		}
};

class BlueCrystal : public Crystal {
	public:
		BlueCrystal(Float3 pos, IGameEventHandler* gameEvent) : Crystal(pos, gameEvent) {
			_model->updateColor(Color::lightBlue);
			ambientColor = { 0.4f, 0.4f, 0.8f, 1.0f };
		}
		void getItem() override {
			_gameEvent->addEvent(new GetItemEvent(this, [=]() {
				PLAYER.addCrystalNum();
				_gameEvent->addEvent(new ShowDialogEvent(
					new MessageDialog({
						{ Talker::SPIRIT, L"三つの水晶を全部揃ったね" },
						{ Talker::SELF, L"ではどうやってホワイトレイヤーに戻れる？" },
						{ Talker::SPIRIT, L"。。。。。。" },
						{ Talker::SELF, L"じいさん？" },
						{ Talker::SPIRIT, L"じいさん言うな！" },
						{ Talker::SPIRIT, L"。。。。。。" },
						{ Talker::SPIRIT, L"確かに特定な状態で特定な行動をすると戻れる" },
						{ Talker::SELF, L"特定な状態で特定な行動？" },
						{ Talker::SPIRIT, L"。。。。。。" },
						{ Talker::SPIRIT, L"ごめん、忘れてた、えへ(・ω<)" },
						{ Talker::SELF, L"。。。。。。" },
						{ Talker::SPIRIT, L"で。。。でも、手がかりはきっとどこかにあるはず！" },
						{ Talker::SPIRIT, L"三つの水晶を揃えたキミなら見つけるはずです！" },
						{ Talker::SPIRIT, L"ゴーゴー！" },
						{ Talker::SELF, L"はー、わかったよ" },
					})
				));
			}));
			_gameEvent->setCheckpoint(CheckPoint::BLUE_CRYSTAL);
		}
		std::wstring getName() override {
			return L"次元水晶（青）をゲット！";
		}
};