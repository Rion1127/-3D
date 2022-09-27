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
	//�G�C���A�X�e���v���[�g
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
	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	//�o�b�N�o�b�t�@
	std::vector<ComPtr<ID3D12Resource>> backBuffers;

	ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;

	WinAPI* winApi_ = nullptr;
	// �t�F���X�̐���
	ComPtr<ID3D12Fence> fence = nullptr;
	UINT64 fenceVal = 0;

public:
	//���t���[�������̍ŏ��ɏ���
	void Updata();

	//���t���[�������̍Ō�ɏ���
	void ResourceBarrier();
private:
	// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
	D3D12_RESOURCE_BARRIER barrierDesc{};
};

