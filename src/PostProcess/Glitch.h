#pragma once

#include "RenderTexture.h"
#include "./DirectX/DirectX.h"
#include "./Render/Shader.h"
#include "./Render/Sprite.h"
#include "./Common/Color.h"

class Glitch {
	public:
		Glitch(RenderTexture* tex) : _texture(tex) {
			D3D11_BUFFER_DESC desc = {};
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.ByteWidth = sizeof(PixelTime);
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;
			desc.CPUAccessFlags = 0;
			DX3D.getDevice()->CreateBuffer(&desc, NULL, &_pixelTimeBuffer);
		}
		
		void update() {
			pt.time = (pt.time + 1) % 1800;
			DX3D.getDeviceContext()->UpdateSubresource(_pixelTimeBuffer, 0, NULL, &pt, 0, 0);
		}

		void draw() {
			DX3D.setTargetView();
			DX3D.getDeviceContext()->PSSetConstantBuffers(1, 1, &_pixelTimeBuffer);
			SHADER.setPS(PS::GLITCH);
			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, _texture->getTex(), Color::white);
		}

	private:
		ID3D11Buffer* _pixelTimeBuffer = nullptr;
		RenderTexture* _texture;
		PixelTime pt;
};