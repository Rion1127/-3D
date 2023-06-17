#pragma once
#include <wrl.h>
#include <map>
#include <string>
#include "Vertices.h"
#include "DirectX.h"
#include "PipelineManager.h"
#include <unordered_map>
#include "LightGroup.h"
#include "Model.h"

using namespace Microsoft::WRL;
class Object3d
{
private:
	std::unique_ptr<Model> model_ = nullptr;

	Vector3 pos_;
	Vector3 rot_;
	Vector3 scale_;

	WorldTransform WT_;

	bool isVisible_ = false;

public:
	Object3d();
	~Object3d();
	//初期化
	void Init();
	//更新
	void Update();
	//描画
	void Draw();

public:
	//セッター
	void SetModel(std::unique_ptr<Model> model) { model_ = std::move(model); }
	void SetPos(const Vector3& pos) { pos_ = pos; }
	void SetScale(const Vector3& scale) { scale_ = scale; }
	void SetRot(const Vector3& rot) { rot_ = rot; }
	void SetParent(WorldTransform* p) { WT_.parent_ = p; }
public:
	//ゲッター
	WorldTransform* GetTransform() { return &WT_; }
	WorldTransform* Getparent() { return WT_.parent_; }
	Vector3 GetPos() { return pos_; }
	Vector3 GetRot() { return rot_; }
	Vector3 GetScale() { return scale_; }
	Model* GetModel() { return model_.get(); }
};

