#pragma once
#include "Field.h"

class FieldManager
{
	//シングルトン
private:
	FieldManager() = default;
	~FieldManager() = default;
public:
	//コピーコンストラクタ無効
	FieldManager(const FieldManager& obj) = delete;
	//代入演算子を無効
	FieldManager& operator=(const FieldManager& obj) = delete;

	static FieldManager* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();

	void TitleUpdate();
private:
	Field field_;
};

