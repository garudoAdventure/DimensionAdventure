#pragma once

#include "Texture.h"
#include "DirectX.h"
#include "MathStruct.h"

class RenderTexture {
	public:
    RenderTexture(float w, float h, Float4 clearColor = {0.0f, 0.0f, 0.0f, 0.0f}) : width(w), height(h), _clearColor(clearColor) {
      D3D11_TEXTURE2D_DESC txDesc = {};
      txDesc.Width = width;
      txDesc.Height = height;
      txDesc.MipLevels = 1;
      txDesc.ArraySize = 1;
      txDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
      txDesc.SampleDesc.Count = 1;
      txDesc.SampleDesc.Quality = 0;
      txDesc.Usage = D3D11_USAGE_DEFAULT;
      txDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
      txDesc.CPUAccessFlags = 0;
      txDesc.MiscFlags = 0;
      DX3D.getDevice()->CreateTexture2D(&txDesc, NULL, &_renderTargetTex);

      D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
      rtvDesc.Format = txDesc.Format;
      rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
      rtvDesc.Texture2D.MipSlice = 0;
      DX3D.getDevice()->CreateRenderTargetView(_renderTargetTex, &rtvDesc, &_renderTargetView);

      D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
      srvDesc.Format = txDesc.Format;
      srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
      srvDesc.Texture2D.MostDetailedMip = 0;
      srvDesc.Texture2D.MipLevels = 1;
      DX3D.getDevice()->CreateShaderResourceView(_renderTargetTex, &srvDesc, &_shaderResourceView);

      txDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
      txDesc.Usage = D3D11_USAGE_DEFAULT;
      txDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
      DX3D.getDevice()->CreateTexture2D(&txDesc, NULL, &_depthStencilTex);

      D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
      dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
      dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
      dsvDesc.Flags = 0;
      DX3D.getDevice()->CreateDepthStencilView(_depthStencilTex, &dsvDesc, &_depthStencilView);
		}
    void setTargetView() {
      DX3D.getDeviceContext()->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
    }
    ~RenderTexture() {
      SAFE_RELEASE(_renderTargetTex);
      SAFE_RELEASE(_depthStencilTex);
      SAFE_RELEASE(_renderTargetView);
      SAFE_RELEASE(_shaderResourceView);
      SAFE_RELEASE(_depthStencilView);
    }
    void clear() {
      float clearColor[4] = { _clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a };
      DX3D.getDeviceContext()->ClearRenderTargetView(_renderTargetView, clearColor);
      DX3D.getDeviceContext()->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }
    ID3D11ShaderResourceView* getTex() {
      return _shaderResourceView;
    }

	private:
    float width;
    float height;
    Float4 _clearColor;
    ID3D11Texture2D* _renderTargetTex;
    ID3D11Texture2D* _depthStencilTex;
    ID3D11RenderTargetView* _renderTargetView;
    ID3D11ShaderResourceView* _shaderResourceView;
    ID3D11DepthStencilView* _depthStencilView;
};