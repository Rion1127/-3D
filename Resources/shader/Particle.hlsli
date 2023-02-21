//マテリアル
cbuffer cbuff0 : register(b0)
{
    matrix mat;
    matrix viewProj;
    float3 cameraPos;
}

//cbuffer cbuff1 : register(b1)
//{
//    float3 m_ambient : packoffset(c0); // アンビエント係数
//    float3 m_diffuse : packoffset(c1); // ディフューズ係数
//    float3 m_specular : packoffset(c2); // スペキュラー係数
//    float m_alpha : packoffset(c2.w); // アルファ
//}

//頂点シェーダの出力構造体
//（頂点シェーダーからピクセルシェーダーへのやり取りに使用する）
struct VSOutput
{
	//システム用頂点座標
    float4 svpos : SV_POSITION; //システム用頂点座標
    float3 normal : NORMAL; //法線ベクトル
    float2 uv : TEXCOORD; //uv座標
};

////3D変換行列
//cbuffer ConstBufferDataTransform : register(b1) {
//	matrix mat;
//}

//cbuffer ViewProjection : register(b2)
//{
//    matrix view; // ビュー変換行列
//    matrix projection; // プロジェクション変換行列
//    float3 cameraPos; // カメラ座標（ワールド座標）
//};

