#pragma once

#include "IGameEvent.h"
#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"
#include "MathTool.h"
#include "Camera.h"
#include <functional>

class ChangeFieldEvent : public IGameEvent {
	public:
		ChangeFieldEvent(Camera* camera, Float2 doorPos, Float3 playerInitPos, std::function<void()> changeFieldFn) :
			_camera(camera), _doorPos(doorPos), _playerInitPos(playerInitPos), _changeFieldFn(changeFieldFn)
		{
			_coverTex = TEXTURE.loadTexture("./assets/UI/cover.png");
		}
		~ChangeFieldEvent() {
		}
		void update() override {
			switch (_status) {
				case Status::FADE_OUT:
					if (_count == 60) {
						_status = Status::LOADING;
						_count = 0;
					}
					break;
				case Status::LOADING:
					if (_count == 30) {
						PLAYER.setPos(_playerInitPos);
						PLAYER.getSpirit()->setPos(_playerInitPos);
						_camera->set2DPos({ _playerInitPos.x, _playerInitPos.y });
						_doorPos = {
							_playerInitPos.x - _camera->get2DPos().x,
							_playerInitPos.y - _camera->get2DPos().y
						};
						_changeFieldFn();
						_status = Status::FADE_IN;
						_count = 0;
					}
					break;
				case Status::FADE_IN:
					if (_count == 35) {
						_isEnd = true;
						PLAYER.setState(new PlayerIdle());
					}
					break;
			}
			_count++;
		}
		void draw() override {
			SHADER.setSamplerState(SamplerState::CLAMP);
			float scale = 0.0f;
			switch (_status) {
				case Status::FADE_OUT:
					scale = MathTool::easeInQuad<float>(-1.6f, 30.0f, _count / 60.f);
					SPRITE.drawSceneCover(_doorPos, { 1280.0f, 1280.0f }, TEXTURE.getTexture(_coverTex), scale);
					break;
				case Status::LOADING:
					SHADER.setPS(PS::NO_TEX);
					SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 1280.0f }, { 0.0f, 0.0f, 0.0f, 1.0f });
					break;
				case Status::FADE_IN:
					scale = MathTool::easeOutQuad<float>(20.0f, -2.0f, _count / 45.0f);
					SPRITE.drawSceneCover(_doorPos, { 1280.0f, 1280.0f }, TEXTURE.getTexture(_coverTex), scale);
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
		std::function<void()> _changeFieldFn;
		
		enum Status {
			FADE_IN,
			LOADING,
			FADE_OUT
		};
		Status _status = Status::FADE_OUT;
};