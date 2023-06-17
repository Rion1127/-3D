#pragma once
#include <wrl.h>
#include <map>
#include <string>
#include <memory>
#include <unordered_map>
#include "Vertices.h"
#include "DirectX.h"
#include "PipelineManager.h"
#include "LightGroup.h"
#include "Texture.h"
#include "myMath.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Material.h"
#include "WorldTransform.h"

class Model
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	struct VertexPosNormalUv {
		Vector3 pos;		//xyz座標
		Vector3 normal;	//法線ベクトル
		Vector2 uv;		//uv座標
	};

	// 名前
	std::string name_;

	//頂点法線スムージング用データ
	bool smoothing_ = false;
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData_;
	void AddSmoothData(unsigned short indexPositon, unsigned short indexVertex);
	//平滑化された頂点法線の計算
	void CalculateSmoothedVertexNormals();
public:
	std::vector<std::unique_ptr<Vertices>> vert_;
	std::vector<Texture> texture_;
	// マテリアルコンテナ
	std::map<std::string, std::unique_ptr<Material>> materials_;
	//ライト
	static std::shared_ptr<LightGroup> lightGroup_;


	~Model();

	static Model* GetInstance();

	static void Ini();
	//モデルを読み込む
	static Model* CreateOBJ(const std::string& modelname, bool smoothing = false);
	static std::unique_ptr<Model> CreateOBJ_uniptr(const std::string& modelname, bool smoothing = false);
	/// <summary>
	/// ブレンド設定
	/// </summary>
	/// <param name="BLEND_ALPHA">アルファブレンド</param>
	/// <param name="BLEND_SUB">減算合成</param>
	/// <param name="BLEND_NEGA">色反転合成</param>
	/// <param name="BLEND_NORMAL">半透明合成</param>
	static void SetBlend(uint32_t blend);

	void SetModel(const Model* model);

	static void SetLight(std::shared_ptr<LightGroup> lightGroup) { Model::lightGroup_ = lightGroup; }
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
	static void PreDraw();

	void DrawOBJ(const WorldTransform& worldTransform);
	void DrawOBJ(const WorldTransform& worldTransform, uint32_t textureHandle);

};

