#pragma once

#include "Shader.h"
#include "Sprite.h"
#include "Color.h"
#include "RenderTexture.h"

class MazeBg {
	public:
		MazeBg() {
			_noiseTex = new RenderTexture(1280, 720);
			_spiralTex = new RenderTexture(1280, 720);

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
			PixelTime pt;
			pt.time = _time;
			DX3D.getDeviceContext()->UpdateSubresource(_pixelTimeBuffer, 0, NULL, &pt, 0, 0);
			DX3D.getDeviceContext()->PSSetConstantBuffers(1, 1, &_pixelTimeBuffer);

			_noiseTex->setTargetView();
			_noiseTex->clear();
			SHADER.setPS(PS::NOISE);
			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, Color::white);

			_spiralTex->setTargetView();
			_spiralTex->clear();
			SHADER.setPS(PS::SPIRAL);
			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, _noiseTex->getTex(), Color::white);

			_time += 0.002f;
		}
		void draw(Float4 color) {
			SHADER.setPS(PS::GENERAL);
			SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, _spiralTex->getTex(), color);
		}

	private:
		RenderTexture* _noiseTex;
		RenderTexture* _spiralTex;
		ID3D11Buffer* _pixelTimeBuffer;
		float _time = 0;
};