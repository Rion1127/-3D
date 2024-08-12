#include "Sprite.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	//return float4(input.uv,0,1);
    float2 uv =
    {
        input.uv.x * tiling.x,
        input.uv.y * tiling.y
    };

    return float4(tex.Sample(smp, uv)) * color;
}