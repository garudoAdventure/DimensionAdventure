#pragma once

#include "Dialog.h"
#include "HALKeyboard.h"
#include "./Common/MathStruct.h"

#include <string>
#include <functional>

class ConfirmDialog : public IDialog {
	public:
		ConfirmDialog(std::vector<std::wstring> contentList, std::function<void()> callback) :
			_contentList(contentList), _callback(callback)
		{
			_pos = { 0.0f, 200.0f };
			_size.y = 200.0f;
			_dialogWidth = 500.0f;
			_currentContentStrLen = _contentList.at(0).length();
		}
		void dialogUpdate() override {
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
			if (_frameCount % 2 == 0) {
				_contentIdx = std::min(_currentContentStrLen, _contentIdx + 1);
			}
			_frameCount++;
		}
		void dialogDraw() override {
			std::wstring wstr = _contentList.at(_contentListIdx).substr(0, _contentIdx);
			IDialog::drawStr(wstr, _pos);

			if (_contentIdx != _currentContentStrLen) {
				return;
			}
			IDialog::drawStr(L"はい", { _pos.x + 50.0f, _pos.y - 80.0f });
			IDialog::drawStr(L"いいえ", { _pos.x + 200.0f, _pos.y - 80.0f });
			if (_isConfirm) {
				IDialog::drawStr(L"＞", { _pos.x + 20.0f, _pos.y - 80.0f });
			}
			else {
				IDialog::drawStr(L"＞", { _pos.x + 170.0f, _pos.y - 80.0f });
			}
		}

	private:
		bool _isConfirm = true;
		std::function<void()> _callback;
		std::vector<std::wstring> _contentList;
		int _frameCount = 0;
};