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

	//�s���t�F�[�Y
	enum class Phase{
		Approach,	//�ڋ߂���
		Leave,		//���E����
	};
	Phase phase_;

};

