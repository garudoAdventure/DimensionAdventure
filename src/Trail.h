#pragma once

#include "Camera.h"
#include "./DirectX/DirectX.h"
#include "./Render/Shader.h"
#include "./Render/Texture.h"
#include "./Common/MathStruct.h"
#include <array>

class Trail {
public:
	static constexpr int TRAIL_LEN = 5;

	Trail(Float3 initPos) {
		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vertex) * (TRAIL_LEN * 2 - 2);
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		DX3D.getDevice()->CreateBuffer(&desc, NULL, &vertexBuffer);

		tex = TEXTURE.loadTexture("./assets/rainbow.png");

		for (int i = 0; i < TRAIL_LEN; i++) {
			trailPos.at(i) = { initPos.x, initPos.y, initPos.z };
		}
	}

	~Trail() {
		SAFE_RELEASE(vertexBuffer);
	}

	void update() {
		std::array<Vertex, TRAIL_LEN * 2 - 2> v;
		Float3 cameraPos = { 0.0f, 0.0f, -30.0f };
		for (int i = 0; i < TRAIL_LEN - 1; i++) {
			const XMFLOAT3 cameraVec = {
				cameraPos.x - trailPos.at(i).x,
				cameraPos.y - trailPos.at(i).y,
				cameraPos.z - trailPos.at(i).z,
			};
			const XMFLOAT3 trailVec = {
				trailPos.at(i + 1).x - trailPos.at(i).x,
				trailPos.at(i + 1).y - trailPos.at(i).y,
				trailPos.at(i + 1).z - trailPos.at(i).z,
			};
			XMFLOAT3 crossVec = {
				cameraVec.y * trailVec.z - cameraVec.z * trailVec.y,
				cameraVec.z * trailVec.x - cameraVec.x * trailVec.z,
				cameraVec.x * trailVec.y - cameraVec.y * trailVec.x,
			};
			const float crossVecLen = sqrtf(
				crossVec.x * crossVec.x + crossVec.y * crossVec.y + crossVec.z * crossVec.z
			);
			crossVec.x /= crossVecLen;
			crossVec.y /= crossVecLen;
			crossVec.z /= crossVecLen;

			v.at(i * 2 + 0).postion = {
				trailPos.at(i).x + crossVec.x * 0.01f,
				trailPos.at(i).y + crossVec.y * 0.01f,
				trailPos.at(i).z + crossVec.z * 0.01f
			};
			v.at(i * 2 + 1).postion = {
				trailPos.at(i).x - crossVec.x * 0.01f,
				trailPos.at(i).y - crossVec.y * 0.01f,
				trailPos.at(i).z - crossVec.z * 0.01f
			};
			v.at(i * 2 + 0).texCoord = { ((i + frame * 2) % 100) * 0.01f, 0.6f };
			v.at(i * 2 + 1).texCoord = { ((i + frame * 2) % 100) * 0.01f, 0.4f };

			v.at(i * 2 + 0).normal = { 0.0f, 0.0f, -1.0f };
			v.at(i * 2 + 1).normal = { 0.0f, 0.0f, -1.0f };

			v.at(i * 2 + 0).color = { 1.0f, 1.0f, 1.0f, 1.0f };
			v.at(i * 2 + 1).color = { 1.0f, 1.0f, 1.0f, 1.0f };
		}
		DX3D.getDeviceContext()->UpdateSubresource(vertexBuffer, 0, NULL, &v, 0, 0);
		frame++;
	}

	void draw() {
		ID3D11ShaderResourceView* texture = TEXTURE.getTexture(tex);
		DX3D.getDeviceContext()->PSSetShaderResources(0, 1, &texture);

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		DX3D.getDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		DX3D.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		Light light;
		light.enable = false;
		SHADER.setLight(light);

		XMVECTOR eye = XMVectorSet(0.0f, 0.0f, -30.0f, 0.0f);
		XMVECTOR focus = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMMATRIX view = XMMatrixLookAtLH(eye, focus, up);

		Transpose matrix;
		matrix.world = XMMatrixIdentity();
		matrix.view = view;
		matrix.projection = SHADER.getPerspectiveMatrix();
		SHADER.setMatrix(matrix);

		DX3D.setDepthEnable(false);

		DX3D.getDeviceContext()->Draw(TRAIL_LEN * 2 - 2, 0);
	}

	void setPos(Float3 pos) {
		for (int i = 0; i < TRAIL_LEN - 1; i++) {
			trailPos.at(i) = trailPos.at(i + 1);
		}
		trailPos.at(TRAIL_LEN - 1) = { pos.x, pos.y, pos.z };
	}

	void resetPos(Float3 pos) {
		for (int i = 0; i < TRAIL_LEN; i++) {
			trailPos.at(i) = { pos.x, pos.y, pos.z };
		}
	}

	private:
		unsigned int tex;
		std::array<XMFLOAT3, TRAIL_LEN> trailPos;
		ID3D11Buffer* vertexBuffer;
		int frame = 0;
};