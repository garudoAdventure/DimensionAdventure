#pragma once

#include "RenderTexture.h"
#include "./DirectX/DirectX.h"
#include "./Render/Shader.h"
#include "./Render/Sprite.h"

struct PixelConst {
	BOOL isVertical;
	float width;
	float height;
	int bloomPower;
};

class PostProcess {
	public:
		PostProcess(RenderTexture* offscreenTex) : _offscreenTex(offscreenTex) {
      _offscreenCopyTex = new RenderTexture(1280, 720);
      for (int i = 0; i < 5; i++) {
        _blurVTex[i] = new RenderTexture(1280 >> i, 720 >> i);
        _blurTex[i] = new RenderTexture(1280 >> i, 720 >> i);
			}

			D3D11_BUFFER_DESC desc = {};
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.ByteWidth = sizeof(PixelConst);
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;
			desc.CPUAccessFlags = 0;
			DX3D.getDevice()->CreateBuffer(&desc, NULL, &_pixelConstBuffer);
		}

    ~PostProcess() {
      SAFE_RELEASE(_pixelConstBuffer);
      delete _offscreenCopyTex;
      for (int i = 0; i < 5; i++) {
        delete _blurVTex[i];
        delete _blurTex[i];
      }
    }

		void update(bool clipLuminance) {
      SHADER.begin();
      
      if (clipLuminance) {
        SHADER.setPS(PS::LUMINANCE);
      }
      else {
        SHADER.setPS(PS::GENERAL);
      }
      _offscreenCopyTex->setTargetView();
      _offscreenCopyTex->clear();
      SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, _offscreenTex->getTex(), Color::white);
      
      DX3D.setBlendMode(BlendMode::NORMAL);
      SHADER.setPS(PS::BLUR);
      SHADER.setSamplerState(SamplerState::CLAMP);
      PixelConst pc;
      for (int i = 0; i < 5; i++) {
        _blurVTex[i]->setTargetView();
        _blurVTex[i]->clear();
        pc.isVertical = true;
        pc.width = 1280 >> i;
        pc.height = 720 >> i;
        DX3D.setViewport(pc.width, pc.height);
        DX3D.getDeviceContext()->UpdateSubresource(_pixelConstBuffer, 0, NULL, &pc, 0, 0);
        DX3D.getDeviceContext()->PSSetConstantBuffers(0, 1, &_pixelConstBuffer);
        SPRITE.drawSprite2D(
          { 0.0f, 0.0f }, { pc.width, pc.height },
          (i == 0 ? _offscreenCopyTex->getTex() : _blurTex[i - 1]->getTex()),
          Color::white,
          pc.width, pc.height
        );

        _blurTex[i]->setTargetView();
        _blurTex[i]->clear();
        pc.isVertical = false;
        DX3D.getDeviceContext()->UpdateSubresource(_pixelConstBuffer, 0, NULL, &pc, 0, 0);
        DX3D.getDeviceContext()->PSSetConstantBuffers(0, 1, &_pixelConstBuffer);
        SPRITE.drawSprite2D({ 0.0f, 0.0f }, { pc.width, pc.height }, _blurVTex[i]->getTex(), Color::white, pc.width, pc.height);
      }
      DX3D.setViewport(1280.0f, 720.0f);
      SHADER.setSamplerState(SamplerState::WRAP);
		}

    void drawBloom(int bloomPower = 3) {
      bloomPower = MathTool::clamp(bloomPower, 1, 5);

      PixelConst pc;
      pc.bloomPower = bloomPower;
      DX3D.getDeviceContext()->UpdateSubresource(_pixelConstBuffer, 0, NULL, &pc, 0, 0);
      DX3D.getDeviceContext()->PSSetConstantBuffers(0, 1, &_pixelConstBuffer);

      DX3D.setBlendMode(BlendMode::REND_TEX);
      SHADER.setPS(PS::BLOOM);
      for (int i = 0; i < bloomPower; i++) {
        ID3D11ShaderResourceView* tex = _blurTex[i]->getTex();
        DX3D.getDeviceContext()->PSSetShaderResources(i, 1, &tex);
      }
      SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, Color::white);
      
      SHADER.setPS(PS::GENERAL);
      DX3D.setBlendMode(BlendMode::NORMAL);
    }

	private:
		ID3D11Buffer* _pixelConstBuffer;
    RenderTexture* _offscreenTex;
    RenderTexture* _offscreenCopyTex;
		RenderTexture* _blurVTex[5];
		RenderTexture* _blurTex[5];
};