//�}�e���A��
cbuffer cbuff0 : register(b0)
{
    matrix mat;
    matrix viewProj;
    float3 cameraPos;
}

//cbuffer cbuff1 : register(b1)
//{
//    float3 m_ambient : packoffset(c0); // �A���r�G���g�W��
//    float3 m_diffuse : packoffset(c1); // �f�B�t���[�Y�W��
//    float3 m_specular : packoffset(c2); // �X�y�L�����[�W��
//    float m_alpha : packoffset(c2.w); // �A���t�@
//}

//���_�V�F�[�_�̏o�͍\����
//�i���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����j
struct VSOutput
{
	//�V�X�e���p���_���W
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float3 normal : NORMAL; //�@���x�N�g��
    float2 uv : TEXCOORD; //uv���W
};

////3D�ϊ��s��
//cbuffer ConstBufferDataTransform : register(b1) {
//	matrix mat;
//}

//cbuffer ViewProjection : register(b2)
//{
//    matrix view; // �r���[�ϊ��s��
//    matrix projection; // �v���W�F�N�V�����ϊ��s��
//    float3 cameraPos; // �J�������W�i���[���h���W�j
//};

