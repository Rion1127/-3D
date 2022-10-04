#include "Basic.hlsli"

//cbuffer ConstBufferDataMaterial : register(b0) {
//	float4 color;	//�F(RGBA)
//}
//
//float4 main() : SV_TARGET
//{
//	return color;
//}

Texture2D<float4> tex : register(t0);	//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);		//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
	//return float4(tex.Sample(smp,input.uv)) ;
	//return float4 (input.normal,1);

	//�g�U���ˌ�(diffuse)�̌v�Z
	//float3 light = normalize(float3(1,-1,1));//�E�����@�����̃��C�g
	//float brightness = dot(-light, input.normal);//�����ւ̃x�N�g���Ɩ@���x�N�g���̓���
	
	//����(AMbient)�̌v�Z
	//float3 light = normalize(float3(1, -1, 1));//�E�����@�����̃��C�g
	//float diffuse = saturate(dot(-light, input.normal));
	//float brightness = diffuse + 0.3f;//�����ւ̃x�N�g���Ɩ@���x�N�g���̓���
	//return float4(brightness, brightness, brightness, 1);//�P�x��RGB�ɑ�����ďo��

	float3 light = normalize(float3(1, -1, 1));//�E�����@�����̃��C�g
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;//�����ւ̃x�N�g���Ɩ@���x�N�g���̓���
	//�e�N�X�`���}�b�s���O�ɂ��F����������ϐ��ɓ����
	float4 texcolor = float4(tex.Sample(smp, input.uv));
	return float4(texcolor.rgb * brightness,texcolor.a) * color;//�P�x��RGB�ɑ�����ďo��

}