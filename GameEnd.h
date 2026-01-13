#pragma once

#include "GameState.h"
#include "Sprite.h"
#include "GameTitle.h"

class GameEnd : public GameState {
	public:
		GameEnd() {
			_gameClearTex = TEXTURE.loadTexture("./assets/gameClear.png");
			_thankPlayTex = TEXTURE.loadTexture("./assets/thankPlay.png");
			_congratulationsSE = SOUND.loadSound("./assets/sound/congratulations.wav");
			_coverColor.a = 0.0f;
			_gameClearTexColor.a = 0.0f;
			_thankPlayTexColor.a = 0.0f;

			SOUND.playSound(_congratulationsSE, 0);
		}
		void update() override {
			if (_showGameClearCount < 180) {
				_showGameClearCount++;
				_gameClearTexColor.a = _showGameClearCount / 180.0f;
				return;
			}
			if (_showThankPlayCount < 180) {
				_showThankPlayCount++;
				_thankPlayTexColor.a = _showThankPlayCount / 180.0f;
				return;
			}
			if (_fadeOutCount < 180) {
				_fadeOutCount++;
				_coverColor.a = _fadeOutCount / 180.0f;
				return;
			}
			game->setState(new GameTitle());
		}
		void draw() override {
			SHADER.begin();
			SHADER.setPS(PS::NO_TEX);
			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, Color::white);
			SHADER.setPS(PS::GENERAL);
			SPRITE.drawSprite2D({ 0.0f, 200.0f }, { 631.0f, 105.0f }, TEXTURE.getTexture(_gameClearTex), _gameClearTexColor);
			SPRITE.drawSprite2D({ 0.0f, 50.0f }, { 850.0f, 84.0f }, TEXTURE.getTexture(_thankPlayTex), _thankPlayTexColor);
			SHADER.setPS(PS::NO_TEX);
			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, _coverColor);
		}

	private:
		unsigned int _gameClearTex;
		unsigned int _thankPlayTex;
		unsigned int _congratulationsSE;
		int _showGameClearCount = 0;
		int _showThankPlayCount = 0;
		int _fadeOutCount = 0;
		Float4 _gameClearTexColor = Color::white;
		Float4 _thankPlayTexColor = Color::white;
		Float4 _coverColor = Color::black;
};