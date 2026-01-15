#include "Model.h"
#include "Shader.h"
#include "Texture.h"

Model::Model(const std::string fileName) {
	_aiScene = aiImportFile(fileName.c_str(), aiProcess_Triangulate);
	_rootNode = _aiScene->mRootNode;
	_globalInverseTransform = _rootNode->mChildren[0]->mTransformation.Inverse();

	for (int i = 0; i < _aiScene->mNumAnimations; i++) {
		aiAnimation* aiAnim = _aiScene->mAnimations[i];
		_animations.emplace_back(aiAnim);
	}

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
	for (int i = 0; i < _meshes.size(); i++) {
		 _meshes[i]->updateBoneTransform(
			 node, _globalInverseTransform * globalTransform
		 );
	}
	for (int i = 0; i < node->mNumChildren; i++) {
		updateNode(frame, node->mChildren[i], globalTransform);
	}
}

void Model::draw(Float3 pos, Float3 radian, Float3 scale) {
	DX3D.setDepthEnable(true);
	SHADER.setPS(PS::NO_TEX);
	for (int i = 0; i < _meshes.size(); i++) {
		Mesh* mesh = _meshes[i];
		aiString texName;
		aiMesh* aiMesh = _aiScene->mMeshes[i];
		aiMaterial* aimaterial = _aiScene->mMaterials[aiMesh->mMaterialIndex];
		aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texName);
		if (texName != aiString("")) {
			TEXTURE.setTexture(_texture[texName.data]);
			SHADER.setPS(PS::GENERAL);
		}
		mesh->draw(pos, radian, scale);
	}
}

void Model::updateColor(Float4 color) {
	for (Mesh* mesh : _meshes) {
		mesh->updateColor(color);
	}
}