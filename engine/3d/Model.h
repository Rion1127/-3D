#pragma once
#include <wrl.h>
#include <map>
#include <string>
#include "Vertices.h"
#include "DirectX.h"
#include "PipelineManager.h"
#include <unordered_map>
#include "LightGroup.h"

using namespace Microsoft::WRL;
class Model
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	~Model();

	static Model* GetInstance();

	static void Ini();
	//モデルを読み込む
	static Model* CreateOBJ(const std::string& modelname,bool smoothing = false);
	static std::unique_ptr<Model> CreateOBJ_uniptr(const std::string& modelname, bool smoothing = false);
	

	void SetModel(const Model* model);

	static void SetLight(LightGroup* lightGroup) { Model::lightGroup = lightGroup; }
private:
	//モデル初期化(CreateOBJ()に入っている)
	void ModelIni(const std::string& modelname, bool smoothing);
	//.OBJから情報を読み込む(ModelIni()に入っている)
	void LoadOBJ(const std::string& modelname);
	//.mtlからテクスチャを読み込む
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	void LoadTexture();

	void AddMaterial(Material* material);

public:
	

	//オブジェクトの色を変える
	void ObjChangeColor(float x, float y, float z, float w);
	void ObjChangeColor(XMFLOAT4 color_);

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

	//頂点法線スムージング用データ
	bool smoothing = false;
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;
	void AddSmoothData(unsigned short indexPositon, unsigned short indexVertex);
	//平滑化された頂点法線の計算
	void CalculateSmoothedVertexNormals();
public:
	std::vector<Vertices*> vert_;
	std::vector<uint32_t> textureHandle_;
	// マテリアルコンテナ
	std::map<std::string, Material*> materials_;
	//ライト
	static LightGroup* lightGroup;

};

