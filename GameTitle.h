#pragma once

#define NOMINMAX

#include "GameState.h"
#include "DirectX.h"
#include "Shader.h"
#include "Texture.h"
#include "Sound.h"
#include "Model.h"
#include "ModelManager.h"
#include "Color.h"
#include "PostProcess.h"
#include "RenderTexture.h"
#include "MathStruct.h"
#include "MathTool.h"
#include "GameMain.h"
#include "MazeBg.h"

class GameTitle : public GameState {
	public:
		GameTitle() {
			_model = MODEL.loadModel("./assets/model/transparentBox.fbx");
			_titleTex = TEXTURE.loadTexture("./assets/gameTitle.png");
			_titleStartTex = TEXTURE.loadTexture("./assets/titleStartHint.png");
			_bgm = SOUND.loadSound("./assets/sound/gameTitle.wav");
			_confirmSE = SOUND.loadSound("./assets/sound/don.wav");
			_title = new RenderTexture(1280.0f, 720.0f);
			_bloomCube = new RenderTexture(1280.0f, 720.0f);
			_postProcess = new PostProcess(_bloomCube);
			_mazeBg = new MazeBg();

			D3D11_BUFFER_DESC desc = {};
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.ByteWidth = sizeof(PixelTime);
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;
			desc.CPUAccessFlags = 0;
			DX3D.getDevice()->CreateBuffer(&desc, NULL, &_pixelTimeBuffer);

			SOUND.playSound(_bgm, -1);
		}
		~GameTitle() {
			SAFE_RELEASE(_pixelTimeBuffer);
			delete _mazeBg;
			delete _postProcess;
			delete _bloomCube;
			delete _title;
		}
		void update() override {
			_rotate += !_isStart ? 0.01f : 0.05f;
			_postProcess->update(false);
			updateColor();
			_mazeBg->update();

			if (!_isShowTitle) {
				_showTitleCount++;
				_coverAlpha = 1.0f - _showTitleCount / 180.0f;
				if (_showTitleCount == 180) {
					_isShowTitle = true;
				}
			}

			if (!_isStart && _isShowTitle && Keyboard_IsKeyTrigger(KK_ENTER)) {
				_isStart = true;
				SOUND.stopSound(_bgm);
				SOUND.playSound(_confirmSE, 0);
			}
			if (_isStart) {
				_startCount++;
				_coverAlpha = _startCount / 180.0f;
				if (_startCount == 180) {
					game->setState(new GameMain());
				}
			}
			_time = (_time + 1) % 1800;
		}
		void draw() override {
			_bloomCube->setTargetView();
			_bloomCube->clear();
			SHADER.begin();
			SHADER.setView({ 0.0f, 0.0f, -10.0f }, { 0.0f, 0.0f, 0.0f });
			SHADER.setProjection(SHADER.getPerspectiveMatrix());
			_model->updateColor(_color);
			_model->draw({ 0.0f, 0.0f, 0.0f }, { 0.0f, _rotate, PI / 4 });

			_title->setTargetView();
			_title->clear();
			SPRITE.drawSprite2D({ 0.0f, 150.0f }, { 661.0f, 225.0f }, TEXTURE.getTexture(_titleTex), Color::white);

			DX3D.setTargetView();
			DX3D.clear();
			
			_mazeBg->draw(_color);
			
			_postProcess->drawBloom(5);

			if (_isShowTitle) {
				SHADER.setPS(PS::GLITCH);
				PixelTime pt;
				pt.time = _time;
				DX3D.getDeviceContext()->UpdateSubresource(_pixelTimeBuffer, 0, NULL, &pt, 0, 0);
				DX3D.getDeviceContext()->PSSetConstantBuffers(1, 1, &_pixelTimeBuffer);
			}

			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, _title->getTex(), Color::white);
			
			SHADER.setPS(PS::GENERAL);
			if (_isShowTitle && _time % 60 > 30) {
				SPRITE.drawSprite2D({ 0.0f, -150.0f }, { 525.0f, 65.0f }, TEXTURE.getTexture(_titleStartTex), Color::white);
			}

			SHADER.setPS(PS::NO_TEX);
			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, { 0.0f, 0.0f, 0.0f, _coverAlpha });
		}

		void updateColor() {
			if (_redCount == _colorSpeed && _blueCount == 0) {
				_greenCount = std::min(_colorSpeed, _greenCount + 1);
			}
			if (_greenCount == _colorSpeed) {
				_redCount = std::max(0, _redCount - 1);
			}
			if (_redCount == 0) {
				_blueCount = std::min(_colorSpeed, _blueCount + 1);
			}
			if (_blueCount == _colorSpeed) {
				_greenCount = std::max(0, _greenCount - 1);
			}
			if (_greenCount == 0) {
				_redCount = std::min(_colorSpeed, _redCount + 1);
			}
			if (_redCount == _colorSpeed) {
				_blueCount = std::max(0, _blueCount - 1);
			}

			_color.r = MathTool::lerp<float>(0.0f, 1.0f, _redCount / 120.0f);
			_color.g = MathTool::lerp<float>(0.0f, 1.0f, _greenCount / 120.0f);
			_color.b = MathTool::lerp<float>(0.0f, 1.0f, _blueCount / 120.0f);
		}

	private:
		Model* _model;
		PostProcess* _postProcess;
		RenderTexture* _bloomCube;
		RenderTexture* _title;
		unsigned int _titleTex;
		unsigned int _titleStartTex;
		unsigned int _bgm;
		unsigned int _confirmSE;
		ID3D11Buffer* _pixelTimeBuffer;
		Float4 _color = { 0.0f, 1.0f, 0.0f, 1.0f };
		const int _colorSpeed = 120;
		int _redCount = _colorSpeed;
		int _greenCount = 0;
		int _blueCount = 0;
		float _rotate = 0.0f;
		float _coverAlpha = 1.0f;
		int _time = 0;
		bool _isStart = false;
		int _startCount = 0;
		int _showTitleCount = 0;
		bool _isShowTitle = false;
		MazeBg* _mazeBg;
};