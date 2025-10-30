#pragma once

#include <assimp/scene.h>
#include <d3d11.h>
#include "DirectX.h"
#include "MathStruct.h"
#include "Shader.h"
#include <vector>

class Mesh {
  public:
		Mesh(aiMesh* pMesh) {
			_vertexData.reserve(pMesh->mNumVertices);
			for (int i = 0; i < pMesh->mNumVertices; i++) {
				const aiVector3D vertexPos = pMesh->mVertices[i];
				const aiVector3D normal = pMesh->mNormals[i];
				Vertex vertex;
				vertex.postion = { vertexPos.x, -vertexPos.z, vertexPos.y, };
				vertex.normal = { normal.x, -normal.z, normal.y };
				vertex.color = { 1.0f, 1.0f, 1.0f, 1.0f };
				vertex.texCoord = { 0.0f, 0.0f };
				if (pMesh->mTextureCoords[0] != nullptr) {
					vertex.texCoord.x = pMesh->mTextureCoords[0][i].x;
					vertex.texCoord.y = pMesh->mTextureCoords[0][i].y;
				}
				_vertexData.emplace_back(vertex);

				if (vertex.postion.x > maxVertex.x) {
					maxVertex.x = vertex.postion.x;
				}
				if (vertex.postion.y > maxVertex.y) {
					maxVertex.y = vertex.postion.y;
				}
				if (vertex.postion.z > maxVertex.z) {
					maxVertex.z = vertex.postion.z;
				}
				if (vertex.postion.x < minVertex.x) {
					minVertex.x = vertex.postion.x;
				}
				if (vertex.postion.y < minVertex.y) {
					minVertex.y = vertex.postion.y;
				}
				if (vertex.postion.z < minVertex.z) {
					minVertex.z = vertex.postion.z;
				}
			}

			_indexData.reserve(pMesh->mNumFaces * 3);
			for (int i = 0; i < pMesh->mNumFaces; i++) {
				aiFace face = pMesh->mFaces[i];
				_indexData.emplace_back(face.mIndices[0]);
				_indexData.emplace_back(face.mIndices[1]);
				_indexData.emplace_back(face.mIndices[2]);
			}

			{
				D3D11_BUFFER_DESC desc = {};
				desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				desc.ByteWidth = sizeof(Vertex) * _vertexData.size();
				desc.Usage = D3D11_USAGE_DEFAULT;
				desc.MiscFlags = 0;
				desc.StructureByteStride = 0;
				desc.CPUAccessFlags = 0;

				D3D11_SUBRESOURCE_DATA data = {};
				data.pSysMem = &_vertexData[0];
				data.SysMemPitch = 0;
				data.SysMemSlicePitch = 0;
				DX3D.getDevice()->CreateBuffer(&desc, &data, &_vertexBuffer);
			}

			{
				D3D11_BUFFER_DESC desc = {};
				desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
				desc.ByteWidth = sizeof(WORD) * _indexData.size();
				desc.Usage = D3D11_USAGE_DEFAULT;
				desc.CPUAccessFlags = 0;
				desc.MiscFlags = 0;
				desc.StructureByteStride = 0;

				D3D11_SUBRESOURCE_DATA data = {};
				data.pSysMem = &_indexData[0];
				data.SysMemPitch = 0;
				data.SysMemSlicePitch = 0;
				DX3D.getDevice()->CreateBuffer(&desc, &data, &_indexBuffer);
			}
		}

		void update() {
		}

		void draw() {
			UINT stride = sizeof(Vertex);
			UINT offset = 0;
			DX3D.getDeviceContext()->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
			DX3D.getDeviceContext()->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
			DX3D.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			DX3D.getDeviceContext()->DrawIndexed(_indexData.size(), 0, 0);
		}
		Float3 getMinVertex() {
			return minVertex;
		}
		Float3 getMaxVertex() {
			return maxVertex;
		}

  private:
		ID3D11Buffer* _vertexBuffer = nullptr;
		ID3D11Buffer* _indexBuffer = nullptr;
		std::vector<Vertex> _vertexData;
		std::vector<WORD> _indexData;
		Float3 maxVertex = { -999.0f, -999.0f, -999.0f };
		Float3 minVertex = { 999.0f, 999.0f, 999.0f };
};