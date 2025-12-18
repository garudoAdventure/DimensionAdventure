#pragma once

#include "Texture.h"

#if _DEBUG
#pragma comment(lib, "DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex_Release.lib")
#endif

Texture::Texture() {
  for (int i = 0; i < 256; i++) {
    textureData[i].srv = NULL;
  }
  textureDataCount = 0;

  D3D11_BUFFER_DESC hasTexCbDesc = {};
  hasTexCbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  hasTexCbDesc.ByteWidth = 16;
  hasTexCbDesc.Usage = D3D11_USAGE_DEFAULT;
  hasTexCbDesc.CPUAccessFlags = 0;
  hasTexCbDesc.MiscFlags = 0;
  hasTexCbDesc.StructureByteStride = 0;

  DX3D.getDevice()->CreateBuffer(&hasTexCbDesc, NULL, &hasTexBuffer);
}

int Texture::loadTexture(const std::string& fileName) {
  for (int i = 0; i < textureDataCount; i++) {
    if (textureData[i].filename == fileName) {
      return i;
    }
  }

  // テクスチャ読み込み
  wchar_t wFileName[256];
  size_t ret;
  mbstowcs_s(&ret, wFileName, fileName.c_str(), 256);

  TexMetadata metadata;
  ScratchImage image;
  LoadFromWICFile(wFileName, WIC_FLAGS_NONE, &metadata, image);
  CreateShaderResourceView(DX3D.getDevice(), image.GetImages(), image.GetImageCount(), metadata, &textureData[textureDataCount].srv);
  textureData[textureDataCount].width = (int)metadata.width;
  textureData[textureDataCount].height = (int)metadata.height;
  textureData[textureDataCount].filename = fileName;

  if (!textureData[textureDataCount].srv) {
    return -1;
  }

  return textureDataCount++;
}

void Texture::setTexture(int texId) {
  if (texId == -1) {
    _hasTex = false;
  } else {
    _hasTex = true;
    ID3D11ShaderResourceView* tex = TEXTURE.getTexture(texId);
    DX3D.getDeviceContext()->PSSetShaderResources(0, 1, &tex);
  }
  DX3D.getDeviceContext()->UpdateSubresource(hasTexBuffer, 0, NULL, &_hasTex, 0, 0);
  DX3D.getDeviceContext()->PSSetConstantBuffers(0, 1, &hasTexBuffer);
}

void Texture::setTexture(ID3D11ShaderResourceView* tex) {
  _hasTex = true;
  DX3D.getDeviceContext()->PSSetShaderResources(0, 1, &tex);
  DX3D.getDeviceContext()->UpdateSubresource(hasTexBuffer, 0, NULL, &_hasTex, 0, 0);
  DX3D.getDeviceContext()->PSSetConstantBuffers(0, 1, &hasTexBuffer);
}

ID3D11ShaderResourceView* Texture::getTexture(int texID) {
  if (texID < 0 || texID >= textureDataCount) {
    return NULL;
  }
  return textureData[texID].srv;
}