//�}�e���A��
cbuffer ConstBufferDataMaterial : register(b0) {
	float4 color;
}

//���_�V�F�[�_�̏o�͍\����
//�i���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����j
struct VSOutput {
	//�V�X�e���p���_���W
	float4 svpos : SV_POSITION;	//�V�X�e���p���_���W
	float3 normal : NORMAL;		//�@���x�N�g��
	//uv���W
	float2 uv :TEXCOORD;
};

//3D�ϊ��s��
cbuffer ConstBufferDataTransform : register(b1) {
	matrix mat;
}

cbuffer ViewProjection : register(b2)
{
    matrix view; // �r���[�ϊ��s��
    matrix projection; // �v���W�F�N�V�����ϊ��s��
    float3 cameraPos; // �J�������W�i���[���h���W�j
};

cbuffer Material : register(b3)
{
    float3 m_ambient : packoffset(c0); // �A���r�G���g�W��
    float3 m_diffuse : packoffset(c1); // �f�B�t���[�Y�W��
    float3 m_specular : packoffset(c2); // �X�y�L�����[�W��
    float m_alpha : packoffset(c2.w); // �A���t�@
}

// ���s�����̐�
static const int DIRLIGHT_NUM = 3;

struct DirLight
{
    float3 lightv; // ���C�g�ւ̕����̒P�ʃx�N�g��
    float3 lightcolor; // ���C�g�̐F(RGB)
    uint active;
};

// �_�����̐�
static const int POINTLIGHT_NUM = 3;

struct PointLight
{
    float3 lightpos; // ���C�g���W
    float3 lightcolor; // ���C�g�̐F(RGB)
    float3 lightatten; // ���C�g���������W��
    uint active;
};

cbuffer LightGroup : register(b4)
{
    float3 ambientColor;
    DirLight dirLights[DIRLIGHT_NUM];
    PointLight pointLights[POINTLIGHT_NUM];
}