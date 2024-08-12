#pragma once
#include <wrl.h>
#include <vector>
#include "math.h"
#include "DirectX.h"
#include "PipelineManager.h"
#include <imgui.h>
#include "Vector2.h"
#include "Texture.h"
#include "Color.h"
#include "myMath.h"
#include "SpriteVertex.h"

/**
 * @file Sprite.h
 * @brief 2Dスプライトに関するクラス
 */

class Sprite
{
private:
	struct Vertex {
		Vector3 pos;
		Vector2 uv;
	};
	enum VertNum {
		LB,	//左下
		LT,	//左上
		RB,	//右下
		RT	//右上
	};
	struct ConstBufferDataMaterial {
		Color color;
	};
	struct ConstBufferDataTransform {
		Matrix4 mat;
		Vector2 tiling;
	};
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static uint32_t SAllNum;
	uint32_t spriteNum_ = 0;
	////頂点データ
	SpriteVertex vertex_;

	//定数バッファ用データ構造体
	ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;
	ConstBufferDataMaterial* constMapMaterial_ = nullptr;

	ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;
	ConstBufferDataTransform* constMapTransform_ = nullptr;
	Matrix4 matProjection_;

	Matrix4 matWorld_{};// ワールド行列

	Color color_;//色

	float rot_;				//回転
	Vector2 pos_;			//座標
	Vector2 scale_;			//スケール
	Vector2 anchorPoint_;	//アンカーポイント
	Vector2 splitNum_;		//画像を分割する数
	Vector2 tilingNum_;		//タイリング
	bool isFlipX_ = false;	//左右フリップ
	bool isFlipY_ = false;	//上下フリップ
	bool isVisible_ = true;					//表示フラグ
	bool isScaleChange_ = false;
	bool isMatrixChange_ = false;
	Vector2 textureLeftTop_ = { 0.0f,0.0f };	//テクスチャ左上座標
	Vector2 textureSize_ = { 0.f,0.f };			//テクスチャ切り出しサイズ
	bool isImguiDisplay_ = false;				//imgui表示フラグ
	uint32_t descriptorSize_ = 0;
	Texture texture_;		//テクスチャ

	bool isVertChange = false;
public:
	Sprite(const std::string& guiname = "");
	//セッター
	void Init(const std::string& guiname = "");
	void Update();

	//画像サイズ自動取得(描画座標は中心)
	void Draw();
	//画像の頂点データを自分で指定
	void Draw(float LuX, float LuY, float RuX, float RuY, float LdX, float LdY, float RdX, float RdY);
	//画像の頂点データを自分で指定
	void Draw(Vector2 LT, Vector2 RT, Vector2 LB, Vector2 RB);
	
	static void AddAllNum() { SAllNum++; }
public:
	//座標を代入する
	void SetPos(const Vector2& pos) { pos_ = pos; }
	//回転させる
	void SetRot(float rot) { rot_ = rot; }
	//スケール設定
	void SetScale(const Vector2& scale) {
		scale_ = scale;
		isScaleChange_ = true;
	}
	//画像の中心を決める
	void SetAnchor(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint; }
	//色変更
	void SetColor(const Color& color) { color_ = color; }
	//左右反転
	void SetFlipX(bool flip) { isFlipX_ = flip; }
	//上下反転
	void SetFlipY(bool flip) { isFlipY_ = flip; }
	//表示フラグ
	void SetVisivle(bool visivle) { isVisible_ = visivle; }
	//画像左上変更
	void SetTex_LeftTop(const Vector2& pos) { textureLeftTop_ = pos; }
	//画像のUVサイズ指定
	void SetTex_Size(const Vector2& pos) { textureSize_ = pos; }
	//テクスチャ
	void SetTexture(Texture* texture) { texture_ = *texture; }
	void SetImGui(bool flag) { isImguiDisplay_ = flag; }
	void SetSplitNum(const Vector2& splitNum) { splitNum_ = splitNum; }
	void SetTilingNum(const Vector2& tilingNum) { tilingNum_ = tilingNum; }
	//画像の描画されるサイズを指定する
	void SetSprite_Size(Vector2 LT, Vector2 RT, Vector2 LB, Vector2 RB);
public:
	//ゲッター
	const Vector2& GetScale() { return scale_; }
	const Vector2& GetPos() { return pos_; }
	float GetRotate() { return rot_; }
	const Vector2& GetsplitNum() { return splitNum_; }
	const Vector2& GetTex_Size() { return textureSize_; }
	const Color& GetColor() { return color_; }
	const Texture& GetTexture() { return texture_; }
private:
	void TransferVertex();
};

