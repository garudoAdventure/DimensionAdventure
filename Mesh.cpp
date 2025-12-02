#include "Mesh.h"
#include "Shader.h"

Mesh::Mesh(aiMesh* aiMesh, aiMaterial** aiMaterials) {
	_vertexData.reserve(aiMesh->mNumVertices);
	aiMaterial* aiMaterial = aiMaterials[aiMesh->mMaterialIndex];
	aiColor4D diffuseColor;
	aiGetMaterialColor(aiMaterial, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor);

	for (int i = 0; i < aiMesh->mNumVertices; i++) {
		const aiVector3D pos = aiMesh->mVertices[i];
		const aiVector3D normal = aiMesh->mNormals[i];
		Vertex vertex;
		vertex.postion = { pos.x, pos.y, pos.z };
		vertex.normal = { normal.x, normal.y, normal.z };
		vertex.color = { diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a };
		vertex.texCoord = { 0.0f, 0.0f };
		if (aiMesh->mTextureCoords[0] != nullptr) {
			vertex.texCoord.x = aiMesh->mTextureCoords[0][i].x;
			vertex.texCoord.y = aiMesh->mTextureCoords[0][i].y;
		}
		for (int j = 0; j < 4; j++) {
			vertex.boneIdx[j] = 0;
			vertex.boneWeight[j] = 0.0f;
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

	_boneOffset.reserve(aiMesh->mNumBones);
	_boneTransform.reserve(aiMesh->mNumBones);
	for (int boneID = 0; boneID < aiMesh->mNumBones; boneID++) {
		aiBone* aiBone = aiMesh->mBones[boneID];
		std::string boneName = aiBone->mName.C_Str();

		_boneNameToID[boneName] = boneID;
		_boneOffset.emplace_back(aiBone->mOffsetMatrix);
		XMFLOAT4X4 boneTransform;
		XMStoreFloat4x4(&boneTransform, XMMatrixIdentity());
		_boneTransform.emplace_back(boneTransform);

		for (int i = 0; i < aiBone->mNumWeights; i++) {
			aiVertexWeight vertexWeight = aiBone->mWeights[i];
			int vertexID = vertexWeight.mVertexId;
			float boneWeight = vertexWeight.mWeight;

			int idx = 0;
			while (idx < 4 && _vertexData[vertexID].boneWeight[idx] != 0.0f) {
				idx++;
			}
			if (idx < 4) {
				_vertexData[vertexID].boneIdx[idx] = boneID;
				_vertexData[vertexID].boneWeight[idx] = boneWeight;
			}
		}
	}
	
	_indexData.reserve(aiMesh->mNumFaces * 3);
	for (int i = 0; i < aiMesh->mNumFaces; i++) {
		aiFace face = aiMesh->mFaces[i];
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

	{
		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(XMFLOAT4X4) * aiMesh->mNumBones;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		DX3D.getDevice()->CreateBuffer(&desc, NULL, &_boneBuffer);
	}
}

void Mesh::update() {
	if (_boneTransform.size() > 0) {
		DX3D.getDeviceContext()->UpdateSubresource(_boneBuffer, 0, NULL, &_boneTransform[0], 0, 0);
	}
	DX3D.getDeviceContext()->UpdateSubresource(_vertexBuffer, 0, NULL, &_vertexData[0], 0, 0);
}

bool Mesh::updateBoneTransform(aiNode* node, aiMatrix4x4 globalTransform) {
	std::string nodeName = node->mName.C_Str();
	if (_boneNameToID.find(nodeName) == _boneNameToID.end()) {
		return false;
	}
	int boneID = _boneNameToID[nodeName];
	aiMatrix4x4 mat = globalTransform * _boneOffset[boneID];
	XMMATRIX boneMat = XMMATRIX(
		(float)mat.a1, (float)mat.a2, (float)mat.a3, (float)mat.a4,
		(float)mat.b1, (float)mat.b2, (float)mat.b3, (float)mat.b4,
		(float)mat.c1, (float)mat.c2, (float)mat.c3, (float)mat.c4,
		(float)mat.d1, (float)mat.d2, (float)mat.d3, (float)mat.d4
	);
	XMStoreFloat4x4(&_boneTransform[boneID], boneMat);
	return true;
}

void Mesh::updateNodeTransform(aiMatrix4x4 globalTransform) {
	nodeTransform = globalTransform;
}

void Mesh::draw(Float3 pos, Float3 radian) {
	aiMatrix4x4 mat = nodeTransform;
	XMMATRIX world = XMMATRIX(
		(float)mat.a1, (float)mat.b1, (float)mat.c1, (float)mat.d1,
		(float)mat.a2, (float)mat.b2, (float)mat.c2, (float)mat.d2,
		(float)mat.a3, (float)mat.b3, (float)mat.c3, (float)mat.d3,
		(float)mat.a4, (float)mat.b4, (float)mat.c4, (float)mat.d4
	);
	world *= XMMatrixScaling(1.0f, 1.0f, 1.0f);
	world *= XMMatrixRotationRollPitchYaw(radian.x, radian.y, radian.z);
	world *= XMMatrixTranslation(pos.x, pos.y, pos.z);

	SHADER.setWorldMatrix(world);
	SHADER.setMatrix();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	DX3D.getDeviceContext()->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	DX3D.getDeviceContext()->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	DX3D.getDeviceContext()->VSSetConstantBuffers(1, 1, &_boneBuffer);
	DX3D.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX3D.getDeviceContext()->DrawIndexed(_indexData.size(), 0, 0);
}

void Mesh::updateColor(Float4 color) {
	for (int i = 0; i < _vertexData.size(); i++) {
		_vertexData[i].color = XMFLOAT4(color.r, color.g, color.b, color.a);
	}
	DX3D.getDeviceContext()->UpdateSubresource(_vertexBuffer, 0, NULL, &_vertexData[0], 0, 0);
}