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
	D3D12_ROOT_PARAMETER rootparam;
	//�萔�o�b�t�@0��
	rootparam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootparam.Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
	rootparam.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_���猩����
	entity.push_back(rootparam);
	////�e�N�X�`�����W�X�^0��
	rootparam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//���
	rootparam.DescriptorTable.pDescriptorRanges = &descriptorRange;					//�f�X�N���v�^�����W
	rootparam.DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
	rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_���猩����
	entity.push_back(rootparam);
	//�萔�o�b�t�@1��
	rootparam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootparam.Descriptor.ShaderRegister = 1;					//�萔�o�b�t�@�ԍ�
	rootparam.Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_���猩����
	entity.push_back(rootparam);
}
