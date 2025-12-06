#pragma once

#include "DirectX.h"
#include "DirectWrite.h"
#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"
#include "Keyboard.h"
#include "MathTool.h"
#include <string>
#include <sstream>
#include <vector>
#include <functional>

enum class Talker {
	SELF,
	SPIRIT
};

struct DialogContext {
	Talker talker;
	std::string content;
};

class IDialog {
	public:
		IDialog() {
			tex = TEXTURE.loadTexture("./assets/messageBox.png");
		}
		virtual void update() = 0;
		virtual void draw() = 0;
		virtual void draw(Float3 pos) {}
		void drawMessageBox() {
			SPRITE.drawSprite2D(_pos, _size, tex, 0.8f);
		}
		void drawStr(Float2 pos, std::string str) {
			DWRITE.drawString(str, {
				pos.x + 640.0f - _size.x * 0.5f + _margin.x,
				360.0f - pos.y - _size.y * 0.5f + _margin.y
			});
		}
		bool isEnd() {
			return _isEnd;
		}

	protected:
		Float2 _pos;
		Float2 _size;
		Float2 _margin = { 50.0f, 40.0f };
		 std::vector<DialogContext> _context;
		int contextIdx = 0;
		int currentContentStrNum = 0;
		int currentContentStrIdx = 0;
		int openDialogAnimCount = 0;
		int contentSpeed = 3;
		int frameCount = 0;
		bool _isEnd = false;

	private:
		unsigned int tex;
};

class MessageDialog : public IDialog {
	public:
		MessageDialog(std::vector<DialogContext> context) {
			_pos = { 0.0f, 100.0f };
			_size = { 0.0f, 200.0f };
			_context = context;
			currentContentStrNum = _context[0].content.length();
			playerIconTex = TEXTURE.loadTexture("./assets/UI/playerIcon.png");
			spiritIconTex = TEXTURE.loadTexture("./assets/UI/spiritIcon.png");
		}
		void update() override {
			if (_isEnd) return;
			if (openDialogAnimCount <= 10) {
				_size.x = MathTool::lerp<float>(0.0f, 800.0f, openDialogAnimCount / 10.0f);
				openDialogAnimCount++;
				return;
			}
			if (currentContentStrIdx == currentContentStrNum / 2 && Keyboard_IsKeyTrigger(KK_ENTER)) {
				contextIdx += 1;
				if (contextIdx < _context.size()) {
					currentContentStrNum = _context[contextIdx].content.length();
					currentContentStrIdx = 0;
				}
				if (contextIdx == _context.size()) {
					_isEnd = true;
				}
			}
			if (frameCount % 2 == 0) {
				currentContentStrIdx = std::min(currentContentStrNum / 2, currentContentStrIdx + 1);
			}
			frameCount++;
		}
		void draw() override {
			if (_isEnd) return;
			std::string str = _context[contextIdx].content.substr(0, currentContentStrIdx * 2);
			IDialog::drawMessageBox();
			int iconTex = -1;
			if (_context[contextIdx].talker == Talker::SELF) {
				iconTex = playerIconTex;
			}
			if (_context[contextIdx].talker == Talker::SPIRIT) {
				iconTex = spiritIconTex;
			}
			if (openDialogAnimCount > 10) {
				SPRITE.drawSprite2D(
					{ _pos.x - _size.x / 2 + 20.0f, _pos.y - _size.y / 2 + 20.0f },
					{ 102.0f, 102.0f }, iconTex
				);
			}
			IDialog::drawStr(_pos, str);
		}

	private:
		unsigned int playerIconTex;
		unsigned int spiritIconTex;
};

class GetItemDialog : public IDialog {
	public:
		GetItemDialog(std::vector<std::string> context) {
			_pos = { 0.0f, 200.0f };
			_size = { 0.0f, 120.0f };
			_context = context;
			currentContextNum = _context[0].length();
		}
		void update() override {
			if (openDialogAnimCount <= 10) {
				_size.x = MathTool::lerp<float>(0.0f, 380.0f, openDialogAnimCount / 10.0f);
				openDialogAnimCount++;
				return;
			}
			if (frameCount % 2 == 0) {
				currentContextIdx = std::min(currentContextNum, currentContextIdx + 1);
			}
			frameCount++;
		}
		void draw() override {
			std::string str = _context[contextIdx].substr(0, currentContextIdx * 2);
			IDialog::drawMessageBox();
			IDialog::drawStr(_pos, str);
		}

	private:
		std::vector<std::string> _context;
		int currentContextNum;
		int currentContextIdx;
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
			SHADER.begin();
			// ビルボード処理
			XMMATRIX world = XMMatrixIdentity();
			world *= SHADER.getInverseView();
			world *= XMMatrixTranslation(pos.x, pos.y, pos.z);
			SHADER.setWorldMatrix(world);
			SPRITE.drawSpriteIn3D({0.0f, 0.0f, 0.0f}, {32.0f, 32.0f}, tex);
		}
	
	private:
		unsigned int tex;
};

