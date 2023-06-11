#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float Gaussian(float2 drawUV, float2 pickUV, float sigma);

float4 main(VSOutput input) : SV_TARGET
{
    //float4 texcolor = tex.Sample(smp, input.uv);
    
    //return float4(texcolor.rgb, 1);
    
    ////uv�V�t�g�u���[
    //float _ShiftWidth = 0.005;
    //float _ShiftNum = 3;
    //float4 col = float4(0, 0, 0, 1);
    //float num = 0;
	//[loop]
    //for (float py = -_ShiftNum / 2; py <= _ShiftNum / 2; py++)
    //{
    //    for (float px = -_ShiftNum / 2; px <= _ShiftNum / 2; px++)
    //    {
    //        col += tex.Sample(smp, input.uv + float2(px, py) * _ShiftWidth);
    //        num++;
    //    }
    //}
    //col.rgb = col.rgb / num;
    //return col;
    
    //�K�E�V�A���u���[
    float totalWeight = 0, _Sigma = 0.025, _StepWidth = 0.1;
    float4 col = float4(0, 0, 0, 0);

    for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
    {
        for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
        {
            float2 pickUV = input.uv + float2(px, py);
            float weight = Gaussian(input.uv, pickUV, _Sigma);
            col += tex.Sample(smp, pickUV) * weight; //Gaussian�Ŏ擾�����u�d�݁v��F�ɂ�����
            totalWeight += weight; //�������u�d�݁v�̍��v�l���T���Ă���
        }
    }
    col.rgb = col.rgb / totalWeight; //�������u�d�݁v���A���ʂ��犄��
    col.a = 1;
    return col;
}

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}