#pragma once
#include "Timer.h"
#include "Sprite.h"
#include <array>

//下のCoinCounterで宣言をしてある
class RescueCountSprite
{
public:
	RescueCountSprite();
	void Update();
	void Draw();
public:
	//コインの数を代入する
	void SetRescueNum(int32_t num,bool isReaction = true);
	void SetPos(Vector2 pos) { basePos_ = pos; }
	void SetIconTexure(Texture* texture) { rescueIcon_.SetTexture(texture); }
	void SetNumberLocalPos(Vector2 localPos) { numberLocalPos_ = localPos; }
	void SetTextLocalPos(Vector2 localPos) { textLocalPos_ = localPos; }
	void SetColor(Color color) { texColor_ = color; }
private:
	//コンボが増えた時のリアクション
	void ReactionInit();
private:
	//数字のスプライト
	std::array<Sprite, 3> numSprite_;
	std::array<std::unique_ptr<Sprite>, 3> num_;
	//数字の左側のアイコン
	Sprite rescueIcon_;
	std::unique_ptr<Sprite> rescue_;
	//表示する数
	int32_t rescueNum_;
	float spriteInterval_;
	//基本のスケール
	Vector2 baseScale_;
	Vector2 numberScale_;
	//基本の座標
	Vector2 basePos_;
	//数字のローカル座標の座標
	Vector2 numberLocalPos_;
	//文字のローカル座標の座標
	Vector2 textLocalPos_;
	//サイズを小さくする時のスピード
	float scaleSubSpeed_;
	//数字が変化したときの大きくなるサイズ
	float reactionScale_;
	//数字の桁数
	int32_t digitNum_;

	Color texColor_;
};

class RescueCount
{
public:
	RescueCount();
	~RescueCount() = default;

	//更新
	void Update();
	//コンボ加算
	void AddCount();
	//コンボを0にする
	void Reset();
	//描画
	void Draw();

	void SetIconTexure(Texture* texture) { rescueCounterSprite_.SetIconTexure(texture); }
	void SetRescueNum(int32_t num, bool isReaction = true) { rescueCounterSprite_.SetRescueNum(num, isReaction); };
public:
	void SetPos(Vector2 pos) { rescueCounterSprite_.SetPos(pos); }
	void SetNumberLocalPos(Vector2 localPos) { rescueCounterSprite_.SetNumberLocalPos(localPos); }
	void SetTextLocalPos(Vector2 localPos) { rescueCounterSprite_.SetTextLocalPos(localPos); }
	void SetTextColor(Color color) { rescueCounterSprite_.SetColor(color); }
	int32_t GetRescueNum() const { return rescueCount_; }
private:
	int32_t rescueCount_;
	const int32_t maxRescueNum_ = 999;
	RescueCountSprite rescueCounterSprite_;

};