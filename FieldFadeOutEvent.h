#pragma once

#include "IGameEvent.h"
#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"
#include "MathTool.h"

class FieldFadeOutEvent : public IGameEvent {
	public:
		FieldFadeOutEvent(Float2 focusPos) : _focusPos(focusPos) {
			coverTex = TEXTURE.loadTexture("./assets/UI/cover.png");
		}
		~FieldFadeOutEvent() {

		}
		void update() override {
			count++;
		}
		void draw() override {
			SHADER.setSamplerState(SamplerState::CLAMP);
			
			if (count > 60) {
				SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 1280.0f }, { 0.0f, 0.0f, 0.0f, 1.0f });
			}
			
			float scale =  MathTool::easeInQuad<float>(-1.6f, 30.0f, count / 60.f);
			SPRITE.drawSprite2DUV(_focusPos, { 1280.0f, 1280.0f }, coverTex, scale);

			SHADER.setSamplerState(SamplerState::WRAP);
		}
		bool isEnd() override {
			return count == 90;
		}

	private:
		unsigned int coverTex;
		Float2 _focusPos;
		int count = 0;
};