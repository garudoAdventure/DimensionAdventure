#pragma once

#include "GameState.h"
#include "GameTitle.h"
#include "FadeCover.h"
#include "./Render/Shader.h"
#include "./Render/Texture.h"
#include "./Render/Sprite.h"

class GameEnd : public GameState {
	public:
		static constexpr int FADE_TIME = 180;

		GameEnd() {
			Float4 fadeColor = Color::black;
			fadeColor.a = 0.0f;
			_fadeCover = new FadeCover(FADE_TIME, fadeColor);
			_gameClearTexColor.a = 0.0f;
			_thankPlayTexColor.a = 0.0f;
			_gameClearTex = TEXTURE.loadTexture("./assets/gameClear.png");
			_thankPlayTex = TEXTURE.loadTexture("./assets/thankPlay.png");
			_congratulationsSE = SOUND.loadSound("./assets/sound/congratulations.wav");

			SOUND.playSound(_congratulationsSE, 0);
		}

		~GameEnd() {
			delete _fadeCover;
		}

		void update() override {
			if (_showGameClearCount < FADE_TIME) {
				_showGameClearCount++;
				_gameClearTexColor.a = (float)_showGameClearCount / FADE_TIME;
				return;
			}
			if (_showThankPlayCount < FADE_TIME) {
				_showThankPlayCount++;
				_thankPlayTexColor.a = (float)_showThankPlayCount / FADE_TIME;
				return;
			}

			if (_fadeCover->fadeOut()) {
				game->setState(new GameTitle());
			}
		}

		void draw() override {
			SHADER.begin();

			// 白い背景
			SHADER.setPS(PS::NO_TEX);
			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, Color::white);
			
			// 文字
			SHADER.setPS(PS::GENERAL);
			SPRITE.drawSprite2D({ 0.0f, 200.0f }, { 631.0f, 105.0f }, TEXTURE.getTexture(_gameClearTex), _gameClearTexColor);
			SPRITE.drawSprite2D({ 0.0f, 50.0f }, { 850.0f, 84.0f }, TEXTURE.getTexture(_thankPlayTex), _thankPlayTexColor);
			
			_fadeCover->draw();
		}

	private:
		FadeCover* _fadeCover;
		unsigned int _gameClearTex;
		unsigned int _thankPlayTex;
		unsigned int _congratulationsSE;
		int _showGameClearCount = 0;
		int _showThankPlayCount = 0;
		Float4 _gameClearTexColor = Color::white;
		Float4 _thankPlayTexColor = Color::white;
};