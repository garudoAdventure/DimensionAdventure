#pragma once

#include "IGameEvent.h"
#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"
#include "MathTool.h"

class FieldFadeInEvent : public IGameEvent {
	public:
		FieldFadeInEvent(Float2 focusPos) : _focusPos(focusPos) {
			coverTex = TEXTURE.loadTexture("./assets/UI/cover.png");
		}
		~FieldFadeInEvent() {

		}
		void update() override {
			count++;
		}
		void draw() override {
			SHADER.setSamplerState(SamplerState::CLAMP);

			float scale = MathTool::easeOutQuad<float>(20.0f, -2.0f, count / 45.0f);
			SPRITE.drawSprite2DUV(_focusPos, { 1280.0f, 1280.0f }, coverTex, scale);
			
			SHADER.setSamplerState(SamplerState::WRAP);
		}
		bool isEnd() override {
			return count == 35;
		}

	private:
		unsigned int coverTex;
		Float2 _focusPos;
		int count = 0;
};