#include "Ranking.h"

Ranking* Ranking::GetInstance(void)
{
	static Ranking ins;

	return &ins;
}

void Ranking::Init()
{
	for (size_t i = 0; i < peopleNumSprite_.size(); i++)
	{
		peopleNumSprite_[i].SetTexture(TextureManager::GetInstance()->GetTexture("PeopleNum"));
		peopleNumSprite_[i].SetScale({ 0.5f,0.5f });
	}

	newScoreSprite_.SetPos(Vector2(450.f, 230.f));
	newScoreSprite_.SetIconTexure(TextureManager::GetInstance()->GetTexture("Score"));
	newScoreSprite_.SetNumberLocalPos(Vector2(400,0));

	newScoreSprite_.SetTextColor(Color(255,255,255,255));

	peopleNumSprite_[3].SetPos(Vector2(1000.f, 230.f));

	Vector2 centerPos = {
		WinAPI::GetWindowSize().x / 2.f,
		0
	};

	
	for (int32_t i = 0; i < sprite_.size(); i++) {
		sprite_[i].SetNumberLocalPos(Vector2(20, 0));
		sprite_[i].SetTextLocalPos(Vector2(-80, 0));

		if (i == 0) {
			sprite_[i].SetIconTexure(TextureManager::GetInstance()->GetTexture("1st"));
			sprite_[i].SetPos(Vector2(centerPos.x / 2.5f, 500.f));

			peopleNumSprite_[i].SetPos(Vector2(centerPos.x / 2.5f + 150.f, 500.f));
		}
		else if(i == 1){
			sprite_[i].SetIconTexure(TextureManager::GetInstance()->GetTexture("2nd"));
			sprite_[i].SetPos(Vector2(centerPos.x, 500.f));

			peopleNumSprite_[i].SetPos(Vector2(centerPos.x + 150.f, 500.f));
		}
		else if (i == 2) {
			sprite_[i].SetIconTexure(TextureManager::GetInstance()->GetTexture("3rd"));
			sprite_[i].SetPos(Vector2(centerPos.x * 1.6f, 500.f));

			peopleNumSprite_[i].SetPos(Vector2(centerPos.x * 1.6f + 150.f, 500.f));
		}
	}

	easingTimer_.SetLimitTime(240);
}

void Ranking::Update()
{
	for (int32_t i = 0; i < sprite_.size(); i++) {
		sprite_[i].SetRescueNum(ranking_[i],false);
		sprite_[i].Update();
	}
	for (int32_t i = 0; i < peopleNumSprite_.size(); i++) {
		peopleNumSprite_[i].Update();
	}
	easingTimer_.AddTime();
	/////////////////
	//デバッグ用
	//newScore_ = 100;
	//////////////////
	easingNewScore_ =
		(int)Easing::Sine::easeOut(easingTimer_.GetTimeRate(),0, (float)newScore_,1.f );

	newScoreSprite_.SetRescueNum(easingNewScore_, false);
	newScoreSprite_.Update();
}

void Ranking::Draw()
{
	for (int32_t i = 0; i < sprite_.size(); i++) {
		sprite_[i].Draw();
	}

	for (int32_t i = 0; i < peopleNumSprite_.size(); i++) {
		peopleNumSprite_[i].Draw();
	}
	newScoreSprite_.Draw();
}

void Ranking::Reset()
{
	easingTimer_.Reset();
}

void Ranking::EasingSkip()
{
	easingTimer_.SetTime(easingTimer_.GetLimitTimer());
}
