//�}�e���A��
cbuffer cbuff0 : register(b0)
{
    matrix mat;
    matrix viewProj;
    float3 cameraPos;
}



//���_�V�F�[�_�̏o�͍\����
//�i���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����j
struct VSOutput
{
	//�V�X�e���p���_���W
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float3 normal : NORMAL; //�@���x�N�g��
    float2 uv : TEXCOORD; //uv���W
};


//�W�I���g���V�F�[�_����s�N�Z���V�F�[�_�ւ̏o��
struct GSOutput
{
	//�V�X�e���p���_���W
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float3 normal : NORMAL; //�@���x�N�g��
    float2 uv : TEXCOORD; //uv���W
};