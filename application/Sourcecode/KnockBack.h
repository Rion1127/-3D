#pragma once
#include "GameStatus.h"
#include "Gravity.h"
class KnockBack
{
public:
	void Init();
	void Update();
	void DrawImGui();

	void GravityReset();
public:
	Vector3 GetKnockVelocity() const { return move_.attenVelocity; }
	float GetGravity() const { return grabity_.GetGravityValue(); }
public:
	void SetKnockVeclocity(const Vector3 knockVelo) {
		move_.attenVelocity = {
			knockVelo.x,
			0,
			knockVelo.z
		};
		grabity_.SetGrabity(knockVelo.y);
	}
private:
	MoveInfo3d_t move_;
	Gravity grabity_;
};

