//マテリアル
cbuffer cbuff0 : register(b0)
{
    matrix mat;
    matrix viewProj;
    float3 cameraPos;
}



//頂点シェーダの出力構造体
//（頂点シェーダーからピクセルシェーダーへのやり取りに使用する）
struct VSOutput
{
	//システム用頂点座標
    float4 svpos : SV_POSITION; //システム用頂点座標
    float3 normal : NORMAL; //法線ベクトル
    float2 uv : TEXCOORD; //uv座標
};


//ジオメトリシェーダからピクセルシェーダへの出力
struct GSOutput
{
	//システム用頂点座標
    float4 svpos : SV_POSITION; //システム用頂点座標
    float3 normal : NORMAL; //法線ベクトル
    float2 uv : TEXCOORD; //uv座標
};