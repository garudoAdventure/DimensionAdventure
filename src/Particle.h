#pragma once

#include "./Render/Shader.h"
#include "./Render/Texture.h"
#include "./Render/Sprite.h"
#include "./Common/MathStruct.h"
#include "./Common/Color.h"
#include "./Utils/MathTool.h"
#include <vector>

class Particle {
	public:
		static constexpr int LIFE_TIME = 300;

		Particle(Float3 pos) {
			_tex = TEXTURE.loadTexture("./assets/elf.png");
			_pos = pos;
			_vel.x = sinf(rand());
			_vel.y = sinf(rand());
			_vel.z = sinf(rand());
		}

		void update() {
			_pos += _vel * 0.02f;
			_color = MathTool::lerp(Color::weakGray, Color::yellow, (float)_lifeTime / LIFE_TIME);
			_lifeTime--;
		}

		void draw() {
			XMMATRIX world = XMMatrixIdentity();
			world *= SHADER.getInverseView();
			world *= XMMatrixTranslation(_pos.x, _pos.y, _pos.z);
			SHADER.setWorld(world);
			SHADER.setMatrix();
			SPRITE.drawSprite3D({ 16.0f, 16.0f }, TEXTURE.getTexture(_tex), _color);
		}

		int getLifeTime() {
			return _lifeTime;
		}

	private:
		unsigned int _tex;
		Float3 _pos{ 0.0f, 0.0f, 0.0f };
		Float3 _vel{ 0.0f, 0.0f, 0.0f };;
		Float4 _color = Color::yellow;
		int _lifeTime = LIFE_TIME;
};

class Emitter {
	public:
		Emitter() {
			_tex = TEXTURE.loadTexture("./assets/elf.png");
			_particles.reserve(150);
		}

		~Emitter() {
			for (Particle* particle : _particles) {
				delete particle;
			}
			_particles.clear();
		}

		void update(Float3 pos) {
			_pos = pos;
			for (int i = 0; i < _particles.size(); i++){
				Particle* particle = _particles.at(i);
				particle->update();
				if (particle->getLifeTime() == 0) {
					delete particle;
					_particles.erase(_particles.begin() + i);
				}
			}
			if (_time % 20 == 0) {
				_particles.emplace_back(new Particle(_pos));
			}
			_color = MathTool::lerp<Float4>(Color::gray, Color::white, (sinf(_time * 0.05f) + 1) * 0.5f);
			_time++;
		}

		void draw(Float3 pos) {
			SHADER.begin();
			XMMATRIX world = XMMatrixIdentity();
			world *= SHADER.getInverseView();
			world *= XMMatrixTranslation(pos.x, pos.y, pos.z);
			SHADER.setWorld(world);
			SHADER.setMatrix();
			SPRITE.drawSprite3D({ 64.0f, 64.0f }, TEXTURE.getTexture(_tex), _color);

			for (Particle* particle : _particles) {
				particle->draw();
			}
		}

	private:
		unsigned int _tex;
		Float3 _pos{ 0.0f, 0.0f, 0.0f };
		Float4 _color = Color::white;
		std::vector<Particle*> _particles;
		int _time = 0;
};