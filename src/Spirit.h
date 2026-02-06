#pragma once

#include "Particle.h"
#include "./Game/IGameEventHandler.h"
#include "./GameEvent/ShowDialogEvent.h"
#include "./Dialog/MessageDialog.h"
#include "./Render/Texture.h"
#include "./Common/MathStruct.h"
#include "./Utils/MathTool.h"

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
				SHADER.setWorld(world);
				SHADER.setMatrix();
				SPRITE.drawSprite3D({ 28.0f, 28.0f }, TEXTURE.getTexture(_buttonTex), Color::white);
			}
		}

		void introHintBlock() {
			_gameEvent->addEvent(new ShowDialogEvent(
				new MessageDialog({
					{ Talker::SPIRIT, L"これはヒントの壁だ。"},
					{ Talker::SPIRIT, L"この迷宮で謎を解くための情報が、この壁に示されるらしい。"},
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
					return L"ジャンプはＺキーだよ。";
				case CheckPoint::FIND_FIRST_CRYSTAL:
					return L"まずは赤い水晶を探しに行こう。";
				case CheckPoint::RED_CRYSTAL:
					return L"今なら、行ける場所が増えているはずだ。";
				case CheckPoint::GREEN_HINT:
					return L"このままでは解けなさそうだ。ヒントの壁を見に行こう。";
				case CheckPoint::GREEN_CRYSTAL:
					return L"最後の水晶は、上の方にありそうな気がする。";
				case CheckPoint::BLUE_HINT:
					return L"困った時は、ヒントの壁を確認してみよう。";
				case CheckPoint::BLUE_CRYSTAL:
					return L"ホワイトレイヤーに戻る方法の手がかりは、きっとどこかにあるはずだ！";
				case CheckPoint::WHITE_LAYER:
					return L"石碑に記されたメッセージによれば、部屋の奥に何があるはずだ。";
				case CheckPoint::REMOTE_CONTROL:
					return L"最初の部屋にもどりましょう。";
				case CheckPoint::WHITE_DOOR:
					return L"出口の扉を起動しましょう。";
				case CheckPoint::PASSWORD:
					return L"この部屋のどこかで、パスワードの手がかりを見た気がする......";
				case CheckPoint::FINAL:
					return L"他の場所に行く必要はなさそうだ。";
			}
		}
};