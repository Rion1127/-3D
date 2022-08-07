#pragma once
#include "PlayerBullet.h"
#include <list>
#include <memory>
class Player
{
public:
	Player();
	~Player();

	void Ini(ID3D12Device* device);

	void Update(ID3D12Device* device,ViewProjection viewProjection);
	

	void Draw(uint32_t graph);
private:
	void Move();

	void Shot(ID3D12Device* device, ViewProjection viewProjection);

	WorldTransform worldTransform;
	Object3d model_{};

	Controller* cInput;

	std::list<std::unique_ptr<PlayerBullet>> bullets;

	//íeÇë≈ÇøèoÇ∑ä‘äu
	const float maxCoolTime = 10;
	float cooltime = maxCoolTime;

};

