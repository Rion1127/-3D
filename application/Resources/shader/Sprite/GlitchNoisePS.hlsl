#include "Default.hlsli"
#include "GlitchNoise.hlsli"
#include "Math.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float BlockNoise(float2 seeds)
{
    return Random(floor(seeds));
}

float NoiseRandom(float2 seeds)
{
    return -1.0 + 2.0 * BlockNoise(seeds);
}

PSOutput main(GSOutput input)
{
    PSOutput output;
    
    float2 uv = input.uv * uvTiling + uvOffset;
    
    // ブロックノイズ
    float noise = BlockNoise(uv.y * blockScale);
    noise += Random(uv.x * 0.3f);
    
    // UVずらし
    float2 glicthUV = uv;
    float randomvalue = NoiseRandom(float2(uv.y, time * noiseSpeed));
    glicthUV.x += randomvalue * sin(sin(glitchIntensity) * 0.5f) * sin(-sin(noise) * 0.2f) * frac(time);
    //glicthUV.x += randomvalue * glitchIntensity * noise * frac(time);

	// テクスチャマッピング 
    float4 texColor = tex.Sample(smp, glicthUV);
    
    // 色収差
    texColor.r = tex.Sample(smp, glicthUV + float2(0.006, 0)).r;
    texColor.b = tex.Sample(smp, glicthUV - float2(0.008, 0)).b;
    
    // 0.1 以下を切り捨て
    clip(baseColor.a - 0.1f);
    clip(texColor.a - 0.1f);
    
	// 計算した色で描画
    float3 baseColorRate = float3(1.0f, 1.0f, 1.0f) - texColorRate;
    float3 bColor = baseColor.rgb * baseColorRate;
    texColor.rgb *= texColorRate;
    
    output.target = float4(bColor.rgb + texColor.rgb, baseColor.a * texColor.a);
    
    return output;
}