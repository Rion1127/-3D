#pragma once
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")
#include <wrl.h>
#include <dxgi1_6.h>
#pragma comment(lib, "dxgi.lib")

#include "WinAPI.h"
class DirectXCommon
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static DirectXCommon* GetInstance();

	void DebugLayer();

	void Ini(WinAPI* winApi);

private:
	void DXGIIni();
	void CommandIni();
	void SwapChainIni();
	void DepthIni();
	void CreateFence();
	ComPtr<ID3D12Device> device;
	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapChain = nullptr;
	ComPtr<ID3D12CommandAllocator> cmdAllocator = nullptr;
	ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
	ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
	ComPtr<ID3D12DescriptorHeap> rtvHeap = nullptr;
	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	//バックバッファ
	std::vector<ComPtr<ID3D12Resource>> backBuffers;

	ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;

	WinAPI* winApi_ = nullptr;
	// フェンスの生成
	ComPtr<ID3D12Fence> fence = nullptr;
	UINT64 fenceVal = 0;

public:
	//毎フレーム処理の最初に書く
	void Updata();

	//毎フレーム処理の最後に書く
	void ResourceBarrier();
private:
	// 1.リソースバリアで書き込み可能に変更
	D3D12_RESOURCE_BARRIER barrierDesc{};
};

