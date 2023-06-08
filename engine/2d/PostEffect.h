#pragma once
#include "Sprite.h"

class PostEffect :
	public Sprite
{
public:
	PostEffect();

	void Draw();
	//シーン前処理
	void PreDrawScene();
	//シーン描画後処理
	void PostDrawScene();
private:
	void CreateTexBuff();
	void CreateSRV();
	void CreateRTV();
	void CreateDepthBuff();
	void CreateDSV();
public:
	void SetTexture(Texture texture) { texture_ = texture; }
private:


	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	ComPtr<ID3D12Resource> texBuff_;
	ComPtr<ID3D12DescriptorHeap> descHeapSRV_;

	//深度バッファ
	ComPtr<ID3D12Resource> depthBuff;
	//RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
private:
	//画面クリアカラー
	static const float clearColor[4];
};

