#pragma once
#include "Sprite.h"
#include "Timer.h"
class Camera2d
{
public:
	Camera2d();
	void Init();
	void Update();
	//スクロール開始位置を描画
	void DrawCol();
	void DrawImGui();
	void StartShake(int32_t time,float power);
	void Reset(const Vector2& playerPos);
public:
	void SetScrollLeftX(float x) { scrollLeftX_ = x; }
	void SetScrollRightX(float x) { scrollRightX_ = x; }
	void SetScrollUpX(float y) { scrollUpY_ = y; }
	void SetScrollDownY(float y) { scrollDownY_ = y; }

	void SetPlayerPos(const Vector2& playerPos) { playerPos_ = playerPos; }
public:
	static Camera2d* GetInstance();
	Vector2 GetScrollValue() { return scrollValue_; }
	Vector2 GetShakeValue() { return shakeValue_; }
	//スクロールとカメラシェイクを足した値を返す
	Vector2 GetAllValue() { return scrollValue_ + shakeValue_; }
	float GetZoomRate() { return zoomRate_; }
private:
	//カメラスクロール開始位置スプライト
	Sprite leftSprite_;
	Sprite rightSprite_;
	Sprite upSprite_;
	Sprite downSprite_;
	float scrollLeftX_;
	float scrollRightX_;
	float scrollUpY_;
	float scrollDownY_;
	Vector2 scrollValue_;
	float speed_;
	float debugSpeed_;
	Vector2 playerPos_;
	//カメラシェイク
	Timer shakeTimer_;
	bool isCameraShake_;
	Vector2 shakeValue_;
	float power_;
	//２Dの拡縮
	float zoomRate_ = 1.f;
	bool isDebug_;
	bool isColDisplay_;
	//リスポーン時一時的にスクロール開始位置を変更させる
	Timer respawnTimer_;
private:
	void CameraShakeUpdate();
	void DebugUpdate();
};

