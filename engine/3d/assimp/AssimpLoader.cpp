#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <cassert>
#include "AssimpLoader.h"
#include "Util.h"



AssimpLoader* AssimpLoader::GetInstance()
{
	static AssimpLoader instance;
	return &instance;
}



bool AssimpLoader::Load(ImportSettings* setting)
{
	assert(setting->filename);

	auto& meshes = setting->meshes;
	auto inverseU = setting->inverseU;
	auto inverseV = setting->inverseV;

	auto path = ToUTF8(setting->filename);

	Assimp::Importer importer;
	//以下のフラグの数値を代入していく
	uint32_t flag = 0;
	
	/*flag |= aiProcess_Triangulate;
	flag |= aiProcess_PreTransformVertices;
	flag |= aiProcess_JoinIdenticalVertices;
	flag |= aiProcess_CalcTangentSpace;
	flag |= aiProcess_GenSmoothNormals;
	flag |= aiProcess_GenUVCoords;
	flag |= aiProcess_TransformUVCoords;
	flag |= aiProcess_RemoveRedundantMaterials;
	flag |= aiProcess_OptimizeMeshes;
	flag |= aiProcess_LimitBoneWeights;*/

	flag |= aiProcess_Triangulate;
	flag |= aiProcess_JoinIdenticalVertices;
	flag |= aiProcess_CalcTangentSpace;
	flag |= aiProcess_GenSmoothNormals;
	flag |= aiProcess_GenUVCoords;
	flag |= aiProcess_TransformUVCoords;
	flag |= aiProcess_RemoveRedundantMaterials;
	flag |= aiProcess_OptimizeMeshes;
	flag |= aiProcess_LimitBoneWeights;

	auto scene = importer.ReadFile(path, flag);

	if (scene == nullptr)
	{
		// もし読み込みエラーがでたら表示する
		printf(importer.GetErrorString());
		printf("\n");
		OutputDebugStringA("scene = nullptr");
		return false;
	}

	// 読み込んだデータを自分で定義したMesh構造体に変換する
	meshes.clear();
	meshes.resize(scene->mNumMeshes);
	for (uint32_t i = 0; i < meshes.size(); ++i)
	{
		const auto pMesh = scene->mMeshes[i];
		LoadMesh(meshes[i], pMesh, inverseU, inverseV);
		const auto pMaterial = scene->mMaterials[i];
		if (scene->mNumMaterials > i) {
			LoadTexture(setting->filename, meshes[i], pMaterial);
		}
		LoadBones(i, scene->mMeshes[i],setting);
		//if (scene->mMeshes[i]->mBones != nullptr)
		//{
		//	//ボーンの情報を生成
		//	setting->bones_.emplace_back(new aiBone(**scene->mMeshes[i]->mBones));
		//	//Matrixを転置
		//	setting->boneMtrix_.emplace_back();
		//	DirectX::XMMATRIX& mt = setting->boneMtrix_.back();
		//	aiMatrix4x4& m = setting->bones_.at(i)->mOffsetMatrix;
		//	mt = {
		//		m.a1, m.b1, m.c1, m.d1,	// 転置
		//		m.a2, m.b2, m.c2, m.d2,
		//		m.a3, m.b3, m.c3, m.d3,
		//		m.a4, m.b4, m.c4, m.d4
		//	};
		//	
		//}
		
	}

	

	scene = nullptr;

	return true;
}

void AssimpLoader::LoadMesh(Mesh& dst, const aiMesh* src, bool inverseU, bool inverseV)
{
	aiVector3D zero3D(0.0f, 0.0f, 0.0f);
	aiColor4D zeroColor(0.0f, 0.0f, 0.0f, 0.0f);

	dst.Vertices.vertices_.resize(src->mNumVertices);

	for (auto i = 0u; i < src->mNumVertices; ++i)
	{
		auto position = &(src->mVertices[i]);
		auto normal = &(src->mNormals[i]);
		auto uv = (src->HasTextureCoords(0)) ? &(src->mTextureCoords[0][i]) : &zero3D;
		

		// 反転オプションがあったらUVを反転させる
		if (inverseU)
		{
			uv->x = 1 - uv->x;
		}
		if (inverseV)
		{
			uv->y = 1 - uv->y;
		}

		Vertices::VertexPosNormalUv vertex = {};
		vertex.pos = Vector3(position->x, position->y, position->z);
		vertex.normal = Vector3(normal->x, normal->y, normal->z);
		vertex.uv = Vector2(uv->x, uv->y);


		dst.Vertices.vertices_[i] = vertex;
	}

	dst.Vertices.indices_.resize(src->mNumFaces * 3);

	for (auto i = 0u; i < src->mNumFaces; ++i)
	{
		const auto& face = src->mFaces[i];

		dst.Vertices.indices_[i * 3 + 0] = (uint16_t)face.mIndices[0];
		dst.Vertices.indices_[i * 3 + 1] = (uint16_t)face.mIndices[1];
		dst.Vertices.indices_[i * 3 + 2] = (uint16_t)face.mIndices[2];
	}
}

void AssimpLoader::LoadTexture(const wchar_t* filename, Mesh& dst, const aiMaterial* src)
{
	aiString path;
	if (src->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
	{
		// テクスチャパスは相対パスで入っているので、ファイルの場所とくっつける
		auto dir = GetDirectoryPath(filename);
		auto file = std::string(path.C_Str());
		
		std::wstring filename_ = dir + ToWideString(file);

		filename_ = ReplaceExtension(filename_,"tga");
		dst.diffuseMap = filename_;
	}
	else
	{
		dst.diffuseMap.clear();
	}
	
}

void AssimpLoader::LoadBones(uint32_t MeshIndex, const aiMesh* pMesh, ImportSettings* setting)
{
	uint32_t m_NumBones = 0;
	for (uint32_t i = 0; i < pMesh->mNumBones; i++)
	{
		uint32_t BoneIndex = 0;
		std::string BoneName(pMesh->mBones[i]->mName.data);

		setting->boneData.emplace_back();
		BoneIndex = m_NumBones;
		m_NumBones++;
		
		aiMatrix4x4& m = pMesh->mBones[i]->mOffsetMatrix;
		aiBone& bone = *pMesh->mBones[i];
		
		for (size_t j = 0; j < bone.mNumWeights; j++)
		{
			auto& weight = bone.mWeights[j];
		}
		setting->boneData[BoneIndex].boneMatrix_ = {
			m.a1, m.b1, m.c1, m.d1,	// 転置
			m.a2, m.b2, m.c2, m.d2,
			m.a3, m.b3, m.c3, m.d3,
			m.a4, m.b4, m.c4, m.d4
		};
		
	}
	
}

