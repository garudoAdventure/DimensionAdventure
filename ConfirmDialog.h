#pragma once

#include "Dialog.h"
#include "Keyboard.h"
#include "MathTool.h"

#include <string>
#include <functional>

class ConfirmDialog : public IDialog {
	public:
		ConfirmDialog(std::vector<std::wstring> context, std::function<void()> callback) : _callback(callback) {
			_pos = { 0.0f, 200.0f };
			_size = { 0.0f, 200.0f };
			_context = context;
			_currentContextNum = _context.at(0).length();
		}
		void update() override {
			if (_openDialogAnimCount <= 10) {
				_size.x = MathTool::lerp<float>(0.0f, 500.0f, _openDialogAnimCount / 10.0f);
				_openDialogAnimCount++;
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
			if (_frameCount % 2 == 0) {
				_currentContextIdx = std::min(_currentContextNum, _currentContextIdx + 1);
			}
			_frameCount++;
		}
		void draw() override {
			IDialog::drawMessageBox();

			std::wstring wstr = _context.at(_contextIdx).substr(0, _currentContextIdx);
			IDialog::drawStr(wstr, _pos);

			if (_currentContextIdx != _currentContextNum) {
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
		std::vector<std::wstring> _context;
		int _contextIdx = 0;
		int _currentContextNum = 0;
		int _currentContextIdx = 0;
		int _openDialogAnimCount = 0;
		int _frameCount = 0;
};