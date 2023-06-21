#include "Bloom.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float Gaussian(float2 drawUV, float2 pickUV, float sigma);

float4 main(VSOutput input) : SV_TARGET
{
    float4 col = tex0.Sample(smp, input.uv);
    
    //1.高輝度部を抽出
    //float grayScale = col.r * 0.299 + col.g * 0.587 + col.b * 0.114;
    //float extract = smoothstep(0.4, 0.9, grayScale);
    //float4 highLumi = col * extract;
    //2.抽出した画像にブラーをかける
    float totalWeight = 0, _Sigma = 0.050, _StepWidth = 0.005;
    float4 gaussianTexCol = float4(0, 0, 0, 0);

    for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
    {
        for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
        {
            float2 pickUV = input.uv + float2(px, py);
            float weight = Gaussian(input.uv, pickUV, _Sigma);
            gaussianTexCol += tex0.Sample(smp, pickUV) * weight; //Gaussianで取得した「重み」を色にかける
            totalWeight += weight; //かけた「重み」の合計値を控えておく
        }
    }
    gaussianTexCol.rgb = gaussianTexCol.rgb / totalWeight; //かけた「重み」分、結果から割る
    ////3.もとの画像とブラーをかけた画像を合成する
    
    ////高輝度部を抽出
    return float4(gaussianTexCol.rgb,1.f);
    
}

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}