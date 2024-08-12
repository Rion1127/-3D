cbuffer ConstBufferDataMaterial : register(b0) {
	float4 color;
}

cbuffer ConstBufferDataTransform : register(b1) {
	matrix mat;
    float2 tiling;
}

//頂点シェーダの出力構造体
//（頂点シェーダーからピクセルシェーダーへのやり取りに使用する）
struct VSOutput {
	//システム用頂点座標
	float4 svpos : SV_POSITION;
	//uv座標
	float2 uv :TEXCOORD;
};