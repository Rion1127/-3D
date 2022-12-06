#pragma once
#include <wrl.h>
#include <map>
#include <string>
#include "Vertices.h"
#include "DirectX.h"
#include "PipelineManager.h"

using namespace Microsoft::WRL;
class Object3d
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	~Object3d();

	static Object3d* GetInstance();

	static void Ini();
	//���f����ǂݍ���
	static Object3d* CreateOBJ(const std::string& modelname);
	static std::unique_ptr<Object3d> CreateOBJ_uniptr(const std::string& modelname);
	/// <summary>
	/// �u�����h�ݒ�
	/// </summary>
	/// <param name="BLEND_ALPHA">�A���t�@�u�����h</param>
	/// <param name="BLEND_SUB">���Z����</param>
	/// <param name="BLEND_NEGA">�F���]����</param>
	/// <param name="BLEND_NORMAL">����������</param>
	static void SetBlend(int blend);

	void SetModel(const Object3d* model);
private:
	//���f��������(CreateOBJ()�ɓ����Ă���)
	void ModelIni(const std::string& modelname);
	//.OBJ�������ǂݍ���(ModelIni()�ɓ����Ă���)
	void LoadOBJ(const std::string& modelname);
	//.mtl����e�N�X�`����ǂݍ���
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	void LoadTexture();

	void AddMaterial(Material* material);

public:
	static void PreDraw();

	//�I�u�W�F�N�g�̐F��ς���
	void ObjChangeColor(float x, float y, float z, float w);
	void ObjChangeColor(XMFLOAT4 color_);

	void DrawOBJ(WorldTransform* worldTransform);
	void DrawOBJ(WorldTransform* worldTransform, uint32_t textureHandle);

private:
	struct VertexPosNormalUv {
		XMFLOAT3 pos;		//xyz���W
		XMFLOAT3 normal;	//�@���x�N�g��
		XMFLOAT2 uv;		//uv���W
	};
	
	// ���O
	std::string name_;

	int blendType = 0;
public:
	std::vector<Vertices*> vert_;
	std::vector<uint32_t> textureHandle_;
	// �}�e���A���R���e�i
	std::map<std::string, Material*> materials_;

};

