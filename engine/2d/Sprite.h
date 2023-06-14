#pragma once
#include <wrl.h>
#include <vector>
#include "math.h"
#include "DirectX.h"
#include "PipelineManager.h"
#include <imgui.h>
#include <DirectXMath.h>
#include "Vector2.h"
#include "Texture.h"
class Sprite
{
public:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	
	static void StaticIni();
	void Ini(const std::string& guiname = "");
	//���W��������
	void SetPos(float x, float y) {
		pos_.x = x;
		pos_.y = y;
	};
	void SetPos(const Vector2& pos) { pos_ = pos; }
	//��]������
	void SetRot(float rot) { rot_ = rot; }
	//�X�P�[���ݒ�
	void SetScale(float x, float y) {
		Scale_.x = x;
		Scale_.y = y;
	};
	void SetScale(const Vector2& scale) { Scale_ = scale; }
	//�摜�̒��S�����߂�
	void SetAnchor(float x, float y) {
		anchorPoint_.x = x;
		anchorPoint_.y = y;
	};
	void SetAnchor(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint;}
	//�F�ύX
	void ChangeColor(float x, float y, float z, float w) {
		color_ = {x, y, z, w};
	};
	void ChangeColor(const XMFLOAT4& color) {
		color_ = color;
	};
	//���E���]
	void SetFlipX(bool flip) { isFlipX_ = flip; }
	//�㉺���]
	void SetFlipY(bool flip) { isFlipY_ = flip; }
	//�\���t���O
	void SetInvisivle(bool invisivle) { isInvisible_ = invisivle; }
	//�摜����ύX
	void SetTex_LeftTop(const Vector2& pos) { textureLeftTop_ = pos; }
	//�摜UV���W�ύX
	void SetTex_Size(const Vector2& pos) { textureSize_ = pos; }
	//�e�N�X�`��
	void SetTexture(Texture* texture) { texture_ = *texture; }

	Vector2 GetScale() { return Scale_; }
	Vector2 GetPos() { return pos_; }

	void SetImGui(bool flag) { isImguiDisplay_ = flag; }
	void DrawImGui();

	void Update();

	static void PreDraw();
	//�摜�T�C�Y�����擾(�`����W�͒��S)
	void Draw();
	//�摜�̒��_�f�[�^�������Ŏw��
	void Draw(float LuX, float LuY, float RuX, float RuY, float LdX, float LdY, float RdX, float RdY, UINT descriptorSize);
	/// <summary>
	/// �u�����h�ݒ�
	/// </summary>
	/// <param name="BLEND_ALPHA">�A���t�@�u�����h</param>
	/// <param name="BLEND_SUB">���Z����</param>
	/// <param name="BLEND_NEGA">�F���]����</param>
	/// <param name="BLEND_NORMAL">����������</param>
	static void SetBlend(uint32_t blend);

	static void AddAllNum() { SAllNum++; }
protected:
	void TransferVertex();
protected:
	static uint32_t SAllNum;
	uint32_t spriteNum_ = 0;

#pragma region ���_�f�[�^
	struct Vertex {
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};
	enum VertNum {
		LB,	//����
		LT,	//����
		RB,	//�E��
		RT	//�E��
	};
	//���_�f�[�^
	std::vector<Vertex> vertices_;
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	// ���_�o�b�t�@�̐���
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;

	Vertex* vertMap_ = nullptr;
#pragma endregion
#pragma region �C���f�b�N�X�f�[�^
	//�C���f�b�N�X�f�[�^
	std::vector<uint16_t> indices_;
	// �C���f�b�N�X�o�b�t�@�̐���
	ComPtr<ID3D12Resource> indexBuff_ = nullptr;
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView_{};
#pragma endregion

	ComPtr<ID3D12GraphicsCommandList> commandList_;
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataMaterial {
		XMFLOAT4 color;
	};
	ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;
	ConstBufferDataMaterial* constMapMaterial_ = nullptr;

	struct ConstBufferDataTransform {
		XMMATRIX mat;
	};
	ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;
	ConstBufferDataTransform* constMapTransform_ = nullptr;
	XMMATRIX matProjection_;

	
	DirectX::XMMATRIX matWorld_{};// ���[���h�s��
	
	DirectX::XMFLOAT4 color_;//�F
	
	float rot_;				//��]
	Vector2 pos_;			//���W
	Vector2 Scale_;			//�X�P�[��
	Vector2 anchorPoint_;	//�A���J�[�|�C���g
	bool isFlipX_ = false;	//���E�t���b�v
	bool isFlipY_ = false;	//�㉺�t���b�v
	bool isInvisible_ = false;					//��\���t���O
	Vector2 textureLeftTop_ = { 0.0f,0.0f };	//�e�N�X�`��������W
	Vector2 textureSize_ = { 0.f,0.f };			//�e�N�X�`���؂�o���T�C�Y
	bool isImguiDisplay_ = false;				//imgui�\���t���O
	uint32_t descriptorSize_ = 0;
	Texture texture_;		//�e�N�X�`��

#pragma region
	std::string guiName_;
	uint32_t clicked_ = 0;
	const char* gui_;

	std::string name_;
#pragma endregion
};

