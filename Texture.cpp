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

void Texture::setTexture(ID3D11ShaderResourceView* tex) {
  DX3D.getDeviceContext()->PSSetShaderResources(0, 1, &tex);
}

ID3D11ShaderResourceView* Texture::getTexture(int texID) {
  if (texID < 0 || texID >= textureDataCount) {
    return NULL;
  }
  return textureData[texID].srv;
}