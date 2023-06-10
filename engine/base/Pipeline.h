#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <d3dx12.h>
#pragma comment(lib, "d3dcompiler.lib")
#include "DirectX.h"

enum BlendNum {
	ADD,
	SUB,
	NEGA,
	ALPHA,
};

//サンプラーデスクを設定
D3D12_STATIC_SAMPLER_DESC SetSAMPLER_DESC();

/// <summary>
/// ブレンド設定
/// </summary>
/// <param name="BLEND_ALPHA">アルファブレンド</param>
/// <param name="BLEND_SUB">減算合成</param>
/// <param name="BLEND_NEGA">色反転合成</param>
/// <param name="BLEND_NORMAL">半透明合成</param>
void SetBlend(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc, uint32_t blend);

class Pipeline
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	ID3D12RootSignature* GetRootSignature() { return rootSignature_.Get(); }

	ID3D12PipelineState* gerPipelineState() { return pipelineState_.Get(); }

private:


	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature_;
	// パイプランステートの生成
	ComPtr<ID3D12PipelineState> pipelineState_;
	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_{};
	//コマンドリストを格納する
	static RDirectX* directX_;

	std::vector<D3D12_INPUT_ELEMENT_DESC>inputLayout_;

	ComPtr < ID3DBlob> vsBlob_ = nullptr; // 頂点シェーダオブジェクト
	ComPtr < ID3DBlob> psBlob_ = nullptr; // ピクセルシェーダオブジェクト
	ComPtr < ID3DBlob> errorBlob_ = nullptr; // エラーオブジェクト
};



enum CULL_MODE {
	NONE = 1,
	FRONT = 2,
	BACK = 3
};
enum TOPOLOGY_TYPE
{
	TOPOLOGY_UNDEFINED = 0,
	TOPOLOGY_POINT = 1,
	TOPOLOGY_LINE = 2,
	TOPOLOGY_TRIANGLE = 3,
	TOPOLOGY_PATCH = 4
};
enum WRIGHT_MASK {
	DEPTH_WRITE_MASK_ZERO = 0,
	DEPTH_WRITE_MASK_ALL = 1
};

class PipelineObject {
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature_;
	// パイプランステート
	ComPtr<ID3D12PipelineState> pipelineStateAdd_;
	ComPtr<ID3D12PipelineState> pipelineStateSub_;
	ComPtr<ID3D12PipelineState> pipelineStateNega_;
	ComPtr<ID3D12PipelineState> pipelineStateAlpha_;

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout_;
	std::vector<D3D12_ROOT_PARAMETER> rootParams_;

	ComPtr<ID3DBlob> vsBlob_ = nullptr; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob_ = nullptr; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob_ = nullptr; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob_ = nullptr; // エラーオブジェクト
	
	
public:
	
	enum ShaderType {
		VS,
		PS,
		GS
	};
public:
	std::string name_;

	void Create(BlendNum blendNum, CULL_MODE cullmode, TOPOLOGY_TYPE topologytype, WRIGHT_MASK depthWriteMasc);

	void Setshader(LPCWSTR fileName,ShaderType shadertype);

	void AddrootParams();
	void AddInputLayout(const char* semanticName, DXGI_FORMAT format);
public:
	ID3D12RootSignature* GetRootSignature() { return rootSignature_.Get(); }

	ID3D12PipelineState* GetPipelineStateAdd() { return pipelineStateAdd_.Get(); }
	ID3D12PipelineState* GetPipelineStateSub() { return pipelineStateSub_.Get(); }
	ID3D12PipelineState* GetPipelineStateNega() { return pipelineStateNega_.Get(); }
	ID3D12PipelineState* GetPipelineStateAlpha() { return pipelineStateAlpha_.Get(); }
};