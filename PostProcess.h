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
      _offscreenCopyTex = new RenderTexture(1280, 720, {0.3f, 0.3f, 0.3f, 0.1f});
      _luminanceTex = new RenderTexture(1280, 720);
      for (int i = 0; i < 4; i++) {
        Float4 color = { 0.54f, 1.0f, 0.43f, 0.0f };
        blurVTex[i] = new RenderTexture(1280 >> i, 720 >> i, color);
        blurTex[i] = new RenderTexture(1280 >> i, 720 >> i, color);
			}

			D3D11_BUFFER_DESC desc = {};
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.ByteWidth = sizeof(PixelConst);
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;
			desc.CPUAccessFlags = 0;
			DX3D.getDevice()->CreateBuffer(&desc, NULL, &pixelConstBuffer);

      circleTex = TEXTURE.loadTexture("./assets/circleTex.png");
      bgTex = TEXTURE.loadTexture("./assets/bg1.png");
      itemTex = TEXTURE.loadTexture("./assets/south.png");
		}

		void update() {
      //SHADER.begin();
      //_offscreenCopyTex->setTargetView();
      //_offscreenCopyTex->clear();
      //// Reference: https://stackoverflow.com/questions/27929483/directx-render-to-texture-alpha-blending
      //DX3D.setBlendMode(BlendMode::REND_TEX);
      //SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, _offscreenTex->getTex());
      //PLAYER.drawCircle();
      //DX3D.setBlendMode(BlendMode::NORMAL);

      _luminanceTex->setTargetView();
      _luminanceTex->clear();
      SHADER.setPS(PS::LUMINANCE);
      SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, _offscreenTex->getTex());

      SHADER.setPS(PS::BLUR);
      SHADER.setSamplerState(SamplerState::CLAMP);
      PixelConst pc;
      for (int i = 0; i < 3; i++) {
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
          (i == 0 ? _luminanceTex->getTex() : blurTex[i - 1]->getTex()),
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
      DX3D.setTargetView();
      DX3D.clear();
      DX3D.setViewport(1280.0f, 720.0f);
      SHADER.begin();

      DX3D.setBlendMode(BlendMode::ADD_ALPHA);
      SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, blurTex[2]->getTex());
      SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, blurTex[1]->getTex());
      SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, blurTex[0]->getTex());
      // SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, _offscreenTex->getTex());
      // SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, _luminanceTex->getTex());
      DX3D.setBlendMode(BlendMode::NORMAL);
    }

	private:
    unsigned int circleTex;
    unsigned int bgTex;
    unsigned int itemTex;
		ID3D11Buffer* pixelConstBuffer;
    RenderTexture* _offscreenTex;
    RenderTexture* _offscreenCopyTex;
    RenderTexture* _luminanceTex;
		RenderTexture* blurVTex[4];
		RenderTexture* blurTex[4];
};