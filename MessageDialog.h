#pragma once

#include "Dialog.h"
#include "Shader.h"
#include "Keyboard.h"
#include "MathTool.h"
#include "Sound.h"
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
			_dialogWidth = 800.0f;
			_context = context;
			_currentContentNum = _context[0].content.length();
			_playerIconTex = TEXTURE.loadTexture("./assets/UI/playerIcon.png");
			_spiritIconTex = TEXTURE.loadTexture("./assets/UI/spiritIcon.png");
			_playerMessageSE = SOUND.loadSound("./assets/sound/message1.wav");
			_spiritMessageSE = SOUND.loadSound("./assets/sound/message2.wav");
			switch (_context.at(_contextIdx).talker) {
				case Talker::SELF:
					_currentSE = _playerMessageSE;
					break;
				case Talker::SPIRIT:
					_currentSE = _spiritMessageSE;
					break;
			}
			SOUND.playSound(_currentSE, -1);
		}
		void dialogUpdate() override {
			if (_contentIdx == _currentContentNum && Keyboard_IsKeyTrigger(KK_ENTER)) {
				_contextIdx += 1;
				if (_contextIdx == _context.size()) {
					_isEnd = true;
				}
				else {
					_currentContentNum = _context[_contextIdx].content.length();
					_contentIdx = 0;
					switch (_context.at(_contextIdx).talker) {
						case Talker::SELF:
							_currentSE = _playerMessageSE;
							break;
						case Talker::SPIRIT:
							_currentSE = _spiritMessageSE;
							break;
					}
					SOUND.playSound(_currentSE, -1);
				}
			}
			if (_frameCount % 2 == 0) {
				_contentIdx = std::min(_currentContentNum, _contentIdx + 1);
				if (_contentIdx == _currentContentNum) {
					SOUND.stopSound(_currentSE);
				}
			}
			_frameCount++;
		}
		void dialogDraw() override {
			std::wstring wstr = _context.at(_contextIdx).content.substr(0, _contentIdx);
			int iconTex = -1;
			switch (_context.at(_contextIdx).talker) {
				case Talker::SELF:
					iconTex = _playerIconTex;
					break;
				case Talker::SPIRIT:
					iconTex = _spiritIconTex;
					break;
			}
			SHADER.setPS(PS::GENERAL);
			SPRITE.drawSprite2D(
				{ _pos.x - _size.x / 2 + 20.0f, _pos.y - _size.y / 2 + 20.0f },
				{ 102.0f, 102.0f }, TEXTURE.getTexture(iconTex), Color::white
			);
			IDialog::drawStrInRect(wstr, _pos, _margin);
		}

	private:
		unsigned int _playerIconTex;
		unsigned int _spiritIconTex;
		unsigned int _playerMessageSE;
		unsigned int _spiritMessageSE;
		unsigned int _currentSE;
		std::vector<DialogContext> _context;
		int _frameCount = 0;
};