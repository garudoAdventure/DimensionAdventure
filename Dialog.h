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

class IDialog {
	public:
		virtual void update() = 0;
		virtual void draw() = 0;
		virtual void draw(Float3 pos) {}
		void drawStr(std::string str) {
			SPRITE.drawSprite2D(_pos, _size, { 0.7f, 0.7f, 0.7f, 0.9f });
			DWRITE.drawString(str, {
				_pos.x + 640.0f - _size.x * 0.5f + _margin.x,
				360.0f - _pos.y - _size.y * 0.5f + _margin.y
			});
		}
		bool isEnd() {
			return _isEnd;
		}

	protected:
		Float2 _pos;
		Float2 _size;
		Float2 _margin = { 20.0f, 10.0f };
		std::vector<std::string> _context;
		int contextIdx = 0;
		int currentContextNum = 0;
		int currentContextIdx = 0;
		int openDialogAnimCount = 0;
		int contextSpeed = 3;
		int frameCount = 0;
		bool _isEnd = false;
};

class NormalDialog : public IDialog {
	public:
		NormalDialog(std::vector<std::string> context) {
			_pos = { 0.0f, 0.0f };
			_size = { 500.0f, 150.0f };
			_context = context;
			currentContextNum = _context[0].length();
		}
		void update() override {
			if (openDialogAnimCount < 10) {
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
			if (openDialogAnimCount < 10) {
				_size.x = MathTool::lerp<float>(0.0f, 500.0f, openDialogAnimCount / 10.0f);
				SPRITE.drawSprite2D(_pos, _size, { 0.7f, 0.7f, 0.7f, 0.9f });
				return;
			}
			std::string str = _context[contextIdx].substr(0, currentContextIdx * 2);
			IDialog::drawStr(str);
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
				openDialogAnimCount++;
				return;
			}
			if (frameCount % 2 == 0) {
				currentContextIdx = std::min(currentContextNum, currentContextIdx + 1);
			}
			frameCount++;
		}
		void draw() override {
			if (openDialogAnimCount < 10) {
				_size.x = MathTool::lerp<float>(0.0f, 500.0f, openDialogAnimCount / 10.0f);
				SPRITE.drawSprite2D(_pos, _size, { 0.7f, 0.7f, 0.7f, 0.9f });
				return;
			}
			std::string str = _context[contextIdx].substr(0, currentContextIdx * 2);
			IDialog::drawStr(str);
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