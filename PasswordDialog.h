#pragma once

#include "Dialog.h"
#include "Keyboard.h"
#include "MathTool.h"
#include "Sprite.h"
#include "Color.h"
#include "IGameEventHandler.h"
#include "ShowDialogEvent.h"
#include "SystemDialog.h"
#include "ExitEvent.h"
#include <string>

class PasswordDialog : public IDialog {
	public:
		PasswordDialog(IGameEventHandler* gameEvent): _gameEvent(gameEvent) {
			_pos = { 0.0f, 100.0f };
			_size = { 0.0f, 300.0f };
			_dialogWidth = 800.0f;
			_currentContentNum = _context.length();
			_numberTex = TEXTURE.loadTexture("./assets/number.png");
			_triangleTex = TEXTURE.loadTexture("./assets/triangle.png");
		}

		void dialogUpdate() override {
			if (_frameCount % 2 == 0) {
				_contentIdx = std::min(_currentContentNum, _contentIdx + 1);
			}
			if (Keyboard_IsKeyTrigger(KK_RIGHT)) {
				_selectDigit = std::min(3, _selectDigit + 1);
			}
			if (Keyboard_IsKeyTrigger(KK_LEFT)) {
				_selectDigit = std::max(0, _selectDigit - 1);
			}
			if (_pushCount == 10) {
				_isPushUp = false;
				_isPushDown = false;
			}
			if (Keyboard_IsKeyTrigger(KK_UP)) {
				_isPushUp = true;
				_pushCount = 0;
				_inputPassword[_selectDigit] = std::min(9, _inputPassword[_selectDigit] + 1);
			}
			if (Keyboard_IsKeyTrigger(KK_DOWN)) {
				_isPushDown = true;
				_pushCount = 0;
				_inputPassword[_selectDigit] = std::max(0, _inputPassword[_selectDigit] - 1);
			}
			if (Keyboard_IsKeyTrigger(KK_ENTER)) {
				_isEnd = true;
				if (
					_inputPassword[0] == 4 &&
					_inputPassword[1] == 7 &&
					_inputPassword[2] == 3 &&
					_inputPassword[3] == 1
				) {
					_gameEvent->addEvent(new ExitEvent(_gameEvent));
					return;
				}
				_gameEvent->addEvent(new ShowDialogEvent(
					new SystemDialog({L"何も反応しない"})
				));
			}
			_time++;
			_frameCount++;
			_pushCount = std::min(10, _pushCount + 1);
		}

		void dialogDraw() override {
			std::wstring wstr = _context.substr(0, _contentIdx);
			IDialog::drawStr(wstr, _pos);
			if (_contentIdx != _currentContentNum) {
				return;
			}
			for (int i = 0; i < 4; i++) {
				const Float2 numberPos = {
					_pos.x - 150.0f + 100.0f * i,
					_pos.y - 20.0f
				};
				if (i == _selectDigit) {
					Float4 selectColor = Color::gray;
					selectColor.a = (sinf(_time * 0.1f) + 1) * 0.5f + 0.2f;
					SHADER.setPS(PS::NO_TEX);
					SPRITE.drawSprite2D(numberPos, { 80.0f, 150.0f }, selectColor);
				}
				SHADER.setPS(PS::NO_TEX);
				SPRITE.drawSprite2D(numberPos, { 40.0f, 60.0f }, Color::black);
				SPRITE.drawSprite2DUV(
					numberPos, { 26.0f, 38.0f }, TEXTURE.getTexture(_numberTex), numberColor[i],
					{ (_inputPassword[i] % 10) * 0.1f, 0.0f }, { 0.1f, 1.0f }
				);
				SPRITE.drawSprite2D({ numberPos.x, numberPos.y + 50.0f }, { 24.0f, 24.0f }, TEXTURE.getTexture(_triangleTex), (_isPushUp && i == _selectDigit ? Color::darkGray : Color::white));
				SPRITE.drawSprite2DRotate({ numberPos.x, numberPos.y - 50.0f }, { 24.0f, 24.0f }, TEXTURE.getTexture(_triangleTex), PI, { numberPos.x, numberPos.y - 50.0f }, (_isPushDown && i == _selectDigit ? Color::darkGray : Color::white));
			}
		}

	private:
		const std::wstring _context{ L"暗証番号を入力してください" };
		IGameEventHandler* _gameEvent;
		unsigned int _numberTex;
		unsigned int _triangleTex;
		int _frameCount = 0;
		int _time = 0;
		bool _isPushUp = false;
		bool _isPushDown = false;
		int _pushCount = 0;
		int _selectDigit = 0;
		int _inputPassword[4] = { 0, 0, 0, 0 };
		Float4 numberColor[4] = {
			Color::lightRed,
			Color::lightGreen,
			Color::lightBlue,
			Color::white
		};
};