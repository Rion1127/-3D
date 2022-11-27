#include <Windows.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <cassert>
#include <string>
#include "WorldTransform.h"
#include "Material.h"
#include "Texture.h"
#include <fstream>
#include <sstream>
#include "Util.h"
#include "Object3d.h"

const std::string kBaseDirectory = "Resources/";

//�R�}���h���X�g���i�[����
static DirectXCommon* directX_ = nullptr;

Pipeline Object3d::pipeline_;

Object3d::~Object3d()
{
	for (auto m : vert_) {
		delete m;
	}
	vert_.clear();
	for (auto m : materials_) {
		delete m.second;
	}
	materials_.clear();
}

Object3d* Object3d::GetInstance()
{
	static Object3d instance;
	return &instance;
}

void Object3d::Ini()
{
	directX_ = DirectXCommon::GetInstance();

	ComPtr < ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr < ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr < ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g
#pragma region �V�F�[�_�t�@�C���ǂݍ���
	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	ShaderCompileFromFile(L"Resources/shader/BasicVS.hlsl", "main", "vs_5_0", &vsBlob,errorBlob.Get());
	
	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	ShaderCompileFromFile(L"Resources/shader/BasicPS.hlsl", "main", "ps_5_0", &psBlob,errorBlob.Get());
	
#pragma endregion

	pipeline_.Ini();
}

Object3d* Object3d::CreateOBJ(const std::string& modelname)
{
	Object3d* instance = new Object3d;
	instance->ModelIni(modelname);

	return instance;
}

std::unique_ptr<Object3d> Object3d::CreateOBJ_uniptr(const std::string& modelname)
{
	std::unique_ptr<Object3d> instance = std::make_unique<Object3d>();
	instance->ModelIni(modelname);

	return /*std::move(*/instance/*)*/;
}

void Object3d::SetBlend(int blend)
{
	pipeline_.SetBlend(blend);
}

void Object3d::SetNormal()
{
	pipeline_.SetNormalBlend();
}

void Object3d::SetModel(const Object3d* model)
{
	
	vert_.emplace_back(new Vertices);	//��̒��_�f�[�^������
	Vertices* vert = vert_.back();		//���vert_�̃A�h���X��vert�ɓ����

	for (size_t i = 0; i < model->vert_[0]->vertices.size(); i++) {
		vert->AddVertices(model->vert_[0]->vertices[i]);
	}

	for (size_t i = 0; i < model->vert_[0]->indices.size(); i++) {
		vert->AddIndex(model->vert_[0]->indices[i]);
	}


	//vert_.resize(model->vert_.size());
	//std::copy(model->vert_.begin(), model->vert_.end(), vert_.begin());

	//textureHandle_.resize(model->textureHandle_.size());
	//std::copy(model->textureHandle_.begin(), model->textureHandle_.end(), textureHandle_.begin());

	//for (size_t i = 0; i < model->materials_.size(); i++) {
	//	materials_.insert(model->materials_.at());
	//}

	//materials_.insert(model->materials_.begin(), model->materials_.end());

	for (size_t i = 0; i < model->textureHandle_.size();i++) {
		textureHandle_.push_back(model->textureHandle_[i]);
	}

	// ���b�V���̃o�b�t�@����
	for (auto& m : vert_) {
		m->CreateBuffer(directX_->GetDevice());
	}
}

void Object3d::LoadOBJ(const std::string& modelname)
{
	const std::string filename = modelname + ".obj";
	const std::string directoryPath = kBaseDirectory + modelname + "/";
	// �t�@�C���X�g���[��
	std::ifstream file;
	// .obj�t�@�C�����J��
	file.open(directoryPath + filename);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	name_ = modelname;

	std::string line;	//�t�@�C����1�s������ϐ�

	std::vector<XMFLOAT3> positions; //���_���W
	std::vector<XMFLOAT3> normals;   // �@���x�N�g��
	std::vector<XMFLOAT2> texcoords; // �e�N�X�`��UV

	vert_.emplace_back(new Vertices);	//��̒��_�f�[�^������
	Vertices* vert = vert_.back();		//���vert_�̃A�h���X��vert�ɓ����

	int indexCountTex = 0;

	while (getline(file, line)) {

		std::stringstream line_stream(line);

		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		std::string key;
		getline(line_stream, key, ' ');

		//�}�e���A��
		if (key == "mtllib") {
			// �}�e���A���̃t�@�C�����ǂݍ���
			std::string filename;
			line_stream >> filename;
			// �}�e���A���ǂݍ���
			LoadMaterial(directoryPath, filename);
		}

		if (key == "v") {
			// X,Y,Z���W�ǂݍ���
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			positions.emplace_back(position);
		}

		if (key == "vn") {
			// X,Y,Z�����ǂݍ���
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			// �@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}

		// �擪������vt�Ȃ�e�N�X�`��
		if (key == "vt") {
			// U,V�����ǂݍ���
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			// V�������]
			texcoord.y = 1.0f - texcoord.y;
			// �e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);
		}

		if (key == "f") {
			int faceIndexCount = 0;
			// ���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			std::string index_string;
			while (getline(line_stream, index_string, ' ')) {
				// ���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				// ���_�ԍ�
				index_stream >> indexPosition;
				index_stream.seekg(1, std::ios_base::cur); // �X���b�V�����΂�
				index_stream >> indexTexcoord;
				index_stream.seekg(1, std::ios_base::cur); // �X���b�V�����΂�
				index_stream >> indexNormal;
				Vertices::VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];

				vert->AddVertices(vertex);

				// �C���f�b�N�X�f�[�^�̒ǉ�
				if (faceIndexCount >= 3) {
					// �l�p�`�|���S����4�_�ڂȂ̂ŁA
					// �l�p�`��0,1,2,3�̓� 2,3,0�ŎO�p�`���\�z����
					vert->AddIndex(indexCountTex - 1);
					vert->AddIndex(indexCountTex);
					vert->AddIndex(indexCountTex - 3);
				}
				else {
					vert->AddIndex(indexCountTex);
				}
				faceIndexCount++;
				indexCountTex++;
			}
		}
	}

	LoadTexture();
}

