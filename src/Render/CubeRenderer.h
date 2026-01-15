#pragma once

#include "./Common/MathStruct.h"

class CubeRenderer {
  public:
		void drawCube(Cube cube, Float4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, int texID = -1);

  private:
		ID3D11Buffer* _vertexBuffer;
		ID3D11Buffer* _indexBuffer;
		Vertex _vertexData[24];
		WORD _indexData[36];

  public:
		static void CreateInstance() {
			DeleteInstance();
			s_instance = new CubeRenderer();
		}
		static void DeleteInstance() {
			delete s_instance;
			s_instance = nullptr;
		}
		static CubeRenderer& GetInstance() {
			return *s_instance;
		}

  private:
		CubeRenderer();
		~CubeRenderer();
		static inline CubeRenderer* s_instance;
};

#define CUBE_RENDERER CubeRenderer::GetInstance()