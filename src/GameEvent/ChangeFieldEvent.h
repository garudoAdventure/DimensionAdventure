#pragma once

#include "IGameEvent.h"
#include "Camera.h"
#include "./Render/Shader.h"
#include "./Render/Texture.h"
#include "./Render/Sprite.h"
#include "./Utils/MathTool.h"
#include <functional>

class ChangeFieldEvent : public IGameEvent {
	public:
		static constexpr int FADE_OUT_TIME = 60;
		static constexpr int LOADING_TIME = 30;
		static constexpr int FADE_IN_TIME = 35;

		ChangeFieldEvent(Camera* camera, Float2 doorPos, Float3 playerInitPos, std::function<void()> changeFieldFn) :
			_camera(camera), _doorPos(doorPos), _playerInitPos(playerInitPos), _triggerChangeFieldFn(changeFieldFn)
		{
			_coverTex = TEXTURE.loadTexture("./assets/UI/cover.png");
		}

		void update() override {
			switch (_status) {
				case Status::FADE_OUT:
					if (_count == FADE_OUT_TIME) {
						_status = Status::LOADING;
						_count = 0;
					}
					break;
				case Status::LOADING:
					if (_count == LOADING_TIME) {
						_status = Status::FADE_IN;
						_count = 0;
						PLAYER.setPos(_playerInitPos);
						PLAYER.getSpirit()->setPos(_playerInitPos);
						_camera->set2DPos({ _playerInitPos.x, _playerInitPos.y });
						_doorPos = {
							_playerInitPos.x - _camera->get2DPos().x,
							_playerInitPos.y - _camera->get2DPos().y
						};
						_triggerChangeFieldFn();
					}
					break;
				case Status::FADE_IN:
					if (_count == FADE_IN_TIME) {
						_isEnd = true;
					}
					break;
			}
			_count++;
		}

		void draw() override {
			SHADER.setSamplerState(SamplerState::CLAMP);
			float coverScale = 0.0f;
			switch (_status) {
				case Status::FADE_OUT:
					coverScale = MathTool::easeInQuad<float>(-1.0f, 30.0f, _count / 60.f);
					SPRITE.drawSceneCover(_doorPos, TEXTURE.getTexture(_coverTex), coverScale);
					break;
				case Status::LOADING:
					SHADER.setPS(PS::NO_TEX);
					SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 1280.0f }, { 0.0f, 0.0f, 0.0f, 1.0f });
					break;
				case Status::FADE_IN:
					coverScale = MathTool::easeOutQuad<float>(20.0f, -1.0f, _count / 45.0f);
					SPRITE.drawSceneCover(_doorPos, TEXTURE.getTexture(_coverTex), coverScale);
					break;
			}
			SHADER.setSamplerState(SamplerState::WRAP);
		}

		bool isEnd() override {
			return _isEnd;
		}

	private:
		unsigned int _coverTex;
		Camera* _camera;
		Float2 _doorPos;
		Float3 _playerInitPos;
		int _count = 0;
		bool _isEnd = false;
		std::function<void()> _triggerChangeFieldFn;
		
		enum Status {
			FADE_IN,
			LOADING,
			FADE_OUT
		};
		Status _status = Status::FADE_OUT;
};