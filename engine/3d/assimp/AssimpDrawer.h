#pragma once
#include "AssimpObjects.h"
#include "GameStatus.h"

class AssimpDrawer
{
public:
	AssimpDrawer(void) = default;
	~AssimpDrawer(void) = default;
	void Init(TestState* state);
	void Update();
	void Draw();

	void SetPos(Vector3 pos) { pos_ = pos; dirty_ = true;}	//座標代入
	void SetScale(Vector3 scale) { scale_ = scale; }		//スケール代入
	void SetAnimationTimer(int32_t time) { animationTimer_ = time; }	//アニメーションの時間
	void SetRepeatAnimation(bool flag) { isRepeatAnimation_ = flag; }	//アニメーション繰り返しフラグ
	void SetDirVec(Vector2 dirVec) { dirVec_ = dirVec; }	//モデルの向き
	Vector3 GetPos() const { return pos_; }
private:
	TestState* state_;
	TestState prevState_;
	AssimpObjects<TestState> objects_;
	//アニメーションの時間
	int32_t animationTimer_ = 60;
	Vector3 pos_;
	Vector3 scale_ = {1,1,1};
	//モデルの向き
	Vector2 dirVec_;
	//アニメーション繰り返しフラグ
	bool isRepeatAnimation_;
	bool dirty_;
};
