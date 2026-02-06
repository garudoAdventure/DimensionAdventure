#pragma once

#define NOMINMAX

#include "GameState.h"
#include "GameMain.h"
#include "Sound.h"
#include "MazeBg.h"
#include "FadeCover.h"
#include "RenderTexture.h"
#include "./DirectX/DirectX.h"
#include "./Render/Shader.h"
#include "./Render/Texture.h"
#include "./Render/Sprite.h"
#include "./Render/Model.h"
#include "./Render/ModelManager.h"
#include "./PostProcess/Bloom.h"
#include "./PostProcess/Glitch.h"
#include "./Common/Color.h"
#include "./Common/MathStruct.h"
#include "./Utils/MathTool.h"

class GameTitle : public GameState {
	public:
		static constexpr int FADE_TIME = 180;
		static constexpr int COLOR_SPEED = 120;

		GameTitle() {
			_bloomCubeTex = new RenderTexture(1280.0f, 720.0f);
			_bloomCube = new Bloom(_bloomCubeTex);
			_titleTex = new RenderTexture(1280.0f, 720.0f);
			_glitchTitle = new Glitch(_titleTex);
			_mazeBg = new MazeBg();
			_fadeCover = new FadeCover(FADE_TIME);
			_cubeModel = MODEL.loadModel("./assets/model/transparentBox.fbx");
			_titleStrTex = TEXTURE.loadTexture("./assets/gameTitle.png");
			_titleStartTex = TEXTURE.loadTexture("./assets/titleStartHint.png");
			_bgm = SOUND.loadSound("./assets/sound/gameTitle.wav");
			_confirmSE = SOUND.loadSound("./assets/sound/don.wav");

			SOUND.playSound(_bgm, -1);
		}

		~GameTitle() {
			delete _mazeBg;
			delete _bloomCubeTex;
			delete _bloomCube;
			delete _titleTex;
			delete _glitchTitle;
			delete _fadeCover;
		}

		void update() override {
			_cubeRotate += !_isPressStart ? 0.01f : 0.05f;
			updateCubeColor();
			_bloomCube->setClipLuminance(false);
			_bloomCube->update();
			_glitchTitle->update();
			_mazeBg->update();

			if (!_isShowTitle) {
				if (_fadeCover->fadeIn()) {
					_isShowTitle = true;
				}
				return;
			}

			if (_isPressStart) {
				if (_fadeCover->fadeOut()) {
					game->setState(new GameMain());
				}
				return;
			}

			if (Keyboard_IsKeyTrigger(KK_ENTER)) {
				_isPressStart = true;
				SOUND.stopSound(_bgm);
				SOUND.playSound(_confirmSE, 0);
			}

			_time++;
		}

		void draw() override {
			// 発光立方体を描画
			_bloomCubeTex->setTargetView();
			_bloomCubeTex->clear();
			SHADER.begin();
			SHADER.setView({ 0.0f, 0.0f, -10.0f }, { 0.0f, 0.0f, 0.0f });
			SHADER.setProjection(SHADER.getPerspectiveMatrix());
			_cubeModel->updateColor(_color);
			_cubeModel->draw({ 0.0f, 0.0f, 0.0f }, { 0.0f, _cubeRotate, PI / 4 });

			// タイトル文字描画
			_titleTex->setTargetView();
			_titleTex->clear();
			SPRITE.drawSprite2D({ 0.0f, 150.0f }, { 661.0f, 225.0f }, TEXTURE.getTexture(_titleStrTex), Color::white);

			DX3D.setTargetView();
			DX3D.clear();
			
			_mazeBg->draw(_color);
			_bloomCube->drawBloom(5);

			if (!_isShowTitle) {
				SHADER.setPS(PS::GENERAL);
				SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, _titleTex->getTex(), Color::white);
			}
			else {
				_glitchTitle->draw();
				if (_time % 60 > 20) {
					SHADER.setPS(PS::GENERAL);
					SPRITE.drawSprite2D({ 0.0f, -150.0f }, { 525.0f, 65.0f }, TEXTURE.getTexture(_titleStartTex), Color::white);
				}
			}

			_fadeCover->draw();
		}

	private:
		Model* _cubeModel;
		Bloom* _bloomCube;
		Glitch* _glitchTitle;
		RenderTexture* _bloomCubeTex;
		RenderTexture* _titleTex;
		FadeCover* _fadeCover;
		MazeBg* _mazeBg;

		unsigned int _titleStrTex;
		unsigned int _titleStartTex;
		unsigned int _bgm;
		unsigned int _confirmSE;
		
		Float4 _color = { 0.0f, 1.0f, 0.0f, 1.0f };
		int _redCount = COLOR_SPEED;
		int _greenCount = 0;
		int _blueCount = 0;

		int _time = 0;
		float _cubeRotate = 0.0f;
		bool _isPressStart = false;
		bool _isShowTitle = false;

		void updateCubeColor() {
			if (_redCount == COLOR_SPEED && _blueCount == 0) {
				_greenCount = std::min(COLOR_SPEED, _greenCount + 1);
			}
			if (_greenCount == COLOR_SPEED) {
				_redCount = std::max(0, _redCount - 1);
			}
			if (_redCount == 0) {
				_blueCount = std::min(COLOR_SPEED, _blueCount + 1);
			}
			if (_blueCount == COLOR_SPEED) {
				_greenCount = std::max(0, _greenCount - 1);
			}
			if (_greenCount == 0) {
				_redCount = std::min(COLOR_SPEED, _redCount + 1);
			}
			if (_redCount == COLOR_SPEED) {
				_blueCount = std::max(0, _blueCount - 1);
			}

			_color.r = MathTool::lerp<float>(0.0f, 1.0f, _redCount / 120.0f);
			_color.g = MathTool::lerp<float>(0.0f, 1.0f, _greenCount / 120.0f);
			_color.b = MathTool::lerp<float>(0.0f, 1.0f, _blueCount / 120.0f);
		}
};