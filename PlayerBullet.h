#pragma once
class PlayerBullet
{
public:
	PlayerBullet();

	void Ini(ID3D12Device* device, WorldTransform worldTransform);

	void Update(ViewProjection viewProjection);


	void Draw();
private:
	void Move();

	bool isAlive;

	WorldTransform worldTransform;
	Object3d model_{};

	uint32_t texture;
};

