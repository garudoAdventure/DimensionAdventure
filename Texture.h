#pragma once

#include "DirectX.h"
#include "DirectXTex.h"
using namespace DirectX;
#include <string>

struct TextureData {
  std::string filename;
  ID3D11ShaderResourceView* srv;
  int width;
  int height;
};

class Texture {
  public:
    int loadTexture(const std::string& fileName);
    void setTexture(int _texId);
    void setTexture(ID3D11ShaderResourceView* tex);
    ID3D11ShaderResourceView* getTexture(int texID);

  private:
    TextureData textureData[256];
    int textureDataCount = 0;
    ID3D11Buffer* hasTexBuffer = nullptr;
    bool _hasTex = false;

  private:
    static inline Texture* s_instance;
    Texture();

  public:
    static void CreateInstance() {
      DeleteInstance();
      s_instance = new Texture();
    }

    static void DeleteInstance() {
      if (s_instance != nullptr) {
        delete s_instance;
        s_instance = nullptr;
      }
    }

    static Texture& GetInstance() {
      return *s_instance;
    }
};

#define TEXTURE Texture::GetInstance()