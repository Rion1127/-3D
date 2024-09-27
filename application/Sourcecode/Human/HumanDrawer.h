#pragma once
#include "AssimpObjects.h"
#include "GameStatus.h"

class HumanDrawer
{
public:
	HumanDrawer(void) = default;
	~HumanDrawer(void) = default;
	void Init(HumanState* state);
	void Update();
	void Draw();

	void SetPos(Vector3 pos) {
		pos_ = pos;
		dirty_ = true;
	}
	void SetScale(Vector3 scale) {
		scale_ = scale;
	}
	void SetAnimationTimer(int32_t time) {
		animationTimer_ = time;
	}
	void SetRepeatAnimation(bool flag) {
		isRepeatAnimation_ = flag;
	}
	void SetActiveShadow(bool flag) {
		objects_.isActiveShadow = flag;
	}
	void SetDirVec(Vector2 dirVec) {
		dirVec_ = dirVec;
	}
	Vector3 GetPos() { return pos_; }
private:
	HumanState* state_;
	HumanState prevState_;
	AssimpObjects<HumanState> objects_;

	int32_t animationTimer_;
	Vector3 pos_;
	Vector3 scale_ = { 1,1,1 };
	Vector2 dirVec_;
	bool isRepeatAnimation_;
	bool dirty_;
};
