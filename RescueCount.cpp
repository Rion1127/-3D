#include "RescueCount.h"

void ValueSprite::Init(int32_t digit, const std::string& textureName, bool isZeroDisplay)
{
	digitNum_ = digit;
	isZeroDisplay_ = isZeroDisplay;
	sprite_.resize(digitNum_);
	digit_.resize(digitNum_);

	for (auto& sprite : sprite_) {
		sprite.SetTexture(TextureManager::GetInstance()->GetTexture(textureName));
		sprite.SetSplitNum(Vector2(10, 1));
		sprite.SetTex_Size(Vector2(128, 128));

	}
	interval_ = 64;

	
	int32_t powValue = (int32_t)pow(10, digitNum_);
	maxValue_ = powValue - 1;
	
}

void ValueSprite::Update()
{
	value_ = Clamp(value_, minValue_, maxValue_);
	//桁ごとに分ける
	digit_ = GetDigitNum(value_, digitNum_);
	for (int32_t i = 0; i < sprite_.size(); i++) {
		//if (digit_[i] == 0 && i == sprite_.size() - 1)continue;
		Vector2 pos = {
			pos_.x + (i * interval_),
			pos_.y
		};
		sprite_[i].SetPos(pos);
		Vector2 scale = {
			scale_.x / 10.f,
			scale_.y,
		};
		sprite_[i].SetScale(scale);


		//画像のUV座標
		Vector2 leftTop = {
			(sprite_[i].GetTexture().size_.x / 10.f) * digit_[i],
			0,
		};
		sprite_[i].SetTex_LeftTop(leftTop);

		sprite_[i].SetColor(texColor_);

		sprite_[i].Update();
	}
	prevValue_ = value_;
}

void ValueSprite::Draw()
{
	for (auto& sprite : sprite_) {
		sprite.Draw();
	}
}

void ValueSprite::DigitUpdate()
{
}

int32_t ValueSprite::GetDigitNumber(int32_t number, int32_t digit)
{
	// digitの値が不正な場合、-1を返す
	if (digit < 1 || digit >(int)std::log10(number) + 1) {
		return 0;
	}

	// numberをdigit桁目から1桁目まで順に取り出す
	int num = number;
	for (int i = 1; i < digit; i++) {
		num /= 10;
	}

	// digit桁目の数値を返す
	return num % 10;
}

std::vector<int32_t> ValueSprite::GetDigitNum(int32_t num, int32_t digitNum)
{
	std::vector<int32_t> result;

	for (int32_t i = digitNum; i > 0; i--) {
		result.emplace_back(GetDigitNumber(num, i));
	}

	return result;
}