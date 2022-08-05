#pragma once
class PlayerBullet
{
public:
	PlayerBullet();

	void Ini(ID3D12Device* device, WorldTransform worldTransform, Vector3 velocity);

	void Update(ViewProjection viewProjection);

	bool IsDead() const { return isDead_; }

	void Draw();
private:
	void Move();

	WorldTransform worldTransform_;
	Object3d model_{};

	Vector3 velocity_;

	Controller* cInput;

	uint32_t texture;

	//寿命
	static const int32_t lifeTimer = 60 * 5;
	int32_t deathTimer_ = lifeTimer;
	//デスフラグ
	bool isDead_ = false;

};

