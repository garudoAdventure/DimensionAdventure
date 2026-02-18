#pragma once

#include "Texture.h"
#include "./Common/MathStruct.h"

class Sprite {
  public:
		void drawSprite2D(Float2 pos, Float2 size, Float4 color);
		void drawSprite2D(Float2 pos, Float2 size, ID3D11ShaderResourceView* tex, Float4 color, float width = 1280.0f, float height = 720.0f);
		void drawSprite2DUV(Float2 pos, Float2 size, ID3D11ShaderResourceView* tex, Float4 color, Float2 uv, Float2 uvSize);
		void drawSprite2DRotate(Float2 pos, Float2 size, ID3D11ShaderResourceView* tex, float radian, Float2 center, Float4 color = {1.0f, 1.0f, 1.0f, 1.0f});
		void drawSceneCover(Float2 pos, ID3D11ShaderResourceView* tex, float scale);
		void drawSprite3D(Float2 size, ID3D11ShaderResourceView* tex, Float4 color);

  private:
		ID3D11Buffer* _vertexBuffer;
		void draw();

  public:
		static void CreateInstance() {
			DeleteInstance();
			s_instance = new Sprite();
		}
		static void DeleteInstance() {
			delete s_instance;
			s_instance = nullptr;
		}
		static Sprite& GetInstance() {
			return *s_instance;
		}
  private:
		Sprite();
		~Sprite();
		static inline Sprite* s_instance;
};

#define SPRITE Sprite::GetInstance()