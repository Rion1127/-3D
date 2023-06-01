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
	auto buffer = new char[length];

	WideCharToMultiByte(CP_UTF8, 0U, value.data(), -1, buffer, length, nullptr, nullptr);

	std::string result(buffer);
	delete[] buffer;
	buffer = nullptr;

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
	int iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str()
		, -1, (char*)NULL, 0, NULL, NULL);

	// �o�b�t�@�̎擾
	CHAR* cpMultiByte = new CHAR[iBufferSize];

	// wstring �� SJIS
	WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str(), -1, cpMultiByte
		, iBufferSize, NULL, NULL);

	// string�̐���
	std::string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);

	// �o�b�t�@�̔j��
	delete[] cpMultiByte;

	// �ϊ����ʂ�Ԃ�
	return(oRet);
}

bool AssimpLoader::Load(ImportSettings setting)
{
	assert(setting.filename);

	auto& meshes = setting.meshes;
	auto inverseU = setting.inverseU;
	auto inverseV = setting.inverseV;

	auto path = ToUTF8(setting.filename);

	Assimp::Importer importer;
	//�ȉ��̃t���O�̐��l�������Ă���
	int flag = 0;
	flag |= aiProcess_Triangulate;
	flag |= aiProcess_PreTransformVertices;
	flag |= aiProcess_CalcTangentSpace;
	flag |= aiProcess_GenSmoothNormals;
	flag |= aiProcess_GenUVCoords;
	flag |= aiProcess_RemoveRedundantMaterials;
	flag |= aiProcess_OptimizeMeshes;

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
	for (size_t i = 0; i < meshes.size(); ++i)
	{
		const auto pMesh = scene->mMeshes[i];
		LoadMesh(meshes[i], pMesh, inverseU, inverseV);
		const auto pMaterial = scene->mMaterials[i];
		LoadTexture(setting.filename, meshes[i], pMaterial);
		
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
		//auto tangent = (src->HasTangentsAndBitangents()) ? &(src->mTangents[i]) : &zero3D;
		//auto color = (src->HasVertexColors(0)) ? &(src->mColors[0][i]) : &zeroColor;

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