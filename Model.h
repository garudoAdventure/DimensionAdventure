#pragma once

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "DirectX.h"
#include "MathStruct.h"
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
#include <string>
#include <vector>
#include <unordered_map>

class Model {
  public:
		Model(const char* fileName) {
			_pScene = aiImportFile(fileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);

			_meshes.reserve(_pScene->mNumMeshes);
			Float3 maxVertex = { -999.0f, -999.0f, -999.0f };
			Float3 minVertex = { 999.0f, 999.0f, 999.0f };
			for (int i = 0; i < _pScene->mNumMeshes; i++) {
				aiMesh* pMesh = _pScene->mMeshes[i];
				Mesh* mesh = new Mesh(pMesh);
				_meshes.emplace_back(mesh);

				Float3 meshMinVertex = mesh->getMinVertex();
				Float3 meshMaxVertex = mesh->getMaxVertex();
				if (meshMinVertex.x <= minVertex.x && meshMinVertex.y <= minVertex.y && meshMinVertex.z <= minVertex.z) {
					minVertex = meshMinVertex;
				}
				if (meshMaxVertex.x >= maxVertex.x && meshMaxVertex.y >= maxVertex.y && meshMaxVertex.z >= maxVertex.z) {
					maxVertex = meshMaxVertex;
				}
			}
			_size = maxVertex - minVertex;

			for (int i = 0; i < _pScene->mNumTextures; i++) {
				aiTexture* pTexture = _pScene->mTextures[i];

				ID3D11ShaderResourceView* texture;
				TexMetadata metadata;
				ScratchImage image;
				LoadFromWICMemory(pTexture->pcData, pTexture->mWidth , WIC_FLAGS_NONE, &metadata, image);
				CreateShaderResourceView(DX3D.getDevice(), image.GetImages(), image.GetImageCount(), metadata, &texture);

				_texture[pTexture->mFilename.data] = texture;
			}
		}
		~Model() {
			for (Mesh* mesh : _meshes) {
				delete mesh;
			}
		}
		void update() {
			
		}
		void draw(Float3 pos) {
			SHADER.begin();
			SHADER.setWorldMatrix(pos);
			SHADER.setMatrix();
			for (int i = 0; i < _meshes.size(); i++) {
				Mesh* mesh = _meshes[i];
				aiMesh* pMesh = _pScene->mMeshes[i];

				aiString texName;
				aiMaterial* pMaterial = _pScene->mMaterials[pMesh->mMaterialIndex];
				pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texName);

				if (texName != aiString("")) {
					TEXTURE.setTexture(0);
					DX3D.getDeviceContext()->PSSetShaderResources(0, 1, &_texture[texName.data]);
				}

				mesh->draw();
			}
		}
		Float3 getSize() {
			return _size;
		}

  private:
		const aiScene* _pScene = nullptr;
		std::vector<Mesh*> _meshes;
		std::unordered_map<std::string, ID3D11ShaderResourceView*> _texture;
		Float3 _size = { 0.0f, 0.0f, 0.0f };
};