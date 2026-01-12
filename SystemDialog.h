#pragma once

#include "Dialog.h"
#include "Keyboard.h"
#include "MathTool.h"
#include <vector>
#include <string>

class SystemDialog : public IDialog {
	public:
		SystemDialog(std::vector<std::wstring> context) {
			_pos = { 0.0f, 200.0f };
			_size = { 0.0f, 120.0f };
			_dialogWidth = 500.0f;
			_context = context;
			_currentContentNum = _context[0].length();
		}
		void dialogUpdate() override {
			if (_frameCount % 2 == 0) {
				_contentIdx = std::min(_currentContentNum, _contentIdx + 1);
			}
			if (_contentIdx == _currentContentNum && Keyboard_IsKeyTrigger(KK_ENTER)) {
				_isEnd = true;
			}
			_frameCount++;
		}
		void dialogDraw() override {
			std::wstring wstr = _context.at(_contextIdx).substr(0, _contentIdx);
			IDialog::drawStr(wstr, _pos);
		}

	private:
		std::vector<std::wstring> _context;
		int _frameCount = 0;
};