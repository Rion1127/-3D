#include "Basic.hlsli"


VSOutput main(float4 pos : POSITION,float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	////�s�N�Z���V�F�[�_�[�ɓn���l
	//VSOutput output;
	///*output.svpos = pos;*/
 //   output.svpos = mul(mul(viewProj,mat), pos);
 //   output.normal = mul(mat, normal);
	//output.uv = uv;
	//return output;
	
	

	//�E�A���A���̕��������������C�g
    float3 lightdir = float3(1, -1, 1);
    lightdir = normalize(lightdir);
	//���C�g�̐F�i���j
    float3 lightcolor = float3(1, 1, 1);
	//�s�N�Z���V�F�[�_�[�ɓn���l
    VSOutput output;
    output.svpos = mul(mul(viewProj, mat), pos);
	//Lambert���˂̌v�Z
    //output.color.rgb = dot(-lightdir, normal) * m_diffuse * lightcolor;
    //output.color.a = m_alpha;
    output.uv = uv;
	
	//�����ˌ�
    float3 ambient = m_alpha;
	//�g�U���ˌ�
    float3 diffuse = dot(-lightdir, normal) * m_ambient;
	//���_���W
    const float3 eye = float3(0, 0, -0);
	//����x
    const float shininess = 1.0f;
	//���_���王�_�ւ̃x�N�g��
    float3 eyedir = normalize(eye - pos.xyz);
	//���ˌ��x�N�g��
    float3 reflect = normalize(lightdir + 2 * dot(-lightdir, normal) * normal);
	//���ʔ��ˌ�
    float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	
    output.color.rgb = (ambient + diffuse + specular) * lightcolor;
    output.color.a = m_alpha;
	
    return output;
}