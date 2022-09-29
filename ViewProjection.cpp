#include <d3dx12.h>
#include "ViewProjection.h"

ViewProjection::ViewProjection()
{
	
	eye = { 0,0,0 };
	target = { 0,0,0 };
	up = { 0,0,0 };
}

void ViewProjection::SetEyePos(float x, float y, float z)
{
	eye.x = x;
	eye.y = y;
	eye.z = z;
}

void ViewProjection::SetEyePos(Vector3 pos)
{
	eye.x = pos.x;
	eye.y = pos.y;
	eye.z = pos.z;
}

void ViewProjection::SetTarget(float x, float y, float z)
{
	target.x = x;
	target.y = y;
	target.z = z;
}

void ViewProjection::SetTarget(Vector3 pos)
{
	target.x = pos.x;
	target.y = pos.y;
	target.z = pos.z;
}

void ViewProjection::SetUpVec(float x, float y, float z)
{
	up.x = x;
	up.y = y;
	up.z = z;
}

void ViewProjection::SetUpVec(Vector3 upVec)
{
	up.x = upVec.x;
	up.y = upVec.y;
	up.z = upVec.z;
}



void ViewProjection::Ini()
{
	directX_ = DirectXCommon::GetInstance();

	CreateConstBuffer();
	Map();
	Update();
}

void ViewProjection::Update()
{
	matView =
		XMMatrixLookAtLH(
			XMLoadFloat3(&eye),
			XMLoadFloat3(&target),
			XMLoadFloat3(&up));

	//�������e�s��̌v�Z
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),
		aspectRatio,
		0.1f, 1000.0f
	);

	// �萔�o�b�t�@�ɏ�������
	constMap->view = matView;
	constMap->projection = matProjection;
	constMap->cameraPos = eye;
}

XMMATRIX ViewProjection::GetMatView()
{
	return matView;
}

XMMATRIX ViewProjection::GetMatProjection()
{
	return matProjection;
}

void ViewProjection::CreateConstBuffer()
{
	HRESULT result;

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataViewProjection) + 0xff) & ~0xff);

	// �萔�o�b�t�@�̐���
	result = directX_->GetDevice()->CreateCommittedResource(
		&heapProps, // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));
}

void ViewProjection::Map()
{
	// �萔�o�b�t�@�Ƃ̃f�[�^�����N
	HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
}
