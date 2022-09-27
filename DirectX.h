#pragma once
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")
#include <wrl.h>
#include <dxgi1_6.h>
#pragma comment(lib, "dxgi.lib")

#include "WinAPI.h"
class DirectX
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void DebugLayer();

	void Ini(WinAPI* winApi);

private:
	void DXGIIni();
	void CommandIni();
	void SwapChainIni();
	void DepthIni();
	ComPtr<ID3D12Device> device;
	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapChain = nullptr;
	ComPtr<ID3D12CommandAllocator> cmdAllocator = nullptr;
	ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
	ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
	ComPtr<ID3D12DescriptorHeap> rtvHeap = nullptr;

	ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;

	WinAPI* winApi_ = nullptr;
};

