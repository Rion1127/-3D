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
	//“–‚½‚è”»’è‚ğŒŸo‚µ‚½‚çŒÄ‚Ño‚³‚ê‚éŠÖ”
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



	//’e‚ğ‘Å‚¿o‚·ŠÔŠu
	const float maxCoolTime = 10;
	float cooltime = maxCoolTime;

};

