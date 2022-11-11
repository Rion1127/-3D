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
	//モデルを読み込む
	static Object3d* CreateOBJ(const std::string& modelname);
	//モデル初期化(CreateOBJ()に入っている)
	void ModelIni(const std::string& modelname);
	//.OBJから情報を読み込む(ModelIni()に入っている)
	void LoadOBJ(const std::string& modelname);
	//.mtlからテクスチャを読み込む
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	void LoadTexture();

	void AddMaterial(Material* material);

	

	static void PreDraw(ID3D12GraphicsCommandList* commandList);
	//オブジェクトの色を変える
	void ChangeColor(float x,float y,float z,float w);
	//オブジェクトの色を変える
	void ChangeColor(XMFLOAT4 color_);
	void ChangeColorObj(XMFLOAT4 color_);

	void DrawCube(WorldTransform* worldTransform,uint32_t descriptorSize);

	void DrawOBJ(WorldTransform* worldTransform);
	void DrawOBJ(WorldTransform* worldTransform, uint32_t textureHandle);

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

