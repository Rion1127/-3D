#pragma once
#include "REngine.h"
#include "LightGroup.h"
#include "Model.h"
#include "Camera.h"
#include "Color.h"
#include <d3d12.h>

/**
 * @file Object3d.h
 * @brief 読み込んだモデルを描画するクラス
 */

class Object3d
{
private:
	struct constBuffData {
		Color color;
		bool isUseDiffuseColor;
	};
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	Model* model_ = nullptr;
	bool isVisible_ = true;
	BillBoard billBoard;

	Color color_;
	bool isUseDiffuseColor_;
	// 頂点バッファの生成
	ComPtr<ID3D12Resource> colorBuff_ = nullptr;
public:
	WorldTransform WT_;
public:
	Object3d();
	~Object3d();
	//初期化
	void Init();
	//更新
	void Update(Camera* camera = Camera::scurrent_);
	//描画
	void Draw();
	void Draw(WorldTransform WT);

public:
	//セッター
	void SetModel(Model* model) { model_ = model; }
	void SetAmbient(const std::string& name, const Vector3& ambient) { model_->materials_.find(name)->second->SetAmbient(ambient); }
	void SetAmbient(const std::string& name, const Color& ambient) { model_->materials_.find(name)->second->SetAmbient(ambient); }
	void SetColor(const Color& color) { color_ = color; }
	void SetPos(const Vector3& pos) { WT_.position_ = pos; }
	void SetScale(const Vector3& scale) { WT_.scale_ = scale; }
	void SetRot(const Vector3& rot) { WT_.rotation_ = rot; }
	void SetIsVisible(bool flag) { isVisible_ = flag; }
	void SetShadowOffsetPos(const Vector3& pos) { model_->SetShadowOffsetPos(pos); }
	void SetShadowAtten(const Vector3& Atten) { model_->SetShadowAtten(Atten); }
	void SetShadowFactorAngle(const Vector2& FactorAngle) { model_->SetShadowFactorAngle(FactorAngle); }
	void SetIsUseDiffuseColor(bool flag) { isUseDiffuseColor_ = flag; }
	void SetParent(WorldTransform* parent) { WT_.parent_ = parent; }
public:
	//ゲッター
	WorldTransform* GetTransform() { return &WT_; }
	Vector3 GetPos() { return WT_.position_; }
	Vector3 GetScale() { return WT_.scale_; }
	Vector3 GetRot() { return WT_.rotation_; }
	bool GetIsVisible() { return isVisible_; }
	Model* GetModel() { return model_; }
};

