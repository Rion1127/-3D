#pragma once
#include "Object3d.h"

class Player
{
public:
	void Initialize();
	void Update();
	void Draw();

	Vector3 GetPos() {
		return pos_;
	}

	Vector3 GetOffsetPos() {
		return offsetPos_;
	}

	void SetIsFever(bool isFever) { isFever_ = isFever; }

private:
	const float MOVE_RADIUSE = 60.f;
	const float PLAYER_Y = 40.f;
	const float SPEED = 0.3f;

	std::unique_ptr<Object3d> playerObj_;
	Vector3 pos_;
	Vector3 offsetPos_;	//糸を垂らす位置

	bool isFever_;
};

