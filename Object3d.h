#pragma once
#include <wrl.h>
#include <map>
#include <string>
#include "Vertices.h"
using namespace Microsoft::WRL;
class Object3d
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	~Object3d();

	static Object3d* GetInstance();

	static void Ini(ID3D12Device* device);

	static Object3d* CreateOBJ(const std::string& modelname, ID3D12Device* device);
	//.OBJから情報を読み込む
	void LoadOBJ(const std::string& modelname);
	//.mtlからテクスチャを読み込む
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
		XMFLOAT3 pos;		//xyz座標
		XMFLOAT3 normal;	//法線ベクトル
		XMFLOAT2 uv;		//uv座標
	};
	
	// 名前
	std::string name_;
	
	std::vector<Vertices*> vert_;
	std::vector<uint32_t> textureHandle_;
	// マテリアルコンテナ
	std::map<std::string, Material*> materials_;
};

