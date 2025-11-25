#pragma once

#include "DirectX.h"
#include "DirectWrite.h"
#include "Texture.h"
#include "Sprite.h"
#include "Keyboard.h"
#include "MathTool.h"
#include <string>
#include <sstream>
#include <vector>
#include <functional>

class IDialog {
	public:
		IDialog() {
			tex = TEXTURE.loadTexture("./assets/messageBox.png");
		}
		virtual void update() = 0;
		virtual void draw() = 0;
		virtual void draw(Float3 pos) {}
		void drawMessageBox() {
			SPRITE.drawSprite2D(_pos, _size, tex, 0.8f);
		}
		void drawStr(Float2 pos, std::string str) {
			DWRITE.drawString(str, {
				pos.x + 640.0f - _size.x * 0.5f + _margin.x,
				360.0f - pos.y - _size.y * 0.5f + _margin.y
			});
		}
		bool isEnd() {
			return _isEnd;
		}

	protected:
		Float2 _pos;
		Float2 _size;
		Float2 _margin = { 50.0f, 40.0f };
		std::vector<std::string> _context;
		int contextIdx = 0;
		int currentContextNum = 0;
		int currentContextIdx = 0;
		int openDialogAnimCount = 0;
		int contextSpeed = 3;
		int frameCount = 0;
		bool _isEnd = false;

	private:
		unsigned int tex;
};

class MessageDialog : public IDialog {
	public:
		MessageDialog(std::vector<std::string> context) {
			_pos = { 0.0f, 100.0f };
			_size = { 800.0f, 200.0f };
			_context = context;
			currentContextNum = _context[0].length();
		}
		void update() override {
			if (openDialogAnimCount < 10) {
				_size.x = MathTool::lerp<float>(0.0f, 800.0f, openDialogAnimCount / 10.0f);
				openDialogAnimCount++;
				return;
			}
			if (currentContextIdx == currentContextNum / 2 && Keyboard_IsKeyTrigger(KK_ENTER)) {
				contextIdx += 1;
				if (contextIdx < _context.size()) {
					currentContextNum = _context[contextIdx].length();
					currentContextIdx = 0;
				}
				if (contextIdx == _context.size()) {
					_isEnd = true;
				}
			}
			if (frameCount % 2 == 0) {
				currentContextIdx = std::min(currentContextNum / 2, currentContextIdx + 1);
			}
			frameCount++;
		}
		void draw() override {
			if (_isEnd) return;
			std::string str = _context[contextIdx].substr(0, currentContextIdx * 2);
			IDialog::drawMessageBox();
			IDialog::drawStr(_pos, str);
		}
};

class GetItemDialog : public IDialog {
	public:
		GetItemDialog(std::vector<std::string> context) {
			_pos = { 0.0f, 200.0f };
			_size = { 300.0f, 80.0f };
			_context = context;
			currentContextNum = _context[0].length();
		}
		void update() override {
			if (openDialogAnimCount < 10) {
				_size.x = MathTool::lerp<float>(0.0f, 500.0f, openDialogAnimCount / 10.0f);
				openDialogAnimCount++;
				return;
			}
			if (frameCount % 2 == 0) {
				currentContextIdx = std::min(currentContextNum, currentContextIdx + 1);
			}
			frameCount++;
		}
		void draw() override {
			std::string str = _context[contextIdx].substr(0, currentContextIdx * 2);
			IDialog::drawMessageBox();
			IDialog::drawStr(_pos, str);
		}
};

class HintDialog : public IDialog {
	public:
		HintDialog() {
			tex = TEXTURE.loadTexture("./assets/buttonA.png");
		}
		void update() override {
		}
		void draw() override {
		}
		void draw(Float3 pos) override {
			SPRITE.drawSpriteIn3D(pos, { 32.0f, 32.0f }, tex);
		}
	
	private:
		unsigned int tex;
};

class ConfirmDialog : public IDialog {
	public:
		ConfirmDialog(std::vector<std::string> context, std::function<void()> callback) : _callback(callback) {
			_pos = { 0.0f, 0.0f };
			_size = { 500.0f, 150.0f };
			_context = context;
			currentContextNum = _context[0].length();
		}
		void update() override {
			if (openDialogAnimCount < 10) {
				_size.x = MathTool::lerp<float>(0.0f, 500.0f, openDialogAnimCount / 10.0f);
				openDialogAnimCount++;
				return;
			}
			if (Keyboard_IsKeyTrigger(KK_RIGHT)) {
				_isConfirm = false;
			}
			if (Keyboard_IsKeyTrigger(KK_LEFT)) {
				_isConfirm = true;
			}
			if (Keyboard_IsKeyTrigger(KK_ENTER)) {
				if (_isConfirm) {
					_callback();
				}
				_isEnd = true;
			}
			if (frameCount % 2 == 0) {
				currentContextIdx = std::min(currentContextNum / 2, currentContextIdx + 1);
			}
			frameCount++;
		}
		void draw() override {
			IDialog::drawMessageBox();

			std::string str = _context[contextIdx].substr(0, currentContextIdx * 2);
			IDialog::drawStr(_pos, str);

			IDialog::drawStr({ _pos.x + 50.0f, _pos.y - 80.0f }, "はい");
			IDialog::drawStr({ _pos.x + 200.0f, _pos.y - 80.0f }, "いいえ");
			if (_isConfirm) {
				IDialog::drawStr({ _pos.x + 20.0f, _pos.y - 80.0f }, "＞");
			}
			else {
				IDialog::drawStr({ _pos.x + 170.0f, _pos.y - 80.0f }, "＞");
			}
		}

	private:
		bool _isConfirm = true;
		std::function<void()> _callback;
};