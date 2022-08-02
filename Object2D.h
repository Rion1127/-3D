#pragma once
#include <wrl.h>
#include <vector>
#include "math.h"
class Object2D
{
public:

	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Ini(ID3D12Device* device);

	void PreDraw(ID3D12GraphicsCommandList* commandList);
	//�摜�T�C�Y�����擾
	void Draw(Vector2 pos,UINT descriptorSize);
	//�摜�̒��_�f�[�^�������Ŏw��
	void Draw(float LuX, float LuY, float RuX, float RuY, float LdX, float LdY, float RdX, float RdY, UINT descriptorSize);
private:
	struct Vertex {
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

#pragma region ���_�f�[�^
	//���_�f�[�^
	std::vector<Vertex> vertices;
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// ���_�o�b�t�@�̐���
	ComPtr<ID3D12Resource> vertBuff = nullptr;
#pragma endregion
#pragma region �C���f�b�N�X�f�[�^
	//�C���f�b�N�X�f�[�^
	std::vector<uint16_t> indices;
	// �C���f�b�N�X�o�b�t�@�̐���
	ComPtr<ID3D12Resource> indexBuff = nullptr;
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};
#pragma endregion


	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;
	// �p�C�v�����X�e�[�g�̐���
	ComPtr<ID3D12PipelineState> pipelineState;
	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	ComPtr<ID3D12GraphicsCommandList> commandList_;
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataMaterial {
		XMFLOAT4 color;
	};
	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

	struct ConstBufferDataTransform {
		XMMATRIX mat;
	};
	ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	ConstBufferDataTransform* constMapTransform = nullptr;
};

