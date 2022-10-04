//マテリアル
cbuffer ConstBufferDataMaterial : register(b0) {
	float4 color;
}

//頂点シェーダの出力構造体
//（頂点シェーダーからピクセルシェーダーへのやり取りに使用する）
struct VSOutput {
	//システム用頂点座標
	float4 svpos : SV_POSITION;	//システム用頂点座標
	float3 normal : NORMAL;		//法線ベクトル
	//uv座標
	float2 uv :TEXCOORD;
};

//3D変換行列
cbuffer ConstBufferDataTransform : register(b1) {
	matrix mat;
}

cbuffer ViewProjection : register(b2)
{
    matrix view; // ビュー変換行列
    matrix projection; // プロジェクション変換行列
    float3 cameraPos; // カメラ座標（ワールド座標）
};

cbuffer Material : register(b3)
{
    float3 m_ambient : packoffset(c0); // アンビエント係数
    float3 m_diffuse : packoffset(c1); // ディフューズ係数
    float3 m_specular : packoffset(c2); // スペキュラー係数
    float m_alpha : packoffset(c2.w); // アルファ
}