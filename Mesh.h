#pragma once

#include <assimp/scene.h>
#include <d3d11.h>
#include "DirectX.h"
#include "MathStruct.h"
#include <vector>
#include <unordered_map>

class Mesh {
  public:
		Mesh(aiMesh* aiMesh, aiMaterial** aiMaterials);
		void update();
		void draw(Float3 pos, Float3 radian);
		bool updateBoneTransform(aiNode* node, aiMatrix4x4 globalTransform);
		void updateNodeTransform(aiMatrix4x4 globalTransform);
		void updateColor(Float4 color);
		Float3 getMinVertex() {
			return minVertex;
		}
		Float3 getMaxVertex() {
			return maxVertex;
		}

  private:
		ID3D11Buffer* _vertexBuffer = nullptr;
		ID3D11Buffer* _indexBuffer = nullptr;
		ID3D11Buffer* _boneBuffer = nullptr;
		std::vector<Vertex> _vertexData;
		std::vector<WORD> _indexData;
		std::vector<aiMatrix4x4> _boneOffset;
		std::vector<XMFLOAT4X4> _boneTransform;
		std::unordered_map<std::string, int> _boneNameToID;
		aiMatrix4x4 nodeTransform = aiMatrix4x4();
		Float3 maxVertex = { -999.0f, -999.0f, -999.0f };
		Float3 minVertex = { 999.0f, 999.0f, 999.0f };
};