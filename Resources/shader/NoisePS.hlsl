#include "Noise.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float Gaussian(float2 drawUV, float2 pickUV, float sigma);

float4 main(VSOutput input) : SV_TARGET
{
    //スキャンノイズ
    float _Speed = 0.01f; //ノイズのスクロール速度
    float _Width = 0.1f; //ノイズがかかる幅
    float _Power = 0.5f; //揺らめき具合
    float sbTime = frac(timer * _Speed);
    float seTime = sbTime + _Width;
    float2 uv = float2(
					input.uv.x + sin(smoothstep(sbTime, seTime, input.uv.y) * 2 * 3.14159) * _Power,
					input.uv.y
					);

    float4 texcolor = tex.Sample(smp, uv);
    return texcolor;
}

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}