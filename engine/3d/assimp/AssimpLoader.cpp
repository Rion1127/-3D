#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <cassert>
#include "AssimpLoader.h"
#include "Util.h"
#include <filesystem>

namespace fs = std::filesystem;

std::wstring GetDirectoryPath(const std::wstring& origin)
{
	std::wstring name = origin;

	fs::path p = origin.c_str();
	return p.remove_filename().c_str();
}

AssimpLoader* AssimpLoader::GetInstance()
{
	static AssimpLoader instance;
	return &instance;
}
//�g���q�����ւ���
std::wstring ReplaceExtension(const std::wstring& origin, const char* ext)
{
	fs::path p = origin.c_str();
	return p.replace_extension(ext).c_str();
}
//wstring��std::string(�}���`�o�C�g������)�ɕϊ�
std::string ToUTF8(const std::wstring& value)
{
	auto length = WideCharToMultiByte(CP_UTF8, 0U, value.data(), -1, nullptr, 0, nullptr, nullptr);
	std::vector<char> buffer;
	buffer.resize(length);
	WideCharToMultiByte(CP_UTF8, 0U, value.data(), -1, buffer.data(), length, nullptr, nullptr);

	std::string result(buffer.data());

	return result;
}

// std::string(�}���`�o�C�g������)����std::wstring(���C�h������)�𓾂�
std::wstring ToWideString(const std::string& str)
{
	auto num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(num1);

	auto num2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], num1);

	assert(num1 == num2);
	return wstr;
}
// std::wstring(���C�h������)����std::string(�}���`�o�C�g������)�𓾂�
std::string WStringToString(std::wstring oWString)
{
	// wstring �� SJIS
	uint32_t iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str()
		, -1, (char*)NULL, 0, NULL, NULL);

	// �o�b�t�@�̎擾
	std::vector<CHAR> cpMultiByte;
	cpMultiByte.resize(iBufferSize);
	// wstring �� SJIS
	WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str(), -1, cpMultiByte.data()
		, iBufferSize, NULL, NULL);

	// string�̐���
	std::string oRet(cpMultiByte.data(), cpMultiByte.data() + iBufferSize - 1);

	// �o�b�t�@�̔j��
	

	// �ϊ����ʂ�Ԃ�
	return(oRet);
}

bool AssimpLoader::Load(ImportSettings* setting)
{
	assert(setting->filename);

	auto& meshes = setting->meshes;
	auto inverseU = setting->inverseU;
	auto inverseV = setting->inverseV;

	auto path = ToUTF8(setting->filename);

	Assimp::Importer importer;
	//�ȉ��̃t���O�̐��l�������Ă���
	uint32_t flag = 0;
	/*flag |= aiProcess_Triangulate;
	flag |= aiProcess_PreTransformVertices;
	flag |= aiProcess_CalcTangentSpace;
	flag |= aiProcess_GenSmoothNormals;
	flag |= aiProcess_GenUVCoords;
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
		// �����ǂݍ��݃G���[���ł���\������
		printf(importer.GetErrorString());
		printf("\n");
		OutputDebugStringA("scene = nullptr");
		return false;
	}

	// �ǂݍ��񂾃f�[�^�������Œ�`����Mesh�\���̂ɕϊ�����
	meshes.clear();
	meshes.resize(scene->mNumMeshes);
	for (uint32_t i = 0; i < meshes.size(); ++i)
	{
		const auto pMesh = scene->mMeshes[i];
		LoadMesh(meshes[i], pMesh, inverseU, inverseV);
		const auto pMaterial = scene->mMaterials[i];
		LoadTexture(setting->filename, meshes[i], pMaterial);

		//LoadBones(i, scene->mMeshes[i],setting);
		//if (scene->mMeshes[i]->mBones != nullptr)
		//{
		//	//�{�[���̏��𐶐�
		//	setting->bones_.emplace_back(new aiBone(**scene->mMeshes[i]->mBones));
		//	//Matrix��]�u
		//	setting->boneMtrix_.emplace_back();
		//	DirectX::XMMATRIX& mt = setting->boneMtrix_.back();
		//	aiMatrix4x4& m = setting->bones_.at(i)->mOffsetMatrix;
		//	mt = {
		//		m.a1, m.b1, m.c1, m.d1,	// �]�u
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

	dst.Vertices.vertices.resize(src->mNumVertices);

	for (auto i = 0u; i < src->mNumVertices; ++i)
	{
		auto position = &(src->mVertices[i]);
		auto normal = &(src->mNormals[i]);
		auto uv = (src->HasTextureCoords(0)) ? &(src->mTextureCoords[0][i]) : &zero3D;
		

		// ���]�I�v�V��������������UV�𔽓]������
		if (inverseU)
		{
			uv->x = 1 - uv->x;
		}
		if (inverseV)
		{
			uv->y = 1 - uv->y;
		}

		Vertices::VertexPosNormalUv vertex = {};
		vertex.pos = DirectX::XMFLOAT3(position->x, position->y, position->z);
		vertex.normal = DirectX::XMFLOAT3(normal->x, normal->y, normal->z);
		vertex.uv = DirectX::XMFLOAT2(uv->x, uv->y);


		dst.Vertices.vertices[i] = vertex;
	}

	dst.Vertices.indices.resize(src->mNumFaces * 3);

	for (auto i = 0u; i < src->mNumFaces; ++i)
	{
		const auto& face = src->mFaces[i];

		dst.Vertices.indices[i * 3 + 0] = (uint16_t)face.mIndices[0];
		dst.Vertices.indices[i * 3 + 1] = (uint16_t)face.mIndices[1];
		dst.Vertices.indices[i * 3 + 2] = (uint16_t)face.mIndices[2];
	}
}

void AssimpLoader::LoadTexture(const wchar_t* filename, Mesh& dst, const aiMaterial* src)
{
	aiString path;
	if (src->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
	{
		// �e�N�X�`���p�X�͑��΃p�X�œ����Ă���̂ŁA�t�@�C���̏ꏊ�Ƃ�������
		auto dir = GetDirectoryPath(filename);
		auto file = std::string(path.C_Str());
		
		std::wstring filename = dir + ToWideString(file);

		filename = ReplaceExtension(filename,"tga");
		dst.diffuseMap = filename;
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
		setting->boneData[BoneIndex].boneMatrix_ = {
			m.a1, m.b1, m.c1, m.d1,	// �]�u
			m.a2, m.b2, m.c2, m.d2,
			m.a3, m.b3, m.c3, m.d3,
			m.a4, m.b4, m.c4, m.d4
		};
		/*if (m_BoneMapping.find(BoneName) == m_BoneMapping.end())
		{
			
			
			BoneInfo bi;
			m_BoneInfo.push_back(bi);
		}
		else
		{
			BoneIndex = m_BoneMapping[BoneName];
		}

		m_BoneMapping[BoneName] = BoneIndex;
		

		for (uint32_t j = 0; j < pMesh->mBones[i]->mNumWeights; j++)
		{
			uint32_t VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
			float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
			Bones[VertexID].AddBoneData(BoneIndex, Weight);
		}*/
	}
	
}
