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
		static constexpr int LIFE_TIME = 360;

		Particle(Float3 pos) : _pos(pos) {
			_tex = TEXTURE.loadTexture("./assets/elf.png");
			_vel.x = sinf(rand());
			_vel.y = sinf(rand());
			_vel.z = sinf(rand());
		}

		void update(Instance& instData) {
			_pos += _vel * 0.02f;
			_color = MathTool::lerp(Color::weakGray, Color::yellow, (float)_lifeTime / LIFE_TIME);
			_lifeTime--;

			XMMATRIX w = XMMatrixIdentity();
			w *= SHADER.getInverseView();
			w *= XMMatrixScaling(0.8f, 0.8f, 1.0f);
			w *= XMMatrixTranslation(_pos.x, _pos.y, _pos.z);
			instData.world = w;
			instData.color = { _color.r, _color.g, _color.b, _color.a };
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
		static constexpr int MAX_NUM = 512;

		Emitter() {
			{
				D3D11_BUFFER_DESC desc = {};
				desc.Usage = D3D11_USAGE_DEFAULT;
				desc.ByteWidth = sizeof(Vertex) * 4;
				desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				desc.CPUAccessFlags = 0;
				DX3D.getDevice()->CreateBuffer(&desc, NULL, &_vertexBuffer);

				std::array<Vertex, 4> v;
				v.at(0).postion = { -0.5f,  0.5f, 0.0f };
				v.at(1).postion = {  0.5f,  0.5f, 0.0f };
				v.at(2).postion = { -0.5f, -0.5f, 0.0f };
				v.at(3).postion = {  0.5f, -0.5f, 0.0f };

				v.at(0).texCoord = { 0.0f, 0.0f };
				v.at(1).texCoord = { 1.0f, 0.0f };
				v.at(2).texCoord = { 0.0f, 1.0f };
				v.at(3).texCoord = { 1.0f, 1.0f };

				for (int i = 0; i < 4; i++) {
					v.at(i).color = { 1.0f, 1.0f, 1.0f, 1.0f };
					v.at(i).normal = { 0.0f, 0.0f, -1.0f };
				}

				D3D11_SUBRESOURCE_DATA data = {};
				data.pSysMem = &v.at(0);
				data.SysMemPitch = 0;
				data.SysMemSlicePitch = 0;
				DX3D.getDevice()->CreateBuffer(&desc, &data, &_vertexBuffer);
			}
			{
				D3D11_BUFFER_DESC desc = {};
				desc.Usage = D3D11_USAGE_DEFAULT;
				desc.ByteWidth = sizeof(WORD) * 6;
				desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
				desc.CPUAccessFlags = 0;
				DX3D.getDevice()->CreateBuffer(&desc, NULL, &_indexBuffer);

				std::array<WORD, 6> indexData = { 0, 1, 2, 2, 1, 3 };
				D3D11_SUBRESOURCE_DATA data = {};
				data.pSysMem = &indexData.at(0);
				data.SysMemPitch = 0;
				data.SysMemSlicePitch = 0;
				DX3D.getDevice()->CreateBuffer(&desc, &data, &_indexBuffer);
			}
			{
				D3D11_BUFFER_DESC desc = {};
				desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				desc.ByteWidth = sizeof(Instance) * MAX_NUM;
				desc.Usage = D3D11_USAGE_DEFAULT;
				desc.CPUAccessFlags = 0;
				DX3D.getDevice()->CreateBuffer(&desc, NULL, &_instanceBuffer);
			}

			_tex = TEXTURE.loadTexture("./assets/elf.png");
			_particles.reserve(MAX_NUM);
			_instanceData.reserve(MAX_NUM);
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
				particle->update(_instanceData.at(i));
				if (particle->getLifeTime() == 0) {
					delete particle;
					_particles.erase(_particles.begin() + i);
					_instanceData.erase(_instanceData.begin() + i);
				}
			}
			if (_time % 10 == 0) {
				Instance ins;
				_instanceData.emplace_back(ins);
				_particles.emplace_back(new Particle(_pos));
			}
			_color = MathTool::lerp<Float4>(Color::gray, Color::white, (sinf(_time * 0.05f) + 1) * 0.5f);
			_time = (_time + 1) % 100;

			DX3D.getDeviceContext()->UpdateSubresource(_instanceBuffer, 0, NULL, &_instanceData[0], 0, 0);
		}

		void draw(Float3 pos) {
			SHADER.begin();
			XMMATRIX world = XMMatrixIdentity();
			world *= SHADER.getInverseView();
			world *= XMMatrixTranslation(pos.x, pos.y, pos.z);
			SHADER.setWorld(world);
			SHADER.setMatrix();
			SPRITE.drawSprite3D({ 64.0f, 64.0f }, TEXTURE.getTexture(_tex), _color);

			// パーティクル描画
			SHADER.setVS(VS::INSTANCE);
			{
				UINT stride = sizeof(Vertex);
				UINT offset = 0;
				DX3D.getDeviceContext()->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
			}
			{
				UINT stride = sizeof(Instance);
				UINT offset = 0;
				DX3D.getDeviceContext()->IASetVertexBuffers(2, 1, &_instanceBuffer, &stride, &offset);
			}

			DX3D.getDeviceContext()->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
			DX3D.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			DX3D.getDeviceContext()->DrawIndexedInstanced(6, _particles.size(), 0, 0, 0);
			SHADER.setVS(VS::GENERAL);
		}

	private:
		unsigned int _tex;
		std::vector<Particle*> _particles;
		std::vector<Instance> _instanceData;
		ID3D11Buffer* _instanceBuffer;
		ID3D11Buffer* _vertexBuffer;
		ID3D11Buffer* _indexBuffer;
		Float3 _pos{ 0.0f, 0.0f, 0.0f };
		Float4 _color = Color::white;
		int _time = 0;
};