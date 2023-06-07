#pragma once
#include <wrl.h>
#include <map>
#include <string>
#include <memory>
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")
#include <cassert>
#include <DirectXTex.h>
#include "Vector2.h"
using namespace DirectX;

struct Texture {
	//テクスチャ
	std::string handleName_;
	//リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};			//設定構造体
	//テクスチャバッファの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff = nullptr;

	uint32_t textureHandle;
	std::string textureName_;
	Vector2 size_;
};

class TextureManager
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static TextureManager* GetInstance();

	void Ini(ID3D12Device* device);

	uint32_t LoadGraph(const std::string& HandleName, const std::string& name);

	void SetGraphicsDescriptorTable(UINT descriptorSize);

	Texture* GetTexture(std::string name);

	//設定をもとにSRV用でスクリプタヒープを生成
	ComPtr<ID3D12DescriptorHeap> srvHeap{};
private:
	
	ComPtr<ID3D12Device> device_{};	//デバイスを格納
	//テクスチャの情報を格納
	std::map < std::string , std::unique_ptr<Texture>> texData{};
	////テクスチャバッファを格納
	//std::map <char, int> test;
	//画像を保存するアドレスを格納する
	uint32_t textureHandle;

//SRVの最大個数
	const size_t kMaxSRVCount = 2056;
	
	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle{};
	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};

};

std::string FileExtension(const std::string& path);

