#include "SpriteCB.hlsli"

//���_�V�F�[�_�̏o�͍\����
struct VSOutput
{
    // ���_���W
    float4 pos : POSITION;
};

// �W�I���g���V�F�[�_�[�̏o�͍\����
struct GSOutput
{
	//�V�X�e���p���_���W
    float4 svpos : SV_POSITION;
        
    // uv�l
    float2 uv : TEXCOORD;
};

// �s�N�Z���V�F�[�_�[�̏o�͍\����
struct PSOutput
{
    // �`��
    float4 target : SV_TARGET;
};