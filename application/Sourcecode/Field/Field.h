#pragma once
#include "Object3d.h"

class Field
{
public:
	void Initialize();
	void Update();
	void Draw();

	void TitleUpdate();
private:
	std::unique_ptr<Object3d> obj_;
	Vector3 pos_;

	float rot_;
};