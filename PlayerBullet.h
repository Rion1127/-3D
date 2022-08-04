#pragma once
class PlayerBullet
{
public:
	PlayerBullet();

	void Ini(ID3D12Device* device, WorldTransform worldTransform, ViewProjection viewProjection);

	void Update(ViewProjection viewProjection);


	void Draw();
private:
	void Move();

	WorldTransform worldTransform_;
	Object3d model_{};

	Controller* cInput;

	uint32_t texture;

};