class ConfirmDialog : public IDialog {
	public:
		ConfirmDialog(std::vector<std::string> context, std::function<void()> callback) : _callback(callback) {
			_pos = { 0.0f, 200.0f };
			_size = { 0.0f, 200.0f };
			_context = context;
			currentContextNum = _context[0].length();
		}
		void update() override {
			if (openDialogAnimCount <= 10) {
				_size.x = MathTool::lerp<float>(0.0f, 500.0f, openDialogAnimCount / 10.0f);
				openDialogAnimCount++;
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
			if (frameCount % 2 == 0) {
				currentContextIdx = std::min(currentContextNum / 2, currentContextIdx + 1);
			}
			frameCount++;
		}
		void draw() override {
			IDialog::drawMessageBox();

			std::string str = _context[contextIdx].substr(0, currentContextIdx * 2);
			IDialog::drawStr(_pos, str);

			if (currentContextIdx == currentContextNum / 2) {
				IDialog::drawStr({ _pos.x + 50.0f, _pos.y - 80.0f }, "はい");
				IDialog::drawStr({ _pos.x + 200.0f, _pos.y - 80.0f }, "いいえ");
				if (_isConfirm) {
					IDialog::drawStr({ _pos.x + 20.0f, _pos.y - 80.0f }, "＞");
				}
				else {
					IDialog::drawStr({ _pos.x + 170.0f, _pos.y - 80.0f }, "＞");
				}
			}
		}

	private:
		bool _isConfirm = true;
		std::function<void()> _callback;
		std::vector<std::string> _context;
		int currentContextNum;
		int currentContextIdx;
};

class DimensionTutorialDialog : public IDialog {
	public:
		DimensionTutorialDialog() {
			_pos = { 0.0f, 0.0f };
			_size = { 800.0f, 500.0f };
			_margin.y = 80.0f;
			_margin.x = 150.0f;
			pic2DTex = TEXTURE.loadTexture("./assets/tutorial/2D.png");
			pic3DTex = TEXTURE.loadTexture("./assets/tutorial/3D.png");
			arrowTex = TEXTURE.loadTexture("./assets/UI/arrow.png");
			backspaceTex = TEXTURE.loadTexture("./assets/tutorial/backspace.png");
		}
		void update() override {
			if (Keyboard_IsKeyTrigger(KK_ENTER)) {
				_isEnd = true;
			}
		}
		void draw() override {
			IDialog::drawMessageBox();
			IDialog::drawStr(_pos, "を押して、2Dと3D世界の切り替えができる！");
			SPRITE.drawSprite2D({ -310.0f, 150.0f }, { 106.0f, 60.0f }, backspaceTex);
			SPRITE.drawSprite2D({ -200.0f, -50.0f }, { 320.0f, 180.0f }, pic2DTex);
			SPRITE.drawSprite2D({ 200.0f, -50.0f }, { 320.0f, 180.0f }, pic3DTex);
			SPRITE.drawSprite2D({ 0.0f, 80.0f }, { 94.0f, 34.0f }, arrowTex);
			SPRITE.drawSprite2DRotate({ 0.0f, 180.0f }, { 94.0f, 34.0f }, arrowTex, PI, { 0.0f, 0.0f });
		}

	private:
		unsigned int pic2DTex;
		unsigned int pic3DTex;
		unsigned int arrowTex;
		unsigned int backspaceTex;
};

class LayerTutorialDialog : public IDialog {
	public:
		LayerTutorialDialog() {
			_pos = { 0.0f, 0.0f };
			_size = { 800.0f, 500.0f };
			_margin.y = 80.0f;
			_margin.x = 150.0f;
			pic2DTex = TEXTURE.loadTexture("./assets/tutorial/2D.png");
			pic3DTex = TEXTURE.loadTexture("./assets/tutorial/3D.png");
			arrowTex = TEXTURE.loadTexture("./assets/UI/arrow.png");
			backspaceTex = TEXTURE.loadTexture("./assets/tutorial/backspace.png");
		}
		void update() override {
			if (Keyboard_IsKeyTrigger(KK_ENTER)) {
				_isEnd = true;
			}
		}
		void draw() override {
			IDialog::drawMessageBox();
			IDialog::drawStr(_pos, "を押して、2Dと3D世界の切り替えができる！");
			SPRITE.drawSprite2D({ -310.0f, 150.0f }, { 106.0f, 60.0f }, backspaceTex);
			SPRITE.drawSprite2D({ -200.0f, -50.0f }, { 320.0f, 180.0f }, pic2DTex);
			SPRITE.drawSprite2D({ 200.0f, -50.0f }, { 320.0f, 180.0f }, pic3DTex);
			SPRITE.drawSprite2D({ 0.0f, 80.0f }, { 94.0f, 34.0f }, arrowTex);
			SPRITE.drawSprite2DRotate({ 0.0f, 180.0f }, { 94.0f, 34.0f }, arrowTex, PI, { 0.0f, 0.0f });
		}

	private:
		unsigned int pic2DTex;
		unsigned int pic3DTex;
		unsigned int arrowTex;
		unsigned int backspaceTex;
};