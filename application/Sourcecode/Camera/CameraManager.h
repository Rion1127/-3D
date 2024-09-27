#pragma once
#include "Camera.h"

class CameraManager
{
	//シングルトン
private:
	CameraManager() = default;
	~CameraManager() = default;
public:
	//コピーコンストラクタ無効
	CameraManager(const CameraManager& obj) = delete;
	//代入演算子を無効
	CameraManager& operator=(const CameraManager& obj) = delete;

	static CameraManager* GetInstance();

public:
	void Initialize();
	void Update();

	Camera* GetCamera() {
		return &camera_;
	}

private:
	Camera camera_;
};

