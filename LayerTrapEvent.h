#pragma once

#include "IGameEventHandler.h"
#include "IGameEvent.h"
#include "DirectX.h"
#include "RenderTexture.h"
#include "Sprite.h"
#include "Shader.h"
#include "MathTool.h"
#include "Color.h"
#include "Player.h"

class LayerTrapEvent : public IGameEvent {
	public:
		LayerTrapEvent(IGameEventHandler* gameEvent) : _gameEvent(gameEvent) {
			_screenTex = _gameEvent->getScreenTex();
			_layerTex = new RenderTexture(1280.0f, 720.0f, Color::darkGray);
			_frameTex = TEXTURE.loadTexture("./assets/UI/frame.png");
		}

		void update() override {
			PLAYER.update();
			_gameEvent->updateField();
			_gameEvent->cameraVibration(true);
			switch (phase) {
				case 0:
					_pos += _vel * 50;
					if (frame <= 10) {
						frame++;
					}
					else {
						frame = 0;
						_pos = { 0.0f, 0.0f };
						int r = rand();
						_vel.x = cosf(r);
						_vel.y = sinf(r);
					}
					if (time == 180) {
						phase = 1;
						frame = 0;
					}
					break;

				case 1:
					if (frame == 300) {
						PLAYER.setLayer(LayerType::RED);
						_gameEvent->cameraVibration(false);
						_isEnd = true;
					}
					frame++;
					break;
			}
			time++;
		}

		void draw() override {
			_screenTex->setTargetView();
			PLAYER.draw();

			_layerTex->setTargetView();
			_layerTex->clear();
			DX3D.setBlendMode(BlendMode::REND_TEX);
			SHADER.setPS(PS::GENERAL);
			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, _screenTex->getTex(), Color::white);

			DX3D.setTargetView();
			switch (phase) {
				case 0:
					DX3D.setBlendMode(BlendMode::NORMAL);
					SHADER.setPS(PS::GENERAL);
					SPRITE.drawSprite2D(_pos, _size, _layerTex->getTex(), { 1.0f, 1.0f, 1.0f, 0.8f });
					break;

				case 1:
					DX3D.setTargetView();
					DX3D.clear({0.0f, 0.0f, 0.0f, 1.0f});
					SHADER.begin();
					
					if (eyeTransformCount <= 30) {
						_eye.x = MathTool::lerp<float>(0.0f, -35.0f, eyeTransformCount / 30.0f);
						_eye.y = MathTool::lerp<float>(0.0f, 14.0f, eyeTransformCount / 30.0f);
						_eye.z = MathTool::lerp<float>(-30.0f, -45.0f, eyeTransformCount / 30.0f);
						eyeTransformCount++;
					}

					for (int i = 5; i >= 0; i--) {
						const int layerSpd = 60;
						Float4 color = Color::white;
						if (layerSwitchCount < layerSpd) {
							layerSwitchCount++;
							if (i == 0) {
								color.a = 1.0f - (layerSwitchCount / (float)layerSpd);
							}
							if (i == 5) {
								color.a = layerSwitchCount / (float)layerSpd;
							}
						}
						else {
							layerSwitchCount = 0;
						}

						XMMATRIX world = XMMatrixIdentity();
						world *= XMMatrixTranslation(0.0f, 0.0f, 10.0f * (i - 1) - layerSwitchCount * 10.0f / (float)layerSpd);
						SHADER.setWorld(world);
						SHADER.setView({ _eye.x, _eye.y, _eye.z - frame * 0.5f }, { 0.0f, 0.0f, 22.5f });
						SHADER.setProjection(SHADER.getPerspectiveMatrix());
						SHADER.setMatrix();

						DX3D.setBlendMode(BlendMode::NORMAL);
						DX3D.setDepthEnable(true);
						SHADER.setPS(PS::GENERAL);
						SPRITE.drawSprite3D({ 1280.0f, 720.0f }, _layerTex->getTex(), color);
					}

					Float4 coverColor = Color::black;
					coverColor.a = frame / 300.0f;
					SHADER.setPS(PS::NO_TEX);
					SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, coverColor);
					break;
			}
		}

		bool isEnd() override {
			return _isEnd;
		}

	private:
		RenderTexture* _screenTex;
		RenderTexture* _layerTex;
		IGameEventHandler* _gameEvent;
		Float2 _size = { 1280.0f, 720.0f };
		Float2 _pos = { 0.0f, 0.0f };
		Float2 _vel = { 0.0f, 0.0f };;
		Float3 _eye = { 0.0f, 0.0f, 0.0f };
		int time = 0;
		int frame = 0;
		int eyeTransformCount = 0;
		int layerSwitchCount = 0;
		unsigned int _frameTex;
		int phase = 0;
		bool _isEnd = false;
};