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

// 平行光源の数
static const int DIRLIGHT_NUM = 3;

struct DirLight
{
    float3 lightv; // ライトへの方向の単位ベクトル
    float3 lightcolor; // ライトの色(RGB)
    uint active;
};

// 点光源の数
static const int POINTLIGHT_NUM = 3;

struct PointLight
{
    float3 lightpos; // ライト座標
    float3 lightcolor; // ライトの色(RGB)
    float3 lightatten; // ライト距離減衰係数
    uint active;
};

cbuffer LightGroup : register(b4)
{
    float3 ambientColor;
    DirLight dirLights[DIRLIGHT_NUM];
    PointLight pointLights[POINTLIGHT_NUM];
}