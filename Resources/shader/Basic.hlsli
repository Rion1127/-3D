//3D変換行列
cbuffer ConstBufferDataTransform : register(b0) {
	matrix mat;
    matrix viewProj;
    float3 cameraPos;
}

cbuffer Material : register(b1)
{
    float3 m_ambient : packoffset(c0); // アンビエント係数
    float3 m_diffuse : packoffset(c1); // ディフューズ係数
    float3 m_specular : packoffset(c2); // スペキュラー係数
    float m_alpha : packoffset(c2.w); // アルファ
}

//cbuffer Light : register(b2)
//{
//    float3 lightv;      //ライトへの方向の単位ベクトル
//    float3 lightcolor;  //ライトの色
//}

struct DirLight
{
    float3 lightv; //ライトへの方向の単位ベクトル
    float3 lightColor;  //ライトの色
    uint active;
};

static const int DIRLIGHT_NUM = 3;

cbuffer Light : register(b2)
{
    float3 ambientColor; //ライトへの方向の単位ベクトル
    DirLight dirLights[DIRLIGHT_NUM];
}


//頂点シェーダの出力構造体
//（頂点シェーダーからピクセルシェーダーへのやり取りに使用する）
struct VSOutput
{
	//システム用頂点座標
    float4 svpos : SV_POSITION; //システム用頂点座標
    //float4 color : COLOR;//色
    float4 worldpos : POSITION;//ワールド座標
    float3 normal : NORMAL; //法線ベクトル
    float2 uv : TEXCOORD; //uv座標
};