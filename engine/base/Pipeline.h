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

//�T���v���[�f�X�N��ݒ�
D3D12_STATIC_SAMPLER_DESC SetSAMPLER_DESC();

/// <summary>
/// �u�����h�ݒ�
/// </summary>
/// <param name="BLEND_ALPHA">�A���t�@�u�����h</param>
/// <param name="BLEND_SUB">���Z����</param>
/// <param name="BLEND_NEGA">�F���]����</param>
/// <param name="BLEND_NORMAL">����������</param>
void SetBlend(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc, int blend);

class Pipeline
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//object3d������
	void Object3dIni(int blend);
private:
#pragma region object3D
	void object3DSetInputLayout();

	void object3DSetRasterizer();

	void object3DSetShader();

	void object3DSetRootSignature();

	void object3DSetOther();

#pragma endregion
public:
	//Sprite������
	void SpriteIni(int blend);
private:
#pragma region Sprite
	void SpriteSetInputLayout();

	void SpriteSetRasterizer();

	void SpriteSetShader();

	void SpriteSetRootSignature();

	void SpriteSetOther();


#pragma endregion
public:
	//Sprite������
	void ToonIni(int blend);
private:
#pragma region Sprite
	void ToonSetInputLayout();

	void ToonSetRasterizer();

	void ToonSetShader();

	void ToonSetRootSignature();

	void ToonSetOther();
public:
	ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }

	ID3D12PipelineState* gerPipelineState() { return pipelineState.Get(); }

private:


	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;
	// �p�C�v�����X�e�[�g�̐���
	ComPtr<ID3D12PipelineState> pipelineState;
	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	//�R�}���h���X�g���i�[����
	static RDirectX* directX_;

	std::vector<D3D12_INPUT_ELEMENT_DESC>inputLayout;

	ComPtr < ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr < ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr < ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g
};


class ParticlePipeline {
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Init(int blend);

	ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }

	ID3D12PipelineState* gerPipelineState() { return pipelineState.Get(); }

private:
	void SetInputLayout();

	void SetRasterizer();

	void SetShader();

	void SetRootSignature();

	void SetOther();

	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;
	// �p�C�v�����X�e�[�g�̐���
	ComPtr<ID3D12PipelineState> pipelineState;
	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	//�R�}���h���X�g���i�[����
	static RDirectX* directX_;

	std::vector<D3D12_INPUT_ELEMENT_DESC>inputLayout;

	ComPtr < ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr < ID3DBlob> gsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr < ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr < ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g
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
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;
	// �p�C�v�����X�e�[�g
	ComPtr<ID3D12PipelineState> pipelineStateAdd;
	ComPtr<ID3D12PipelineState> pipelineStateSub;
	ComPtr<ID3D12PipelineState> pipelineStateNega;
	ComPtr<ID3D12PipelineState> pipelineStateAlpha;

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	std::vector<D3D12_ROOT_PARAMETER> rootParams_;

	ComPtr<ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> gsBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g
	
	
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
	ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }

	ID3D12PipelineState* GetPipelineStateAdd() { return pipelineStateAdd.Get(); }
	ID3D12PipelineState* GetPipelineStateSub() { return pipelineStateSub.Get(); }
	ID3D12PipelineState* GetPipelineStateNega() { return pipelineStateNega.Get(); }
	ID3D12PipelineState* GetPipelineStateAlpha() { return pipelineStateAlpha.Get(); }
};