#include "RescueCount.h"
#include "Util.h"

#pragma region RescueCount

RescueCount::RescueCount() {
	rescueCount_ = 0;
	rescueCounterSprite_.SetRescueNum(rescueCount_);
}

void RescueCount::Update()
{
	rescueCounterSprite_.Update();
}

void RescueCount::AddCount()
{
	//コンボを加算
	rescueCount_++;
	//上限と下限を超えないようにする
	rescueCount_ = Clamp(rescueCount_, 0, maxRescueNum_);
	rescueCounterSprite_.SetRescueNum(rescueCount_);
}
void RescueCount::Reset()
{
	rescueCount_ = 0;
	rescueCounterSprite_.SetRescueNum(rescueCount_);
}
void RescueCount::Draw()
{
	rescueCounterSprite_.Draw();
}
#pragma endregion

#pragma region CoinSprite
RescueCountSprite::RescueCountSprite()
{
	spriteInterval_ = 40;
	rescueNum_ = 0;
	scaleSubSpeed_ = 0.08f;
	reactionScale_ = 2.5f;
	baseScale_ = Vector2(0.5f, 0.5f);
	
	numberScale_ = Vector2(0.1f, 1.f) * baseScale_;
	digitNum_ = 3;
	numberLocalPos_ = Vector2(200, 0);
	int32_t index = 0;
	for (auto& sprite : numSprite_) {
		sprite.SetTexture(TextureManager::GetInstance()->GetTexture("Number"));
		sprite.SetSplitNum(Vector2(10, 1));
		sprite.SetTex_Size(Vector2(128, 128));
		sprite.SetScale(numberScale_);
		Vector2 pos = {
			basePos_.x + (spriteInterval_ * index) + numberLocalPos_.x,
			basePos_.y

		};
		sprite.SetPos(pos);
		sprite.SetColor(texColor_);
		index++;
	}
	rescueIcon_.SetPos(basePos_ + textLocalPos_);
	rescueIcon_.SetScale(baseScale_);
	rescueIcon_.SetTexture(TextureManager::GetInstance()->GetTexture("Now_score"));

	
}

void RescueCountSprite::Update()
{
	auto digit = GetDigitNum(rescueNum_, digitNum_);
	for (int32_t i = 0; i < numSprite_.size(); i++) {
		//スケール
		auto scale = numSprite_[i].GetScale();
		MoveTo(numberScale_, scaleSubSpeed_, scale);
		numSprite_[i].SetScale(scale);
		//画像のUV座標
		Vector2 leftTop = {
			(numSprite_[i].GetTexture().size_.x / 10.f) * digit[i],
			0,
		};
		numSprite_[i].SetTex_LeftTop(leftTop);
		//座標
		Vector2 pos = {
			basePos_.x + (spriteInterval_ * i) + numberLocalPos_.x,
			basePos_.y
		};
		numSprite_[i].SetPos(pos);
		numSprite_[i].SetColor(texColor_);
		numSprite_[i].Update();
	}

	rescueIcon_.SetPos(basePos_ + textLocalPos_);
	rescueIcon_.Update();

}

void RescueCountSprite::Draw()
{
	rescueIcon_.Draw();
	for (auto& sprite : numSprite_) {
		sprite.Draw();
	}

}

void RescueCountSprite::SetRescueNum(int32_t num, bool isReaction)
{
	rescueNum_ = num;
	if(isReaction) ReactionInit();
}
void RescueCountSprite::ReactionInit()
{
	for (int32_t i = 0; i < numSprite_.size(); i++) {
		auto scale = numberScale_ * reactionScale_;
		numSprite_[i].SetScale(scale);
	}
}
#pragma endregion
