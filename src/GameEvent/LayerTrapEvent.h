#pragma once

#include "IGameEvent.h"
#include "RenderTexture.h"
#include "Sound.h"
#include "Layer.h"
#include "FadeCover.h"
#include "./DirectX/DirectX.h"
#include "./Game/IGameEventHandler.h"
#include "./Player/Player.h"
#include "./Render/Shader.h"
#include "./Render/Texture.h"
#include "./Render/Sprite.h"
#include "./Common/Color.h"
#include "./Utils/MathTool.h"

class LayerTrapEvent : public IGameEvent {
	public:
		static constexpr int SCREEN_SCATTER_TIME = 180;
		static constexpr int FALLING_LAYER_TIME = 300;
		static constexpr int LAYER_SWITCH_SPEED = 60;

		LayerTrapEvent(IGameEventHandler* gameEvent, unsigned int earthquakeSE) : _gameEvent(gameEvent), _earthquakeSE(earthquakeSE){
			_screenTex = _gameEvent->getScreenTex();
			_layerTex = new RenderTexture(1280.0f, 720.0f, Color::darkGray);
			_fadeCover = new FadeCover(300);
			_borderTex = TEXTURE.loadTexture("./assets/UI/border.png");
			_trapSE = SOUND.loadSound("./assets/sound/trap.wav");
		}

		void update() override {
			PLAYER.update();
			_gameEvent->updateField();
			_gameEvent->cameraVibration(true);
			if (!_isScreenScatterEnd) {
				_screenPos += _scatterVel * 50;
				if (_screenScatterCount <= 10) {
					_screenScatterCount++;
				}
				else {
					_screenScatterCount = 0;
					int r = rand();
					_scatterVel.x = cosf(r);
					_scatterVel.y = sinf(r);
					_screenPos = { 0.0f, 0.0f };
					SOUND.playSound(_trapSE, 0);
				}
				if (_time == SCREEN_SCATTER_TIME) {
					_isScreenScatterEnd = true;
					_time = 0;
				}
			}
			else {
				if (_time == FALLING_LAYER_TIME) {
					PLAYER.setLayer(LayerType::RED);
					_gameEvent->cameraVibration(false);
					SOUND.stopSound(_earthquakeSE);
					_isEnd = true;
				}
				SOUND.setVolume(_earthquakeSE, (float)(FALLING_LAYER_TIME - _time) / FALLING_LAYER_TIME);
				_fadeCover->fadeOut();
			}

			_time++;
		}

		void draw() override {
			_screenTex->setTargetView();
			PLAYER.draw();

			_layerTex->setTargetView();
			_layerTex->clear();
			_gameEvent->drawGameScene(0);

			DX3D.setTargetView();

			if (!_isScreenScatterEnd) {
				// 画面を散らばっている演出
				DX3D.setBlendMode(BlendMode::NORMAL);
				SHADER.setPS(PS::GENERAL);
				SPRITE.drawSprite2D(_screenPos, { 1280.0f, 720.0f }, _layerTex->getTex(), { 1.0f, 1.0f, 1.0f, 0.8f });
			}
			else {
				// 別レイヤーに飛ばされる演出
				DX3D.clear({ 0.0f, 0.0f, 0.0f, 1.0f });
				SHADER.begin();

				if (_eyeMoveCount <= 30) {
					_eye.x = MathTool::lerp<float>(0.0f, -35.0f, (float)_eyeMoveCount / 30);
					_eye.y = MathTool::lerp<float>(0.0f, 14.0f, (float)_eyeMoveCount / 30);
					_eye.z = MathTool::lerp<float>(-30.0f, -45.0f, (float)_eyeMoveCount / 30);
					_eyeMoveCount++;
				}

				for (int i = 5; i >= 0; i--) {
					Float4 color = Color::white;
					if (_layerSwitchCount == LAYER_SWITCH_SPEED) {
						_layerSwitchCount = 0;
					} else {
						_layerSwitchCount++;
						if (i == 0) {
							color.a = 1.0f - ((float)_layerSwitchCount / LAYER_SWITCH_SPEED);
						}
						if (i == 5) {
							color.a = (float)_layerSwitchCount / LAYER_SWITCH_SPEED;
						}
					}

					XMMATRIX world = XMMatrixIdentity();
					world *= XMMatrixTranslation(0.0f, 0.0f, 10.0f * (i - 1) - _layerSwitchCount * 10.0f / LAYER_SWITCH_SPEED);
					SHADER.setWorld(world);
					SHADER.setView({ _eye.x, _eye.y, _eye.z - _time * 0.5f }, { 0.0f, 0.0f, 22.5f });
					SHADER.setProjection(SHADER.getPerspectiveMatrix());
					SHADER.setMatrix();

					DX3D.setBlendMode(BlendMode::NORMAL);
					DX3D.setDepthEnable(true);
					SHADER.setPS(PS::GENERAL);
					SPRITE.drawSprite3D({ 1280.0f, 720.0f }, _layerTex->getTex(), color);
				}

				_fadeCover->draw();
			}
		}

		bool isEnd() override {
			return _isEnd;
		}

	private:
		RenderTexture* _screenTex;
		RenderTexture* _layerTex;
		IGameEventHandler* _gameEvent;
		FadeCover* _fadeCover;
		Float2 _screenPos = { 0.0f, 0.0f };
		Float2 _scatterVel = { 0.0f, 0.0f };;
		Float3 _eye = { 0.0f, 0.0f, 0.0f };
		unsigned int _borderTex;
		unsigned int _trapSE;
		unsigned int _earthquakeSE;
		int _time = 0;
		int _screenScatterCount = 0;
		int _eyeMoveCount = 0;
		int _layerSwitchCount = 0;
		bool _isScreenScatterEnd = false;
		bool _isEnd = false;
};