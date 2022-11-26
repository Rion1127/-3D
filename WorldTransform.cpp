
#include "Util.h"
#include "WorldTransform.h"

WorldTransform::WorldTransform()
{
	
}

void WorldTransform::SetScale(float x, float y, float z)
{
	scale = { x,y,z };
}

void WorldTransform::SetRotation(float x, float y, float z)
{
	rotation = { x,y,z };
}

void WorldTransform::SetPosition(float x, float y, float z)
{
	position = { x,y,z };
}

void WorldTransform::AddScale(float x, float y, float z)
{
	scale.x += x;
	scale.y += y;
	scale.z += z;
}

void WorldTransform::AddRotation(float x, float y, float z)
{
	rotation.x += x;
	rotation.y += y;
	rotation.z += z;
}

void WorldTransform::AddPosition(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;
}

void WorldTransform::Ini()
{
	HRESULT result;

	directx_ = DirectXCommon::GetInstance();

	//�萔�o�b�t�@�̃q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//�萔�o�b�t�@�̃��\�[�X�ݒ�
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result = directx_->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform)
	);
	assert(SUCCEEDED(result));
	

	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));
}

void WorldTransform::Update(ViewProjection viewProjection, int isBillboard)
{
	XMMATRIX matScale, matRot, matTrans;

	//�X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(rotation.z);
	matRot *= XMMatrixRotationX(rotation.x);
	matRot *= XMMatrixRotationY(rotation.y);
	matTrans = XMMatrixTranslation(
		position.x, position.y, position.z);

	//���[���h�s��̍���
	matWorld = XMMatrixIdentity();	//�ό`�����Z�b�g
	//�r���{�[�h
	if (isBillboard == 1) {
		matWorld *= viewProjection.matBillboard;
	}
	else if (isBillboard == 2) {
		matWorld *= viewProjection.matBillboardY;
	}

	matWorld *= matScale;			//���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld *= matRot;				//���[���h�s��ɊJ�X�𔽉f
	matWorld *= matTrans;			//���[���h�s��ɕ��s�ړ��𔽉f

	//�e�I�u�W�F�N�g�������
	if (parent) {
		//�e�I�u�W�F�N�g�̃��[���h�s����|����
		matWorld *= parent->matWorld;
	}

	//�萔�o�b�t�@�փf�[�^�]��
	constMapTransform->mat = matWorld ;
	constMapTransform->viewProj = viewProjection.GetMatView() * viewProjection.GetMatProjection();
}