void Object3d::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	// �t�@�C���X�g���[��
	std::ifstream file;
	// �}�e���A���t�@�C�����J��
	file.open(directoryPath + filename);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	Material* material = nullptr;

	// 1�s���ǂݍ���
	std::string line;
	while (getline(file, line)) {
		// �擪�̃^�u�����͖�������
		if (line[0] == '\t') {
			line.erase(line.begin()); // �擪�̕������폜
		}
		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		std::string key;
		getline(line_stream, key, ' ');

		// �擪�̃^�u�����͖�������
		if (key[0] == '\t') {
			key.erase(key.begin()); // �擪�̕������폜
		}

		// �擪������newmtl�Ȃ�}�e���A����
		if (key == "newmtl") {

			// ���Ƀ}�e���A���������
			if (material) {
				// �}�e���A�����R���e�i�ɓo�^
				AddMaterial(material);
			}

			// �V�����}�e���A���𐶐�
			material = Material::Create(directX_->GetDevice());
			// �}�e���A�����ǂݍ���
			line_stream >> material->name_;
			
		}

		// �擪������map_Kd�Ȃ�e�N�X�`���t�@�C����
		if (key == "map_Kd") {
			// �e�N�X�`���̃t�@�C�����ǂݍ���
			line_stream >> material->textureFilename_;

			// �t���p�X����t�@�C���������o��
			size_t pos1;
			pos1 = material->textureFilename_.rfind('\\');
			if (pos1 != std::string::npos) {
				material->textureFilename_ = material->textureFilename_.substr(
					pos1 + 1, material->textureFilename_.size() - pos1 - 1);
			}

			pos1 = material->textureFilename_.rfind('/');
			if (pos1 != std::string::npos) {
				material->textureFilename_ = material->textureFilename_.substr(
					pos1 + 1, material->textureFilename_.size() - pos1 - 1);
			}
		}
	}
	// �t�@�C�������
	file.close();

	
	if (material) {
		// �}�e���A����o�^
		AddMaterial(material);
	}
}

void Object3d::LoadTexture()
{
	std::string directoryPath = name_ + "/";

	for (auto& m : materials_) {
		Material* material = m.second;

		// �e�N�X�`������
		if (material->textureFilename_.size() > 0) {
			// �}�e���A���Ƀe�N�X�`���ǂݍ���
			material->LoadTexture(directoryPath);
		}
		// �e�N�X�`���Ȃ�
		else {
			// �}�e���A���Ƀe�N�X�`���ǂݍ���
			material->LoadTexture("white1x1.png");
		}
		textureHandle_.push_back(material->textureHandle_);
	}
}

void Object3d::AddMaterial(Material* material)
{
	// �R���e�i�ɓo�^
	materials_.emplace(material->name_, material);
}

void Object3d::ModelIni(const std::string& modelname)
{
	LoadOBJ(modelname);
	// ���b�V���̃o�b�t�@����
	for (auto& m : vert_) {
		m->CreateBuffer(directX_->GetDevice());
	}
}

void Object3d::PreDraw()
{
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	directX_->GetCommandList()->SetPipelineState(pipeline_.gerPipelineState());
	directX_->GetCommandList()->SetGraphicsRootSignature(pipeline_.GetRootSignature());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	directX_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g
}

void Object3d::ObjChangeColor(float x, float y, float z, float w)
{
	for (auto& v : vert_) {
		v->ChangeColor(x, y, z, w);
	}
}

void Object3d::ObjChangeColor(XMFLOAT4 color_)
{
	for (auto& v : vert_) {
		v->ChangeColor(color_);
	}
}

void Object3d::DrawOBJ(WorldTransform* worldTransform)
{
	for (auto& v : vert_) {
		v->Draw(directX_->GetCommandList(), worldTransform, textureHandle_.at(0));
	}
}

void Object3d::DrawOBJ(WorldTransform* worldTransform, uint32_t textureHandle)
{
	for (auto& v : vert_) {
		v->Draw(directX_->GetCommandList(), worldTransform, textureHandle);
	}
}

