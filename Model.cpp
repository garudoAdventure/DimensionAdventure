#include "Model.h"
#include "Texture.h"
#include "Shader.h"

Model::Model(const char* fileName) {
	_aiScene = aiImportFile(fileName,
		aiProcess_Triangulate
		//aiProcess_JoinIdenticalVertices |
		//aiProcess_LimitBoneWeights |
		//aiProcess_ImproveCacheLocality |
		//aiProcess_FlipUVs |
		//aiProcessPreset_TargetRealtime_MaxQuality |
		//aiProcess_ConvertToLeftHanded
	);
	_rootNode = _aiScene->mRootNode;
	_globalInverseTransform = _rootNode->mChildren[0]->mTransformation.Inverse();

	for (int i = 0; i < _aiScene->mNumAnimations; i++) {
		aiAnimation* aiAnim = _aiScene->mAnimations[i];
		_animations.emplace_back(aiAnim);
	}
	loadAnim(0);

	_meshes.reserve(_aiScene->mNumMeshes);
	Float3 maxVertex = { -999.0f, -999.0f, -999.0f };
	Float3 minVertex = { 999.0f, 999.0f, 999.0f };
	for (int i = 0; i < _aiScene->mNumMeshes; i++) {
		aiMesh* aiMesh = _aiScene->mMeshes[i];
		Mesh* mesh = new Mesh(aiMesh, _aiScene->mMaterials);
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

	for (int i = 0; i < _aiScene->mNumTextures; i++) {
		aiTexture* aiTexture = _aiScene->mTextures[i];

		ID3D11ShaderResourceView* texture;
		TexMetadata metadata;
		ScratchImage image;
		LoadFromWICMemory(aiTexture->pcData, aiTexture->mWidth, WIC_FLAGS_NONE, &metadata, image);
		CreateShaderResourceView(DX3D.getDevice(), image.GetImages(), image.GetImageCount(), metadata, &texture);

		_texture[aiTexture->mFilename.data] = texture;
	}
}

Model::~Model() {
	for (Mesh* mesh : _meshes) {
		delete mesh;
	}
}

void Model::loadAnim(int animIdx) {
	aiAnimation* aiAnim = _animations[animIdx];
	_animDuration = aiAnim->mDuration;
	for (int i = 0; i < aiAnim->mNumChannels; i++) {
		aiNodeAnim* pNodeAnim = aiAnim->mChannels[i];
		AnimData animData;
		for (int j = 0; j < pNodeAnim->mNumPositionKeys; j++) {
			animData.time.emplace_back(pNodeAnim->mPositionKeys[j].mTime);
			aiVector3D scaling = pNodeAnim->mScalingKeys[j].mValue;
			aiQuaternion rotation = pNodeAnim->mRotationKeys[j].mValue;
			aiVector3D position = pNodeAnim->mPositionKeys[j].mValue;
			animData.animTransformation.emplace_back(aiMatrix4x4(scaling, rotation, position));
		}
		_animData[pNodeAnim->mNodeName.C_Str()] = animData;
	}
}

void Model::update() {
	static int count = 0;
	updateNode(_frame, _rootNode, aiMatrix4x4());
	if (count % 2 == 0) {
		_frame = (_frame + 1) % _animDuration;
	}
	for (Mesh* mesh : _meshes) {
		mesh->update();
	}
	count++;
}

void Model::updateNode(int frame, aiNode* node, aiMatrix4x4 parentTransform) {
	std::string nodeName = node->mName.C_Str();
	aiMatrix4x4 nodeTransform = node->mTransformation;

	AnimData animData = _animData[nodeName];
	bool animDataExist = animData.time.size() != 0;
	if (animDataExist) {
		for (int i = 0; i < animData.time.size(); i++) {
			if (animData.time[i] <= frame) {
				nodeTransform = animData.animTransformation[i];
			}
		}
	}

	aiMatrix4x4 globalTransform = parentTransform * nodeTransform;
	if (nodeName == "NurbsPath.001") {
		for (int i = 0; i < node->mNumMeshes; i++) {
			Mesh* mesh = _meshes[node->mMeshes[i]];
			mesh->updateNodeTransform(_globalInverseTransform * globalTransform);
		}
	}	
	for (int i = 0; i < _meshes.size(); i++) {
		 _meshes[i]->updateBoneTransform(node, _globalInverseTransform * globalTransform);
	}
	for (int i = 0; i < node->mNumChildren; i++) {
		updateNode(frame, node->mChildren[i], globalTransform);
	}
}

void Model::draw(Float3 pos, Float3 radian) {
	SHADER.begin();

	Light light;
	light.enable = true;
	XMVECTOR direction{ 0.5f, 0.5f, 1.0f };
	direction = XMVector3Normalize(direction);
	XMStoreFloat3(&light.direction, direction);
	SHADER.setLight(light);

	TEXTURE.setTexture(-1);
	for (int i = 0; i < _meshes.size(); i++) {
		Mesh* mesh = _meshes[i];
		aiString texName;
		if (texName != aiString("")) {
			TEXTURE.setTexture(0);
			DX3D.getDeviceContext()->PSSetShaderResources(0, 1, &_texture[texName.data]);
		}
		mesh->draw(pos, radian);
	}
}