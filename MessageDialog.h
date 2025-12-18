#pragma once

#include "Dialog.h"
#include "Keyboard.h"
#include "MathTool.h"
#include <vector>
#include <string>

enum class Talker {
	SELF,
	SPIRIT
};

struct DialogContext {
	Talker talker;
	std::wstring content;
};

class MessageDialog : public IDialog {
	public:
		MessageDialog(std::vector<DialogContext> context) {
			_pos = { 0.0f, 100.0f };
			_size = { 0.0f, 200.0f };
			_context = context;
			_currentContextNum = _context[0].content.length();
			_playerIconTex = TEXTURE.loadTexture("./assets/UI/playerIcon.png");
			_spiritIconTex = TEXTURE.loadTexture("./assets/UI/spiritIcon.png");
		}
		void update() override {
			if (_isEnd) return;
			if (_openDialogAnimCount <= 10) {
				_size.x = MathTool::lerp<float>(0.0f, 800.0f, _openDialogAnimCount / 10.0f);
				_openDialogAnimCount++;
				return;
			}
			if (_currentContextIdx == _currentContextNum && Keyboard_IsKeyTrigger(KK_ENTER)) {
				_contextIdx += 1;
				if (_contextIdx < _context.size()) {
					_currentContextNum = _context[_contextIdx].content.length();
					_currentContextIdx = 0;
				}
				if (_contextIdx == _context.size()) {
					_isEnd = true;
				}
			}
			if (_frameCount % 2 == 0) {
				_currentContextIdx = std::min(_currentContextNum, _currentContextIdx + 1);
			}
			_frameCount++;
		}
		void draw() override {
			if (_isEnd) return;
			std::wstring wstr = _context.at(_contextIdx).content.substr(0, _currentContextIdx);
			IDialog::drawMessageBox();
			int iconTex = -1;
			switch (_context.at(_contextIdx).talker) {
				case Talker::SELF:
					iconTex = _playerIconTex;
					break;
				case Talker::SPIRIT:
					iconTex = _spiritIconTex;
					break;
			}
			if (_openDialogAnimCount > 10) {
				SPRITE.drawSprite2D(
					{ _pos.x - _size.x / 2 + 20.0f, _pos.y - _size.y / 2 + 20.0f },
					{ 102.0f, 102.0f }, iconTex
				);
			}
			IDialog::drawStrInRect(wstr, _pos, _margin);
		}

	private:
		unsigned int _playerIconTex;
		unsigned int _spiritIconTex;
		std::vector<DialogContext> _context;
		int _contextIdx = 0;
		int _currentContextNum;
		int _currentContextIdx = 0;
		int _openDialogAnimCount = 0;
		int _frameCount = 0;
};