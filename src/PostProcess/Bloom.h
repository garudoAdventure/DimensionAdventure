#pragma once

#include "RenderTexture.h"
#include "./DirectX/DirectX.h"
#include "./Render/Shader.h"
#include "./Render/Sprite.h"
#include "./Common/Color.h"
#include "./Utils/MathTool.h"
#include <array>

struct PixelConst {
  BOOL isVertical;
  float width;
  float height;
  int bloomPower;
};

class Bloom {
  public:
    Bloom(RenderTexture* offscreenTex) : _offscreenTex(offscreenTex) {
      _offscreenCopyTex = new RenderTexture(1280, 720);
      for (int i = 0; i < 5; i++) {
        _blurVTex.at(i) = new RenderTexture(1280 >> i, 720 >> i);
        _blurTex.at(i) = new RenderTexture(1280 >> i, 720 >> i);
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

    ~Bloom() {
      SAFE_RELEASE(_pixelConstBuffer);
      delete _offscreenCopyTex;
      for (int i = 0; i < 5; i++) {
        delete _blurVTex.at(i);
        delete _blurTex.at(i);
      }
    }

    void update() {
      SHADER.begin();

      if (needClipLuminance) {
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
        _blurVTex.at(i)->setTargetView();
        _blurVTex.at(i)->clear();
        pc.isVertical = true;
        pc.width = 1280 >> i;
        pc.height = 720 >> i;
        DX3D.setViewport(pc.width, pc.height);
        DX3D.getDeviceContext()->UpdateSubresource(_pixelConstBuffer, 0, NULL, &pc, 0, 0);
        DX3D.getDeviceContext()->PSSetConstantBuffers(0, 1, &_pixelConstBuffer);
        SPRITE.drawSprite2D(
          { 0.0f, 0.0f }, { pc.width, pc.height },
          (i == 0 ? _offscreenCopyTex->getTex() : _blurTex.at(i - 1)->getTex()),
          Color::white,
          pc.width, pc.height
        );

        _blurTex.at(i)->setTargetView();
        _blurTex.at(i)->clear();
        pc.isVertical = false;
        DX3D.getDeviceContext()->UpdateSubresource(_pixelConstBuffer, 0, NULL, &pc, 0, 0);
        DX3D.getDeviceContext()->PSSetConstantBuffers(0, 1, &_pixelConstBuffer);
        SPRITE.drawSprite2D({ 0.0f, 0.0f }, { pc.width, pc.height }, _blurVTex.at(i)->getTex(), Color::white, pc.width, pc.height);
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
        ID3D11ShaderResourceView* tex = _blurTex.at(i)->getTex();
        DX3D.getDeviceContext()->PSSetShaderResources(i, 1, &tex);
      }
      SPRITE.drawSprite2D({ 0.0f, 0.0f }, { 1280.0f, 720.0f }, Color::white);

      SHADER.setPS(PS::GENERAL);
      DX3D.setBlendMode(BlendMode::NORMAL);
    }

    void setClipLuminance(bool clipLuminance) {
      needClipLuminance = clipLuminance;
    }

  private:
    ID3D11Buffer* _pixelConstBuffer;
    RenderTexture* _offscreenTex;
    RenderTexture* _offscreenCopyTex;
    std::array<RenderTexture*, 5> _blurVTex;
    std::array<RenderTexture*, 5> _blurTex;
    bool needClipLuminance = false;
};