#pragma once
class Enemy
{
public:
	void Ini(ID3D12Device* device);
	void Update(ViewProjection viewprojection);
	void Draw();


private:
	void Move();
	void Approach();
	void Leave();
	WorldTransform worldTransoform;
	Object3d model_{};

	uint32_t texture;

	//行動フェーズ
	enum class Phase{
		Approach,	//接近する
		Leave,		//離脱する
	};
	Phase phase_;

};

