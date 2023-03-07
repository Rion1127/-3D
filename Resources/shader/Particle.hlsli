//�}�e���A��
cbuffer Transform : register(b0)
{
    matrix mat;
}

//���_�V�F�[�_�̏o�͍\����
//�i���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����j
struct VSOutput
{
	//�V�X�e���p���_���W
    float4 pos : POSITION; //���_���W
};


//�W�I���g���V�F�[�_����s�N�Z���V�F�[�_�ւ̏o��
struct GSOutput
{
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float2 uv : TEXCOORD; //uv���W
};