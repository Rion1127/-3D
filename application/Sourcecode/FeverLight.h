#pragma once
#include "Object3d.h"
#include "Timer.h"
class FeverLight
{
public:
	FeverLight() = default;
	~FeverLight() = default;

	void Init();
	void Update();
	void Draw();
public:
	void SetIsFever(bool flag) { isFever_ = flag; }
private:
	struct LightObj {
		std::unique_ptr<Object3d> obj;
		Vector3 swayValue;
		Timer swayTimer;
	};
private:

	std::unique_ptr<Object3d> flower_;
	std::vector <LightObj> lights_;
	bool isFever_;

	Vector3 defaultPos_;
	Vector3 feverPos_;
	Vector3 pos_;
	Vector3 rot_;
	Vector3 lightScale_;
	Vector3 lightDefaultScale_;
	Timer easingTimer_;
};

