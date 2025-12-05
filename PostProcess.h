#pragma once

#include "DirectX.h"
#include "RenderTexture.h"
#include "Texture.h"
#include "Shader.h"
#include "Sprite.h"
#include "Model.h"
#include "Player.h"

struct PixelConst {
	BOOL isVertical;
	float width;
	float height;
	float dummy3;
};

class PostProcess {
	public:
		PostProcess(RenderTexture* offscreenTex) : _offscreenTex(offscreenTex) {
      _offscreenCopyTex = new RenderTexture(1280, 720);
      _luminanceTex = new RenderTexture(1280, 720);
      for (int i = 0; i < 5; i++) {
        Float4 clearColor = { 0.0f, 0.0f, 0.0f, 0.0f };
        blurVTex[i] = new RenderTexture(1280 >> i, 720 >> i);
        blurTex[i] = new RenderTexture(1280 >> i, 720 >> i);
			}

			D3D11_BUFFER_DESC desc = {};
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.ByteWidth = sizeof(PixelConst);
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;
			desc.CPUAccessFlags = 0;
			DX3D.getDevice()->CreateBuffer(&desc, NULL, &pixelConstBuffer);
		}

		void update() {      
      SHADER.begin();
      _offscreenCopyTex->setTargetView();
      _offscreenCopyTex->clear();
      SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, _offscreenTex->getTex());

      //_luminanceTex->setTargetView();
      //_luminanceTex->clear();
      //SHADER.setPS(PS::LUMINANCE);
      //SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, _offscreenCopyTex->getTex());

      DX3D.setBlendMode(BlendMode::NORMAL);
      SHADER.setPS(PS::BLUR);
      SHADER.setSamplerState(SamplerState::CLAMP);
      PixelConst pc;
      for (int i = 0; i < 5; i++) {
        blurVTex[i]->setTargetView();
        blurVTex[i]->clear();
        pc.isVertical = true;
        pc.width = 1280 >> i;
        pc.height = 720 >> i;
        DX3D.setViewport(pc.width, pc.height);
        DX3D.getDeviceContext()->UpdateSubresource(pixelConstBuffer, 0, NULL, &pc, 0, 0);
        DX3D.getDeviceContext()->PSSetConstantBuffers(1, 1, &pixelConstBuffer);
        SPRITE.drawSprite2D(
          { 0.0f, 0.0f }, { pc.width, pc.height },
          (i == 0 ? _offscreenCopyTex->getTex() : blurTex[i - 1]->getTex()),
          pc.width, pc.height
        );

        blurTex[i]->setTargetView();
        blurTex[i]->clear();
        pc.isVertical = false;
        DX3D.getDeviceContext()->UpdateSubresource(pixelConstBuffer, 0, NULL, &pc, 0, 0);
        DX3D.getDeviceContext()->PSSetConstantBuffers(1, 1, &pixelConstBuffer);
        SPRITE.drawSprite2D({ 0.0f, 0.0f }, { pc.width, pc.height }, blurVTex[i]->getTex(), pc.width, pc.height);
      }
      SHADER.setSamplerState(SamplerState::WRAP);
		}

    void draw() {
      DX3D.setBlendMode(BlendMode::ADD_ALPHA);
      // SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, blurTex[4]->getTex());
      // SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, blurTex[3]->getTex());
      SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, blurTex[2]->getTex());
      SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, blurTex[1]->getTex());
      SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, blurTex[0]->getTex());
      DX3D.setBlendMode(BlendMode::NORMAL);
    }

	private:
		ID3D11Buffer* pixelConstBuffer;
    RenderTexture* _offscreenTex;
    RenderTexture* _offscreenCopyTex;
    RenderTexture* _luminanceTex;
		RenderTexture* blurVTex[5];
		RenderTexture* blurTex[5];
};