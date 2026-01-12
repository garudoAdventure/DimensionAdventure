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
			_model->updateColor({_ambientColor.x, _ambientColor.y, _ambientColor.z, _ambientColor.w });
		}
		void draw() override {
			Light light;
			light.enable = true;
			light.ambient = _ambientColor;
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
		Model* _model;
		XMFLOAT4 _ambientColor;

	private:
		float rotate = 0.0f;
};

class RedCrystal : public Crystal {
	public:
		RedCrystal(Float3 pos, IGameEventHandler* gameEvent) : Crystal(pos, gameEvent) {
			_ambientColor = { 0.4f, 0.2f, 0.3f, 1.0f };
		}
		void getItem() override {
			PLAYER.setState(new PlayerFreeze());
			PLAYER.changeState();
			unsigned int earthquakeSE = SOUND.loadSound("./assets/sound/earthquake.wav");
			_gameEvent->addEvent(new GetItemEvent(_gameEvent, this, [=]() {
				PLAYER.addCrystalNum();
				if (!PLAYER.is2D()) {
					PLAYER.convertDimension();
				}
				_gameEvent->addEvent(new TrapEventFH(_gameEvent, earthquakeSE));
				_gameEvent->addEvent(new LayerTrapEvent(_gameEvent, earthquakeSE));
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
			_ambientColor = { 0.2f, 0.3f, 0.2f, 1.0f };
		}
		void getItem() override {
			PLAYER.setState(new PlayerFreeze());
			PLAYER.changeState();
			_gameEvent->addEvent(new GetItemEvent(_gameEvent, this, [=]() {
				PLAYER.addCrystalNum();
				if (!PLAYER.is2D()) {
					PLAYER.convertDimension();
				}
				_gameEvent->addEvent(new ShowDialogEvent(
					new MessageDialog({
						{ Talker::SPIRIT, L"これで二つ目の水晶も手にいれだな。" },
						{ Talker::SPIRIT, L"今なら、別のレイヤーへ切り替えられるはずだ。" },
					})
				));
				_gameEvent->addEvent(new ShowDialogEvent(new LayerTutorialDialog()));
				_gameEvent->addEvent(new ShowDialogEvent(
					new MessageDialog({
						{ Talker::SPIRIT, L"それぞれのレイヤー空間は、大体同じだが、実は微妙の違いがあるんだ。" },
						{ Talker::SPIRIT, L"今のレイヤーでは行けない場所は、他のレイヤーなら行けるかもしれない。" },
						{ Talker::SPIRIT, L"困った時は、他のレイヤーを探索してみるといい。" },
						{ Talker::SELF, L"わかりました。最後の水晶を探しに行きましょう。" }
					})
				));
				_gameEvent->setCheckpoint(CheckPoint::GREEN_CRYSTAL);
				PLAYER.setState(new PlayerIdle());
			}));
		}
		std::wstring getName() override {
			return L"次元水晶（緑）をゲット！";
		}
};

class BlueCrystal : public Crystal {
	public:
		BlueCrystal(Float3 pos, IGameEventHandler* gameEvent) : Crystal(pos, gameEvent) {
			_ambientColor = { 0.2f, 0.2f, 0.5f, 1.0f };
		}
		void getItem() override {
			PLAYER.setState(new PlayerFreeze());
			PLAYER.changeState();
			_gameEvent->addEvent(new GetItemEvent(_gameEvent, this, [=]() {
				PLAYER.addCrystalNum();
				_gameEvent->addEvent(new ShowDialogEvent(
					new MessageDialog({
						{ Talker::SPIRIT, L"三つの水晶を全部揃ったな。" },
						{ Talker::SELF, L"じゃあ、どうやってホワイトレイヤーに戻れるんですか？" },
						{ Talker::SPIRIT, L"......" },
						{ Talker::SELF, L"じいさん？" },
						{ Talker::SPIRIT, L"じいさん言うな！" },
						{ Talker::SPIRIT, L"......" },
						{ Talker::SPIRIT, L"確か、特定の状態で特定の行動をすると戻れるはずだ。" },
						{ Talker::SELF, L"特定な状態で、特定な行動？" },
						{ Talker::SPIRIT, L"......" },
						{ Talker::SPIRIT, L"すまん、忘れてた。えへ(・ω<)" },
						{ Talker::SELF, L"......" },
						{ Talker::SPIRIT, L"で......でも、手がかりはきっとどこかにあるはずだ！" },
						{ Talker::SPIRIT, L"三つの水晶を揃えた君なら、見つけられるはずだ！" },
						{ Talker::SPIRIT, L"ゴーゴー！" },
						{ Talker::SELF, L"はぁ......わかりましたよ。" },
					})
				));
			}));
			_gameEvent->setCheckpoint(CheckPoint::BLUE_CRYSTAL);
			PLAYER.setState(new PlayerIdle());
		}
		std::wstring getName() override {
			return L"次元水晶（青）をゲット！";
		}
};