cbuffer ConstBufferDataMaterial : register(b0) {
	float4 color;
}

cbuffer ConstBufferDataTransform : register(b1) {
	matrix mat;
}

// 円形ゲージに必要なもの
cbuffer ConstBufferDataCircleGauge : register(b2)
{
    float radian;  //(0~2πまで)
};

// 頂点シェーダーの出力構造体
struct VSOutput
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float3 normal : NORMAL; //法線ベクトル
    float2 uv : TEXCOORD; // uv値
};