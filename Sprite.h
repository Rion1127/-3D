#pragma once
#include <wrl.h>
#include <vector>
#include "math.h"
#include "DirectX.h"
class Sprite
{
public:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Ini();
	//���W��������
	void SetPos(Vector2 pos);
	//��]������
	void SetRot(float rot);
	//�X�P�[���ݒ�
	void SetScale(Vector2 scale);

	void ChangeColor(float x, float y, float z, float w) {
		color_ = {x, y, z, w};
	};
	void ChangeColor(DirectX::XMFLOAT4 color) {
		color_ = color;
	};

	Vector2 GetScale() { return Scale_; }
	Vector2 GetPos() { return pos_; }

	void PreDraw();
	//�摜�T�C�Y�����擾(�`����W�͒��S)
	void Draw(UINT descriptorSize);
	//�摜�̒��_�f�[�^�������Ŏw��
	void Draw(float LuX, float LuY, float RuX, float RuY, float LdX, float LdY, float RdX, float RdY, UINT descriptorSize);
private:
	DirectXCommon* directX_;
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
	//////////////////////////��ŃN���X��
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;
	// �p�C�v�����X�e�[�g�̐���
	ComPtr<ID3D12PipelineState> pipelineState;
	////////////////////////////

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	ComPtr<ID3D12GraphicsCommandList> commandList_;
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataMaterial {
		XMFLOAT4 color;
	};
	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	struct ConstBufferDataTransform {
		XMMATRIX mat;
	};
	ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	ConstBufferDataTransform* constMapTransform = nullptr;
	XMMATRIX matProjection;

	// ���[���h�s��
	DirectX::XMMATRIX matWorld_{};
	//�F
	DirectX::XMFLOAT4 color_;
	//��]
	float rot_;
	//���W
	Vector2 pos_;
	//�X�P�[��
	Vector2 Scale_;
};

