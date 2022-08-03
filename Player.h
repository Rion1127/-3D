#pragma once
class Player
{
public:
	Player();

	void Ini(ID3D12Device* device);

	void Update(ViewProjection viewProjection);
	

	void Draw(uint32_t graph);
private:
	void Move();

	WorldTransform worldTransform;
	Object3d model_{};

	Controller* cInput;

	//Vector2 mainSpeed;

};

