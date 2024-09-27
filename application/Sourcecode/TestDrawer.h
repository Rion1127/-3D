#pragma once
#include "AssimpObjects.h"
#include "GameStatus.h"

class TestDrawer
{
public:
	TestDrawer(void) = default;
	~TestDrawer(void) = default;
	void Init();
	void Update();
	void Draw();

	void SetPos(Vector3 pos) {
		if (objects_.currentObj_ == nullptr) return;
		objects_.currentObj_->SetPos(pos);
	}
	void SetAnimationTimer(int32_t time) {
		objects_.currentObj_->SetAnimationTime(time);
	}
private:
	AssimpObjects<TestState> objects_;
};
