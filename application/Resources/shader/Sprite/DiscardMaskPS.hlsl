#include "DiscardMask.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> mask : register(t1); // 0番スロットに設定されたマスク
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

PSOutput main(GSOutput input)
{
    PSOutput output;

	// テクスチャマッピング
    float4 texColor = tex.Sample(smp, input.uv * uvTiling + uvOffset);
    float4 maskColor = mask.Sample(smp, input.uv * uvTiling + uvOffset);
    
    clip(maskRate - maskColor.r);
    
    // クリップ値以下を切り捨て
    clip((texColor.a * baseColor.a) - alphaClipVal);
    
	// 計算した色で描画
    float3 baseColorRate = float3(1.0f, 1.0f, 1.0f) - texColorRate;
    float3 bColor = baseColor.rgb * baseColorRate;
    texColor.rgb *= texColorRate;
    
    output.target = float4(bColor.rgb + texColor.rgb, baseColor.a * texColor.a);
    
    return output;
}