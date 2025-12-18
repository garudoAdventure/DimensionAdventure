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
			_context = context;
			_currentContextNum = _context[0].length();
		}
		void update() override {
			if (_openDialogAnimCount <= 10) {
				_size.x = MathTool::lerp<float>(0.0f, 500.0f, _openDialogAnimCount * 0.1f);
				_openDialogAnimCount++;
				return;
			}
			if (_frameCount % 2 == 0) {
				_currentContextIdx = std::min(_currentContextNum, _currentContextIdx + 1);
			}
			if (_currentContextIdx == _currentContextNum && Keyboard_IsKeyTrigger(KK_ENTER)) {
				_isEnd = true;
			}
			_frameCount++;
		}
		void draw() override {
			std::wstring wstr = _context.at(_contextIdx).substr(0, _currentContextIdx);
			IDialog::drawMessageBox();
			IDialog::drawStr(wstr, _pos);
		}

	private:
		std::vector<std::wstring> _context;
		int _contextIdx = 0;
		int _currentContextNum;
		int _currentContextIdx = 0;
		int _openDialogAnimCount = 0;
		int _frameCount = 0;
};