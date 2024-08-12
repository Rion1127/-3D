#pragma once
#include <wrl.h>
#include <map>
#include <string>
#include <memory>
#include <d3d12.h>

#include "DirectXTex/d3dx12.h"
#include <cassert>
#include <vector>
#include <DirectXTex.h>
#include "Vector2.h"

/**
 * @file Texture.h
 * @brief DirectTexを用いた画像読み込み・管理
 */

struct Texture {
	//テクスチャバッファの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff = nullptr;
	//テクスチャファイル名
	std::string fileName_;
	//設定したテクスチャの名前
	std::string textureName_;
	//テクスチャアドレス
	uint32_t textureHandle;
	//テクスチャサイズ
	Vector2 size_;
	//Gpuハンドル
	UINT64 gpuHandle_;
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle_;
};

struct ImGuiData {
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
};

class TextureManager
{
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//SRVの最大個数
	const size_t kMaxSRVCount = 2056;
	//テクスチャの情報を格納
	std::map < std::string, std::unique_ptr<Texture>> texData{};
	//次に格納する場所のアドレス
	uint32_t textureHandle;
	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle{};
	//設定をもとにSRV用でスクリプタヒープを生成
	ComPtr<ID3D12DescriptorHeap> srvHeap{};
	UINT descriptorSize_;
public:
	static TextureManager* GetInstance();

	void Ini();

	void LoadGraph(const std::string& fileName, const std::string& name);

	void SetGraphicsDescriptorTable(UINT descriptorSize);
	void SetGraphicsDescriptorTable(UINT index, UINT descriptorSize);

	Texture* GetTexture(const std::string& name);
	Texture* GetTexture(UINT64 gpuHandle);

	ComPtr<ID3D12DescriptorHeap> GetSrvHeap() { return srvHeap; }
	UINT GetDescriptorSize() { return descriptorSize_; }
	ImGuiData GetImGuiData();
private:
	TextureManager() {};
};

std::string FileExtension(const std::string& path);

