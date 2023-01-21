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
    output.color.rgb = dot(-lightdir, normal) * m_diffuse * lightcolor;
    output.color.a = m_alpha;
    output.uv = uv;
	
    return output;
}