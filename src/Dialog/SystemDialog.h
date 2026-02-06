#pragma once

#include "Dialog.h"
#include "HALKeyboard.h"
#include "./Utils/MathTool.h"
#include <vector>
#include <string>

class SystemDialog : public IDialog {
	public:
		SystemDialog(std::vector<std::wstring> contentList) : _contentList(contentList) {
			_pos = { 0.0f, 200.0f };
			_size.y = 120.0f;
			_dialogWidth = 500.0f;
			_currentContentStrLen = contentList.at(0).length();
		}
		void dialogUpdate() override {
			if (_frameCount % 2 == 0) {
				_contentIdx = std::min(_currentContentStrLen, _contentIdx + 1);
			}
			if (_contentIdx == _currentContentStrLen && Keyboard_IsKeyTrigger(KK_ENTER)) {
				_isEnd = true;
			}
			_frameCount++;
		}
		void dialogDraw() override {
			std::wstring wstr = _contentList.at(_contentListIdx).substr(0, _contentIdx);
			IDialog::drawStr(wstr, _pos);
		}

	private:
		std::vector<std::wstring> _contentList;
		int _frameCount = 0;
};