#include "DiscardMask.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> mask : register(t1); // 0�ԃX���b�g�ɐݒ肳�ꂽ�}�X�N
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

PSOutput main(GSOutput input)
{
    PSOutput output;

	// �e�N�X�`���}�b�s���O
    float4 texColor = tex.Sample(smp, input.uv * uvTiling + uvOffset);
    float4 maskColor = mask.Sample(smp, input.uv * uvTiling + uvOffset);
    
    clip(maskRate - maskColor.r);
    
    // �N���b�v�l�ȉ���؂�̂�
    clip((texColor.a * baseColor.a) - alphaClipVal);
    
	// �v�Z�����F�ŕ`��
    float3 baseColorRate = float3(1.0f, 1.0f, 1.0f) - texColorRate;
    float3 bColor = baseColor.rgb * baseColorRate;
    texColor.rgb *= texColorRate;
    
    output.target = float4(bColor.rgb + texColor.rgb, baseColor.a * texColor.a);
    
    return output;
}