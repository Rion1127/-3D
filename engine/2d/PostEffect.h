#pragma once
#include "Sprite.h"

class PostEffect :
	public Sprite
{
public:
	PostEffect();

	void Draw();
private:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	ComPtr<ID3D12Resource> texBuff_;
	ComPtr<ID3D12DescriptorHeap> descHeapSRV_;
};

