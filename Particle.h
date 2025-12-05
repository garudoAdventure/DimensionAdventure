#pragma once

#include "MathStruct.h"
#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"
#include "MathTool.h"
#include "Color.h"
#include <vector>

class Particle {
	public:
		Particle(Float3 pos) {
			tex = TEXTURE.loadTexture("./assets/elf.png");
			_pos = pos;
			vel.x = sinf(rand());
			vel.y = sinf(rand());
			vel.z = sinf(rand());
		}
		void update() {
			_pos += vel * 0.02f;
			color = MathTool::lerp(Color::weakGray, Color::yellow, lifeTime / 300.0f);
			lifeTime--;
		}
		void draw() {
			XMMATRIX world = XMMatrixIdentity();
			world *= SHADER.getInverseView();
			world *= XMMatrixTranslation(_pos.x, _pos.y, _pos.z);
			SHADER.setWorldMatrix(world);
			SPRITE.drawSpriteIn3D({ 0.0f, 0.0f, 0.0f }, { 16.0f, 16.0f }, tex, color);
		}
		int getLifeTime() {
			return lifeTime;
		}

	private:
		unsigned int tex;
		Float3 _pos = { 0.0f, 0.0f, 0.0f };
		Float3 vel;
		Float3 size = { 0.5f, 0.5f, 0.5f };
		Float4 color = Color::yellow;
		int lifeTime = 300;
};

class Emitter {
	public:
		Emitter() {
			tex = TEXTURE.loadTexture("./assets/elf.png");
		}
		void update(Float3 pos) {
			_pos = pos;
			for (int i = 0; i < particles.size(); i++){
				Particle* particle = particles[i];
				particle->update();
				if (particle->getLifeTime() == 0) {
					delete particle;
					particles.erase(particles.begin() + i);
				}
			}
			if (time % 20 == 0) {
				particles.emplace_back(new Particle(_pos));
			}
			_color = MathTool::lerp<Float4>(Color::gray, Color::white, (sinf(time * 0.05f) + 1) * 0.5f);
			time++;
		}
		void draw(Float3 pos) {
			SHADER.begin();
			XMMATRIX world = XMMatrixIdentity();
			world *= SHADER.getInverseView();
			world *= XMMatrixTranslation(pos.x, pos.y, pos.z);
			SHADER.setWorldMatrix(world);
			SPRITE.drawSpriteIn3D({ 0.0f, 0.0f, 0.0f }, { 64.0f, 64.0f }, tex, _color);

			for (Particle* particle : particles) {
				particle->draw();
			}
		}

	private:
		Float3 _pos = { 0.0f, 0.0f, 0.0f };
		Float4 _color = Color::white;
		unsigned int tex;
		std::vector<Particle*> particles;
		int time = 0;
};