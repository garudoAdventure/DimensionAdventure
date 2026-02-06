#pragma once

#define NOMINMAX

#include "./Render/Shader.h"
#include "./Render/Sprite.h"
#include "./Common/Color.h"
#include "./Utils/MathTool.h"

class FadeCover {
	public:
		FadeCover(int fadeTime, Float4 color = Color::black) :
			_fadeTime(fadeTime), _fadeColor(color) {
		}
		
		bool fadeIn() {
			_fadeColor.a = MathTool::lerp<float>(1.0f, 0.0f, (float)_fadeInCount / _fadeTime);
			_fadeInCount = std::min(_fadeTime, _fadeInCount + 1);
			return _fadeInCount == _fadeTime;
		}

		bool fadeOut() {
			_fadeColor.a = MathTool::lerp<float>(0.0f, 1.0f, (float)_fadeOutCount / _fadeTime);
			_fadeOutCount = std::min(_fadeTime, _fadeOutCount + 1);
			return _fadeOutCount == _fadeTime;
		}

		void draw() {
			SHADER.setPS(PS::NO_TEX);
			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, _fadeColor);
		}

	private:
		Float4 _fadeColor;
		const int _fadeTime;
		int _fadeInCount = 0;
		int _fadeOutCount = 0;
};