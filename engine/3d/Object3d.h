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
	Model* model_ = nullptr;

	Vector3 pos_;
	Vector3 rot_;
	Vector3 scale_;

	WorldTransform WT_;

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
	void SetModel(Model* model) { model_ = model; }
	void SetPos(Vector3 pos) { pos_ = pos; }
	void SetScale(Vector3 scale) { scale_ = scale; }
	void SetRot(Vector3 rot) { rot_ = rot; }

public:
	//ゲッター
	WorldTransform GetTransform() { return WT_; }
};

