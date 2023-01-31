#pragma once
#include <wrl.h>
#include "Vertices.h"
using namespace Microsoft::WRL;
class BoardObject
{
public:
	BoardObject();
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static BoardObject* GetInstance();

	static void Ini();

	void shadowIni();

	static void PreDraw();

	void shadowPreDraw();

	void ChangeColor(float x, float y, float z, float w);
	void ChangeColor(XMFLOAT4 color_);

	void Draw(WorldTransform* worldTransform, uint32_t descriptorSize);

	void ShadowDraw(WorldTransform* worldTransform, uint32_t descriptorSize);

private:
	

	// ���[�g�V�O�l�`��
	static ComPtr < ID3D12RootSignature> sRootSignature;
	// �p�C�v�����X�e�[�g�̐���
	static ComPtr<ID3D12PipelineState> sPipelineState;

	//�R�}���h���X�g���i�[����
	static DirectXCommon* sdirectX_ ;
	//���_�f�[�^
	static Vertices sVertices_;
};

