#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include "DirectX.h"

#define BLEND_ALPHA		0x0000
#define BLEND_SUB		0x0001
#define BLEND_NEGA		0x0002
#define BLEND_NORMAL	0x0003

class Pipeline
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Ini();
	/// <summary>
	/// ブレンド設定
	/// </summary>
	/// <param name="BLEND_ALPHA">アルファブレンド</param>
	/// <param name="BLEND_SUB">減算合成</param>
	/// <param name="BLEND_NEGA">色反転合成</param>
	/// <param name="BLEND_NORMAL">半透明合成</param>
	void SetBlend(int blend);

	ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }

	ID3D12PipelineState* gerPipelineState() { return pipelineState.Get(); }

private:
	void SetInputLayout();

	void SetRasterizer();

	void SetShader();

	void SetRootSignature();

	void SetOther();

	void BlendIni();
	
	D3D12_STATIC_SAMPLER_DESC SetSAMPLER_DESC();

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;
	// パイプランステートの生成
	ComPtr<ID3D12PipelineState> pipelineState;
	// パイプランステートの生成
	ComPtr<ID3D12PipelineState> pipelineState2;
	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	//コマンドリストを格納する
	static DirectXCommon* directX_;

	std::vector<D3D12_INPUT_ELEMENT_DESC>inputLayout;

	ComPtr < ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
	ComPtr < ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
	ComPtr < ID3DBlob> errorBlob = nullptr; // エラーオブジェクト
};

