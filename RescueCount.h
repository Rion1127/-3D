#pragma once
#include <vector>
#include "Sprite.h"
class ValueSprite
{
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="digit">桁数</param>
	/// <param name="isZeroDisplay">十の位以上の0を表示する</param>
	void Init(int32_t digit, const std::string& textureName = "Number", bool isZeroDisplay = true);
	void Update();
	void Draw();

	void SetPos(Vector2 pos) { pos_ = pos; }
	void SetInterval(int32_t interval) { interval_ = interval; }
	void SetScale(Vector2 scale) { scale_ = scale; }
	void SetValue(int32_t value) { value_ = value; }
	void SetColor(Color color) { texColor_ = color; }
private:
	void DigitUpdate();
	int32_t GetDigitNumber(int32_t number, int32_t digit);
	std::vector<int32_t> GetDigitNum(int32_t num, int32_t digitNum);
protected:
	std::vector<Sprite> sprite_;
	//桁ごとの数字
	std::vector<int32_t> digit_;
	int32_t maxValue_;
	int32_t minValue_ = 0;
	int32_t value_;
	int32_t prevValue_;
	Vector2 pos_;
	Vector2 scale_ = { 1,1 };
	//桁数
	int32_t digitNum_;

	int32_t interval_;
	bool isZeroDisplay_;

	Color texColor_;
};

