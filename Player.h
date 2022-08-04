#pragma once
#include "PlayerBullet.h"
class Player
{
public:
	Player();

	void Ini(ID3D12Device* device);

	void Update(ID3D12Device* device,ViewProjection viewProjection);
	

	void Draw(uint32_t graph);
private:
	void Move();

	void Shot(ID3D12Device* device);

	WorldTransform worldTransform;
	Object3d model_{};

	Controller* cInput;

	PlayerBullet bullet;

	//Vector2 mainSpeed;

};

