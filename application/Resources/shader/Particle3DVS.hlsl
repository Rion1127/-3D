#include "Particle3D.hlsli"


VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	//�@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
    float4 wnormal = normalize(mul(mat, float4(normal, 0)));
    float4 wpos = mul(mat, pos);
	
	//�s�N�Z���V�F�[�_�[�ɓn���l
    VSOutput output;
    output.svpos = mul(mul(viewProj, mat), pos);
    output.worldpos = wpos;
    output.normal = wnormal.xyz;
    output.uv = uv;
    return output;
}