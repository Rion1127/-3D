#include "Bloom.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float Gaussian(float2 drawUV, float2 pickUV, float sigma);

float4 main(VSOutput input) : SV_TARGET
{
    float4 col = tex0.Sample(smp, input.uv);
    
    //1.���P�x���𒊏o
    //float grayScale = col.r * 0.299 + col.g * 0.587 + col.b * 0.114;
    //float extract = smoothstep(0.4, 0.9, grayScale);
    //float4 highLumi = col * extract;
    //2.���o�����摜�Ƀu���[��������
    float totalWeight = 0, _Sigma = 0.050, _StepWidth = 0.005;
    float4 gaussianTexCol = float4(0, 0, 0, 0);

    for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
    {
        for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
        {
            float2 pickUV = input.uv + float2(px, py);
            float weight = Gaussian(input.uv, pickUV, _Sigma);
            gaussianTexCol += tex0.Sample(smp, pickUV) * weight; //Gaussian�Ŏ擾�����u�d�݁v��F�ɂ�����
            totalWeight += weight; //�������u�d�݁v�̍��v�l���T���Ă���
        }
    }
    gaussianTexCol.rgb = gaussianTexCol.rgb / totalWeight; //�������u�d�݁v���A���ʂ��犄��
    ////3.���Ƃ̉摜�ƃu���[���������摜����������
    
    ////���P�x���𒊏o
    return float4(gaussianTexCol.rgb,1.f);
    
}

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}