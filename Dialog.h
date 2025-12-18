#pragma once

#include "DirectX.h"
#include "DirectWrite.h"
#include "Texture.h"
#include "Sprite.h"

class IDialog {
	public:
		IDialog() {
			_tex = TEXTURE.loadTexture("./assets/messageBox.png");
		}
		virtual void update() = 0;
		virtual void draw() = 0;
		void drawMessageBox() {
			SPRITE.drawSprite2D(_pos, _size, _tex, 0.8f);
		}
		void drawStr(std::wstring wstr, Float2 pos) {
			Float2 strPos = {
				pos.x + 640.0f - _size.x * 0.5f + _margin.x,
				360.0f - pos.y - _size.y * 0.5f + _margin.y
			};
			DWRITE.drawString(wstr, strPos);
		}
		void drawStrInRect(std::wstring wstr, Float2 pos, Float2 margin) {
			pos.x = pos.x + 640.0f;
			pos.y = 360.0f - pos.y;
			D2D1_RECT_F rect = {
				pos.x - _size.x * 0.5f + margin.x,
				pos.y - _size.y * 0.5f + margin.y,
				pos.x + _size.x * 0.5f - margin.x,
				pos.y + _size.y * 0.5f - margin.y,
			};
			DWRITE.drawString(wstr, rect);
		}
		bool isEnd() {
			return _isEnd;
		}

	protected:
		Float2 _pos = { 0.0f, 0.0f };
		Float2 _size = { 0.0f, 0.0f };
		Float2 _margin = { 50.0f, 40.0f };
		bool _isEnd = false;
		
	private:
		unsigned int _tex;
};