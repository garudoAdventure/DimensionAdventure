#pragma once

#include "DirectX.h"
#include "DirectWrite.h"
#include "Texture.h"
#include "Sprite.h"
#include "Keyboard.h"
#include <string>
#include <sstream>
#include <vector>

class Dialog {
	public:
		Dialog(std::vector<std::string> context) : _context(context) {
			currentContextNum = _context[0].length();
		}
		void update() {
			if (openDialogAnimCount < 10) {
				openDialogAnimCount++;
				return;
			}
			if (Keyboard_IsKeyTrigger(KK_ENTER)) {
				contextIdx += 1;
				if (contextIdx < _context.size()) {
					currentContextNum = _context[contextIdx].length();
					currentContextIdx = 0;
				}
			}
			frameCount++;
		}
		void draw() {
			if (contextIdx == _context.size()) return;
			if (openDialogAnimCount < 10) {
				size.x = MathTool::lerp<float>(0.0f, 500.0f, openDialogAnimCount / 10.0f);
				SPRITE.drawSprite2D(pos, size, { 0.7f, 0.7f, 0.7f, 0.9f });
				return;
			}
			SPRITE.drawSprite2D(pos, size, { 0.7f, 0.7f, 0.7f, 0.9f });
			
			std::string str = _context[contextIdx].substr(0, currentContextIdx * 2);
			DWRITE.drawString(str, {
				pos.x + 640.0f - size.x * 0.5f + margin.x,
				360.0f - pos.y - size.y * 0.5f + margin.y
			});

			if (frameCount % 2 == 0) {
				currentContextIdx = std::min(currentContextNum, currentContextIdx + 1);
			}
		}
		bool isEnd() {
			return contextIdx == _context.size();
		}

	protected:
		Float2 pos = { 0.0f, 0.0f };
		Float2 size = { 500.0f, 150.0f };
		const Float2 margin = { 20.0f, 10.0f };
		std::vector<std::string> _context;
		int contextIdx = 0;
		int currentContextNum = 0;
		int currentContextIdx = 0;
		int openDialogAnimCount = 0;
		int contextSpeed = 3;
		int frameCount = 0;
};

class GetItemDialog : public Dialog {
	public:
		GetItemDialog(std::vector<std::string> context) : Dialog(context) {
			pos = { 0.0f, 200.0f };
			size = { 300.0f, 80.0f };
		}
};