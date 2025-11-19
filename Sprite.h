#pragma once

#include "MathStruct.h"

class Sprite {
  public:
		void drawTextureSprite(Float3 pos, Float2 size, float alpha);
		void drawSprite2D(Float2 pos, Float2 size, Float4 color);
		void drawSprite2D(Float2 pos, Float2 size, int texID);
		void drawSprite2DRotate(Float2 pos, Float2 size, int texID, float radian, Float2 center);
		void drawSprite2DUV(Float2 pos, Float2 size, int texID, float scale);
		void drawSpriteIn3D(Float3 pos, Float2 size, int texID);

  private:
		ID3D11Buffer* _vertexBuffer;

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