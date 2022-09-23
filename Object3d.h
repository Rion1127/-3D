#pragma once
#include <wrl.h>
#include <map>
#include <string>
#include "Vertices.h"
using namespace Microsoft::WRL;
class Object3d
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	~Object3d();

	static Object3d* GetInstance();

	static void Ini(ID3D12Device* device);

	static Object3d* CreateOBJ(const std::string& modelname, ID3D12Device* device);
	//.OBJ�������ǂݍ���
	void LoadOBJ(const std::string& modelname);
	//.mtl����e�N�X�`����ǂݍ���
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	void LoadTexture();

	void AddMaterial(Material* material);

	void ModelIni(const std::string& modelname, ID3D12Device* device);

	static void PreDraw(ID3D12GraphicsCommandList* commandList);

	void ChangeColor(float x,float y,float z,float w);
	void ChangeColor(XMFLOAT4 color_);

	void DrawCube(WorldTransform* worldTransform,uint32_t descriptorSize);

	void DrawOBJ(WorldTransform* worldTransform);

private:
	struct VertexPosNormalUv {
		XMFLOAT3 pos;		//xyz���W
		XMFLOAT3 normal;	//�@���x�N�g��
		XMFLOAT2 uv;		//uv���W
	};
	
	// ���O
	std::string name_;
	
	std::vector<Vertices*> vert_;
	std::vector<uint32_t> textureHandle_;
	// �}�e���A���R���e�i
	std::map<std::string, Material*> materials_;
};

