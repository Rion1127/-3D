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

public:
	void Init();

	void Update();

	void Draw(WorldTransform WT_);

public:
	//セッター
	void SetModel(Model* model) { model_ = model; }
};

