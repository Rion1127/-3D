#include "mRootParameter.h"

void mRootParameter::Ini()
{
	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	//�f�X�N���v�^�����W�̐ݒ�
	descriptorRange.NumDescriptors = 1;					//��x�̕`��ɍD�����e�N�X�`����1���Ȃ̂�1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;				//�e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//���[�g�̃p�����[�^�ݒ�
	//�萔�o�b�t�@0��
	entity[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	entity[0].Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
	entity[0].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	entity[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_���猩����
	////�e�N�X�`�����W�X�^0��
	entity[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//���
	entity[1].DescriptorTable.pDescriptorRanges = &descriptorRange;					//�f�X�N���v�^�����W
	entity[1].DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
	entity[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_���猩����
	//�萔�o�b�t�@1��
	entity[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	entity[2].Descriptor.ShaderRegister = 1;					//�萔�o�b�t�@�ԍ�
	entity[2].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	entity[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_���猩����
}
