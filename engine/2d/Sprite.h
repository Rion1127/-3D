#pragma once
#include <wrl.h>
#include <vector>
#include "math.h"
#include "DirectX.h"
#include "PipelineManager.h"
#include <imgui.h>
#include <DirectXMath.h>
#include "Vector2.h"
#include "Texture.h"
class Sprite
{
public:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	
	static void StaticIni();
	void Ini(const std::string& guiname = "");
	//座標を代入する
	void SetPos(float x, float y) {
		pos_.x = x;
		pos_.y = y;
	};
	void SetPos(const Vector2& pos) { pos_ = pos; }
	//回転させる
	void SetRot(float rot) { rot_ = rot; }
	//スケール設定
	void SetScale(float x, float y) {
		Scale_.x = x;
		Scale_.y = y;
	};
	void SetScale(const Vector2& scale) { Scale_ = scale; }
	//画像の中心を決める
	void SetAnchor(float x, float y) {
		anchorPoint_.x = x;
		anchorPoint_.y = y;
	};
	void SetAnchor(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint;}
	//色変更
	void ChangeColor(float x, float y, float z, float w) {
		color_ = {x, y, z, w};
	};
	void ChangeColor(const XMFLOAT4& color) {
		color_ = color;
	};
	//左右反転
	void SetFlipX(bool flip) { isFlipX_ = flip; }
	//上下反転
	void SetFlipY(bool flip) { isFlipY_ = flip; }
	//表示フラグ
	void SetInvisivle(bool invisivle) { isInvisible_ = invisivle; }
	//画像左上変更
	void SetTex_LeftTop(const Vector2& pos) { textureLeftTop_ = pos; }
	//画像UV座標変更
	void SetTex_Size(const Vector2& pos) { textureSize_ = pos; }
	//テクスチャ
	void SetTexture(Texture* texture) { texture_ = *texture; }

	Vector2 GetScale() { return Scale_; }
	Vector2 GetPos() { return pos_; }

	void SetImGui(bool flag) { isImguiDisplay_ = flag; }
	void DrawImGui();

	void Update();

	static void PreDraw();
	//画像サイズ自動取得(描画座標は中心)
	void Draw();
	//画像の頂点データを自分で指定
	void Draw(float LuX, float LuY, float RuX, float RuY, float LdX, float LdY, float RdX, float RdY, UINT descriptorSize);
	/// <summary>
	/// ブレンド設定
	/// </summary>
	/// <param name="BLEND_ALPHA">アルファブレンド</param>
	/// <param name="BLEND_SUB">減算合成</param>
	/// <param name="BLEND_NEGA">色反転合成</param>
	/// <param name="BLEND_NORMAL">半透明合成</param>
	static void SetBlend(uint32_t blend);

	static void AddAllNum() { SAllNum++; }
protected:
	void TransferVertex();
protected:
	static uint32_t SAllNum;
	uint32_t spriteNum_ = 0;

#pragma region 頂点データ
	struct Vertex {
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};
	enum VertNum {
		LB,	//左下
		LT,	//左上
		RB,	//右下
		RT	//右上
	};
	//頂点データ
	std::vector<Vertex> vertices_;
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	// 頂点バッファの生成
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;

	Vertex* vertMap_ = nullptr;
#pragma endregion
#pragma region インデックスデータ
	//インデックスデータ
	std::vector<uint16_t> indices_;
	// インデックスバッファの生成
	ComPtr<ID3D12Resource> indexBuff_ = nullptr;
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_{};
#pragma endregion

	ComPtr<ID3D12GraphicsCommandList> commandList_;
	//定数バッファ用データ構造体
	struct ConstBufferDataMaterial {
		XMFLOAT4 color;
	};
	ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;
	ConstBufferDataMaterial* constMapMaterial_ = nullptr;

	struct ConstBufferDataTransform {
		XMMATRIX mat;
	};
	ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;
	ConstBufferDataTransform* constMapTransform_ = nullptr;
	XMMATRIX matProjection_;

	
	DirectX::XMMATRIX matWorld_{};// ワールド行列
	
	DirectX::XMFLOAT4 color_;//色
	
	float rot_;				//回転
	Vector2 pos_;			//座標
	Vector2 Scale_;			//スケール
	Vector2 anchorPoint_;	//アンカーポイント
	bool isFlipX_ = false;	//左右フリップ
	bool isFlipY_ = false;	//上下フリップ
	bool isInvisible_ = false;					//非表示フラグ
	Vector2 textureLeftTop_ = { 0.0f,0.0f };	//テクスチャ左上座標
	Vector2 textureSize_ = { 0.f,0.f };			//テクスチャ切り出しサイズ
	bool isImguiDisplay_ = false;				//imgui表示フラグ
	uint32_t descriptorSize_ = 0;
	Texture texture_;		//テクスチャ

#pragma region
	std::string guiName_;
	uint32_t clicked_ = 0;
	const char* gui_;

	std::string name_;
#pragma endregion
};

