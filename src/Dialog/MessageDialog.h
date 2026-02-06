#pragma once

#include "Dialog.h"
#include "HALKeyboard.h"
#include "Sound.h"
#include "./Render/Shader.h"
#include "./Utils/MathTool.h"
#include <vector>
#include <string>

enum class Talker {
	SELF,
	SPIRIT
};

struct DialogContent {
	Talker talker;
	std::wstring content;
};

class MessageDialog : public IDialog {
	public:
		MessageDialog(std::vector<DialogContent> contentList) : _contentList(contentList) {
			_pos = { 0.0f, 100.0f };
			_size = { 0.0f, 200.0f };
			_dialogWidth = 800.0f;
			_currentContentStrLen = contentList.at(0).content.length();
			_playerIconTex = TEXTURE.loadTexture("./assets/UI/playerIcon.png");
			_spiritIconTex = TEXTURE.loadTexture("./assets/UI/spiritIcon.png");
			_playerMessageSE = SOUND.loadSound("./assets/sound/message1.wav");
			_spiritMessageSE = SOUND.loadSound("./assets/sound/message2.wav");
			switch (contentList.at(_contentListIdx).talker) {
				case Talker::SELF:
					_currentSE = _playerMessageSE;
					break;
				case Talker::SPIRIT:
					_currentSE = _spiritMessageSE;
					break;
			}
		}

		void dialogUpdate() override {
			if (_contentListIdx == 0 && _contentIdx == 0) {
				SOUND.playSound(_currentSE, -1);
			}
			if (_contentIdx == _currentContentStrLen && Keyboard_IsKeyTrigger(KK_ENTER)) {
				if (_contentListIdx + 1 == _contentList.size()) {
					_isEnd = true;
					return;
				}
				_contentListIdx += 1;
				_currentContentStrLen = _contentList.at(_contentListIdx).content.length();
				_contentIdx = 0;
				switch (_contentList.at(_contentListIdx).talker) {
					case Talker::SELF:
						_currentSE = _playerMessageSE;
						break;
					case Talker::SPIRIT:
						_currentSE = _spiritMessageSE;
						break;
				}
				SOUND.playSound(_currentSE, -1);
			}
			if (_frameCount % 2 == 0) {
				_contentIdx = std::min(_currentContentStrLen, _contentIdx + 1);
				if (_contentIdx == _currentContentStrLen) {
					SOUND.stopSound(_currentSE);
				}
			}
			_frameCount++;
		}

		void dialogDraw() override {
			std::wstring wstr = _contentList.at(_contentListIdx).content.substr(0, _contentIdx);
			IDialog::drawStrInRect(wstr, _pos, _margin);

			int iconTex = -1;
			switch (_contentList.at(_contentListIdx).talker) {
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
		}

	private:
		unsigned int _playerIconTex;
		unsigned int _spiritIconTex;
		unsigned int _playerMessageSE;
		unsigned int _spiritMessageSE;
		unsigned int _currentSE;
		std::vector<DialogContent> _contentList;
		int _frameCount = 0;
};