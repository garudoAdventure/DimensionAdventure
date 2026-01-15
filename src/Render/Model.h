#pragma once

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "./DirectX/DirectX.h"
#include "./Common/MathStruct.h"
#include <string>
#include <vector>
#include <unordered_map>

struct AnimData {
	std::vector<int> time;
	std::vector<aiMatrix4x4> animTransformation;
};

class Model {
  public:
		Model(const std::string fileName);
		~Model();
		void update();
		void draw(Float3 pos, Float3 radian, Float3 scale = {1.0f, 1.0f, 1.0f});
		void loadAnim(int animIdx);
		void updateNode(int frame, aiNode* node, aiMatrix4x4 parentTransform);
		void updateColor(Float4 color);
		Float3 getSize() {
			return _size;
		}

  private:
		const aiScene* _aiScene = nullptr;
		aiNode* _rootNode;
		aiMatrix4x4 _globalInverseTransform;
		std::vector<Mesh*> _meshes;
		std::unordered_map<std::string, ID3D11ShaderResourceView*> _texture;
		Float3 _size = { 0.0f, 0.0f, 0.0f };

		std::vector<aiAnimation*> _animations;
		std::unordered_map<std::string, AnimData> _animData;
		int _frame = 0;
		int _animDuration = 1;
};