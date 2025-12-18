#pragma once

#define NOMINMAX

#include "GameState.h"
#include "DirectX.h"
#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include "ModelManager.h"
#include "Color.h"
#include "PostProcess.h"
#include "RenderTexture.h"
#include "MathTool.h"
#include "GameMain.h"

#define ColorSpeed	(240)

struct PixelTime {
	float time;
	Float3 dummy;
};

class GameTitle : public GameState {
	public:
		GameTitle() {
			_model = MODEL.loadModel("./assets/model/transposeBox.fbx");
			_titleTex = TEXTURE.loadTexture("./assets/gameTitle.png");
			_titleStartTex = TEXTURE.loadTexture("./assets/titleStartHint.png");
			_title = new RenderTexture(1280.0f, 720.0f);
			_bg = new RenderTexture(1280.0f, 720.0f);
			_postProcess = new PostProcess(_bg);

			D3D11_BUFFER_DESC desc = {};
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.ByteWidth = sizeof(PixelTime);
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;
			desc.CPUAccessFlags = 0;
			DX3D.getDevice()->CreateBuffer(&desc, NULL, &_pixelTimeBuffer);
		}
		~GameTitle() {
			SAFE_RELEASE(_pixelTimeBuffer);
			delete _postProcess;
			delete _bg;
			delete _title;
		}
		void update() override {
			_rotate += !_isStart ? 0.01f : 0.05f;
			_postProcess->update();
			updateColor();

			if (!_isShowTitle) {
				_showTitleCount++;
				_coverAlpha = 1.0f - _showTitleCount / 180.0f;
				if (_showTitleCount == 180) {
					_isShowTitle = true;
				}
			}

			if (_isShowTitle && Keyboard_IsKeyTrigger(KK_ENTER)) {
				_isStart = true;
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
			_bg->setTargetView();
			_bg->clear();
			SHADER.begin();
			SHADER.setView({ 0.0f, 0.0f, -10.0f }, { 0.0f, 0.0f, 0.0f });
			SHADER.setProjection(SHADER.getPerspectiveMatrix());
			_model->updateColor(_color);
			_model->draw({ 0.0f, 0.0f, 0.0f }, { 0.0f, _rotate, PI / 4 });

			_title->setTargetView();
			_title->clear();
			SPRITE.drawSprite2D({ 0.0f, 150.0f }, { 661.0f, 225.0f }, _titleTex);

			DX3D.setTargetView();
			DX3D.clear();
			_postProcess->drawBlurBloom();

			if (_isShowTitle) {
				SHADER.setPS(PS::GLITCH);
				PixelTime pt;
				pt.time = _time;
				DX3D.getDeviceContext()->UpdateSubresource(_pixelTimeBuffer, 0, NULL, &pt, 0, 0);
				DX3D.getDeviceContext()->PSSetConstantBuffers(2, 1, &_pixelTimeBuffer);
			}

			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, _title->getTex());
			
			SHADER.setPS(PS::NORMAL);
			if (_isShowTitle && _time % 60 > 30) {
				SPRITE.drawSprite2D({ 0.0f, -150.0f }, { 525.0f, 65.0f }, _titleStartTex);
			}

			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, { 0.0f, 0.0f, 0.0f, _coverAlpha });
		}

		void updateColor() {
			if (_redCount == ColorSpeed && _blueCount == 0) {
				_greenCount = std::min(ColorSpeed, _greenCount + 1);
			}
			if (_greenCount == ColorSpeed) {
				_redCount = std::max(0, _redCount - 1);
			}
			if (_redCount == 0) {
				_blueCount = std::min(ColorSpeed, _blueCount + 1);
			}
			if (_blueCount == ColorSpeed) {
				_greenCount = std::max(0, _greenCount - 1);
			}
			if (_greenCount == 0) {
				_redCount = std::min(ColorSpeed, _redCount + 1);
			}
			if (_redCount == ColorSpeed) {
				_blueCount = std::max(0, _blueCount - 1);
			}

			_color.r = MathTool::lerp<float>(0.0f, 1.0f, _redCount / 240.0f);
			_color.g = MathTool::lerp<float>(0.0f, 1.0f, _greenCount / 240.0f);
			_color.b = MathTool::lerp<float>(0.0f, 1.0f, _blueCount / 240.0f);
		}

	private:
		Model* _model;
		PostProcess* _postProcess;
		RenderTexture* _bg;
		RenderTexture* _title;
		unsigned int _titleTex;
		unsigned int _titleStartTex;
		ID3D11Buffer* _pixelTimeBuffer;
		Float4 _color = { 1.0f, 0.0f, 0.0f, 1.0f };
		int _redCount = ColorSpeed;
		int _greenCount = 0;
		int _blueCount = 0;
		float _rotate = 0.0f;
		float _coverAlpha = 1.0f;
		int _time = 0;
		bool _isStart = false;
		int _startCount = 0;
		int _showTitleCount = 0;
		bool _isShowTitle = false;
};