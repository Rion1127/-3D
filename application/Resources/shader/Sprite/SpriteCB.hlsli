// ワールド行列
cbuffer cbMatWorld : register(b0)
{
    matrix matWorld;
}

// 色
cbuffer cbColor : register(b1)
{
    float4 baseColor;
    float3 texColorRate;
    float alphaClipVal;
}

// テクスチャ設定
cbuffer cbTextureSetting : register(b2)
{
    float2 texSeqSize; // 画像サイズ
    float2 texRange; // 絵の範囲
    
    float2 texSizeRatio; // テクスチャサイズ比率

    float2 texSeqNum; // 連番位置
    float2 texSeqMax; // 連番数
		
    float texFlipX; // X軸反転
    float texFlipY; // Y軸反転

    float2 texAnchor; // アンカーポイント
    float2 texOffset; // オフセット
    
    float2 uvTiling; // タイリング
    float2 uvOffset; // オフセット
}
