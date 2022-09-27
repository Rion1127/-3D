#pragma once
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d12.h>
#include <d3dx12.h>
#pragma comment(lib, "d3d12.lib")
#include <wrl.h>
#include <dxgi1_6.h>
#pragma comment(lib, "dxgi.lib")
#include <d3d12sdklayers.h>

#include "WinAPI.h"
class DirectXCommon
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static DirectXCommon* GetInstance();

	ID3D12Device* GetDevice() { return device.Get(); }
	ID3D12GraphicsCommandList* GetCommandList() { return commandList.Get(); }

	void Ini(WinAPI* winApi);

private:
	// DXGI�f�o�C�X������
	void InitializeDXGIDevice();
	// �X���b�v�`�F�[���̐���
	void CreateSwapChain();
	// �R�}���h�֘A������
	void InitializeCommand();
	// �����_�[�^�[�Q�b�g����
	void CreateFinalRenderTargets();
	// �[�x�o�b�t�@����
	void CreateDepthBuffer();
	/// �t�F���X����
	void CreateFence();
	ComPtr<ID3D12Device> device;
	ComPtr<IDXGIFactory7> dxgiFactory_;
	ComPtr<IDXGISwapChain4> swapChain;
	ComPtr<ID3D12CommandAllocator> commandAllocator_;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandQueue> commandQueue_;
	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	//�o�b�N�o�b�t�@
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	ComPtr<ID3D12Resource> depthBuffer_;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;

	WinAPI* winApi_ = nullptr;
	// �t�F���X�̐���
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;
	
public:
	// �`��O����
	void PreDraw();
	// �`��㏈��
	void PostDraw();
	// �����_�[�^�[�Q�b�g�̃N���A
	void ClearRenderTarget();
	// �[�x�o�b�t�@�̃N���A
	void ClearDepthBuffer();

};

