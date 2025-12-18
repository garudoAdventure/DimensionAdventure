#pragma once

#include "MathStruct.h"
#include "Particle.h"
#include "MathTool.h"
#include "Texture.h"
#include "IGameEventHandler.h"
#include "ShowDialogEvent.h"
#include "MessageDialog.h"

class Spirit {
	public:
		Spirit(IGameEventHandler* gameEvent) : _gameEvent(gameEvent) {
			_emitter = new Emitter();
			_buttonTex = TEXTURE.loadTexture("./assets/keyX.png");
		}
		~Spirit() {
			delete _emitter;
		}
		void update() {
			_pos = MathTool::lerp<Float3>(_pos, _newPos, 0.05f);
			_pos.y += sinf(_time * 0.05f) * 0.01f;

			_isStatic = abs(_pos.x - _newPos.x) < 0.01f && abs(_pos.z - _newPos.z) < 0.01f;
			if (_isStatic) {
				chat();
			}

			_emitter->update(_pos);
			
			_time = (_time + 1) % 1000;
		}
		void updatePos(Float3 playerPos, Float3 playerDir) {
			_newPos = {
				playerPos.x - 3.0f * playerDir.x,
				playerPos.y + 2.0f,
				playerPos.z
			};
		}
		void setPos(Float3 playerPos) {
			_pos = {
				playerPos.x - 3.0f,
				playerPos.y + 2.0f,
				playerPos.z
			};
		}
		void draw() {
			_emitter->draw(_pos);
		}
		void chat() {
			if (Keyboard_IsKeyTrigger(KK_X)) {
				_gameEvent->addEvent(new ShowDialogEvent(
					new MessageDialog({
						{ Talker::SPIRIT, getSpiritHint() }
						})
				));
			}
		}
		void drawHint() {
			if (_isStatic) {
				XMMATRIX world = XMMatrixIdentity();
				world *= SHADER.getInverseView();
				world *= XMMatrixTranslation(_pos.x, _pos.y + 2.0f, _pos.z);
				SHADER.setWorldMatrix(world);
				SPRITE.drawSpriteIn3D({ 28.0f, 28.0f }, _buttonTex);
			}
		}
		void introHintBlock() {
			_gameEvent->addEvent(new ShowDialogEvent(
				new MessageDialog({
					{ Talker::SPIRIT, L"これはヒントの壁です"},
					{ Talker::SPIRIT, L"この迷宮内で謎解きをする時、必要な情報はこの壁に現れるらしいです"}
				})
			));
		}

	private:
		IGameEventHandler* _gameEvent;
		Float3 _pos = { 0.0f, 0.0f, 0.0f };
		Float3 _newPos = { 0.0f, 0.0f, 0.0f };
		Emitter* _emitter;
		int _time = 0;
		bool _isStatic = true;
		unsigned int _buttonTex;

		std::wstring getSpiritHint() {
			switch (_gameEvent->getCheckpoint()) {
				case CheckPoint::JUMP:
					return L"ジャンプはＺキーだよ";
				case CheckPoint::FIND_FIRST_CRYSTAL:
					return L"今は赤い水晶を探して行こう";
				case CheckPoint::RED_CRYSTAL:
					return L"今は行ける場所を探していこう";
				case CheckPoint::GREEN_HINT:
					return L"このままでは解けないらしいです、ヒントの壁を見て行きましょう";
				case CheckPoint::GREEN_CRYSTAL:
					return L"最後の水晶は上方にあるの気がする";
				case CheckPoint::BLUE_HINT:
					return L"困ったら、ヒントの壁を見て行きましょう";
				case CheckPoint::BLUE_CRYSTAL:
					return L"ホワイトレイヤーに戻る方法の手がかりはきっとどこにあるはず！";
				case CheckPoint::WHITE_DOOR:
					return L"パスワードの手がかりはこの空間のどこかで見た気がします。。。";
			}
		}
};