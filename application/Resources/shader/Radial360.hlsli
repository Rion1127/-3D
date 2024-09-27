cbuffer ConstBufferDataMaterial : register(b0) {
	float4 color;
}

cbuffer ConstBufferDataTransform : register(b1) {
	matrix mat;
}

// �~�`�Q�[�W�ɕK�v�Ȃ���
cbuffer ConstBufferDataCircleGauge : register(b2)
{
    float radian;  //(0~2�΂܂�)
};

// ���_�V�F�[�_�[�̏o�͍\����
struct VSOutput
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float3 normal : NORMAL; //�@���x�N�g��
    float2 uv : TEXCOORD; // uv�l
};