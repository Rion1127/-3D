#include "SpriteCB.hlsli"

//頂点シェーダの出力構造体
struct VSOutput
{
    // 頂点座標
    float4 pos : POSITION;
};

// ジオメトリシェーダーの出力構造体
struct GSOutput
{
	//システム用頂点座標
    float4 svpos : SV_POSITION;
        
    // uv値
    float2 uv : TEXCOORD;
};

// ピクセルシェーダーの出力構造体
struct PSOutput
{
    // 描画
    float4 target : SV_TARGET;
};