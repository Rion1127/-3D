#pragma once
#include "PlayerBullet.h"
#include <list>
#include <memory>
#include <d3d12.h>
#include "ViewProjection.h"
#include "mInput.h"
#include "Object3d.h"
class Player
{
public:
	Player();
	~Player();

	void Ini();

	void Update(ID3D12Device* device,ViewProjection viewProjection);
	//�����蔻������o������Ăяo�����֐�
	void OnCollision();

	void Draw(uint32_t graph);

	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets; }
private:
	void Move();

	void Shot(ID3D12Device* device, ViewProjection viewProjection);

	WorldTransform worldTransform;
	Object3d model_{};

	Controller* cInput;

	std::list<std::unique_ptr<PlayerBullet>> bullets;



	//�e��ł��o���Ԋu
	const float maxCoolTime = 10;
	float cooltime = maxCoolTime;

};

