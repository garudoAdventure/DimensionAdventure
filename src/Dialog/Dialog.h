#pragma once

#define NOMINMAX

#include "./DirectX/DirectX.h"
#include "./DirectX/DirectWrite.h"
#include "./Render/Shader.h"
#include "./Render/Texture.h"
#include "./Render/Sprite.h"
#include "./Common/Color.h"
#include "./Utils/MathTool.h"

class IDialog {
	public:
		static constexpr int DIALOG_OPEN_TIME = 10;

		IDialog() {
			_tex = TEXTURE.loadTexture("./assets/messageBox.png");
		}
		void update() {
			if (_isEnd) return;
			if (_openDialogAnimCount <= DIALOG_OPEN_TIME) {
				_size.x = MathTool::lerp<float>(0.0f, _dialogWidth, _openDialogAnimCount * 0.1f);
				_openDialogAnimCount++;
				return;
			}
			dialogUpdate();
		}
		void draw() {
			drawMessageBox();
			if (_openDialogAnimCount <= DIALOG_OPEN_TIME) {
				return;
			}
			dialogDraw();
		}
		bool isEnd() {
			return _isEnd;
		}

	protected:
		Float2 _pos{ 0.0f, 0.0f };
		Float2 _size{ 0.0f, 0.0f };
		Float2 _margin{ 50.0f, 40.0f };
		float _dialogWidth = 0.0f;
		bool _isEnd = false;
		int _contentIdx = 0;
		int _contentListIdx = 0;
		int _currentContentStrLen = 0;

		virtual void dialogUpdate() = 0;
		virtual void dialogDraw() = 0;

		void drawMessageBox() {
			Float4 color = Color::white;
			color.a = 0.7f;
			SHADER.setPS(PS::GENERAL);
			SPRITE.drawSprite2D(_pos, _size, TEXTURE.getTexture(_tex), color);
		}
		void drawStr(std::wstring wstr, Float2 pos) {
			const Float2 strPos = {
				pos.x + DX3D.SCREEN_WIDTH * 0.5f - _size.x * 0.5f + _margin.x,
				DX3D.SCREEN_HEIGHT * 0.5f - pos.y - _size.y * 0.5f + _margin.y
			};
			DWRITE.drawString(wstr, strPos);
		}
		void drawStrInRect(std::wstring wstr, Float2 pos, Float2 margin) {
			pos.x = pos.x + DX3D.SCREEN_WIDTH * 0.5f;
			pos.y = DX3D.SCREEN_HEIGHT * 0.5f - pos.y;
			const D2D1_RECT_F rect = {
				pos.x - _size.x * 0.5f + margin.x,
				pos.y - _size.y * 0.5f + margin.y,
				pos.x + _size.x * 0.5f - margin.x,
				pos.y + _size.y * 0.5f - margin.y,
			};
			DWRITE.drawString(wstr, rect);
		}
		
	private:
		unsigned int _tex;
		int _openDialogAnimCount = 0;
};