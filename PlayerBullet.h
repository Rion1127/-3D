#pragma once
class PlayerBullet
{
public:
	PlayerBullet();

	void Ini(ID3D12Device* device, WorldTransform worldTransform, Vector3 velocity);

	void Update(ViewProjection viewProjection);


	void Draw();
private:
	void Move();

	WorldTransform worldTransform_;
	Object3d model_{};

	Vector3 velocity_;

	Controller* cInput;

	uint32_t texture;

};

