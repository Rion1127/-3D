#include "Basic.hlsli"

//cbuffer ConstBufferDataMaterial : register(b0) {
//	float4 color;	//色(RGBA)
//}
//
//float4 main() : SV_TARGET
//{
//	return color;
//}

Texture2D<float4> tex : register(t0);	//0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	//return float4(tex.Sample(smp,input.uv)) ;
	//return float4 (input.normal,1);

	//拡散反射光(diffuse)の計算
	//float3 light = normalize(float3(1,-1,1));//右下奥　向きのライト
	//float brightness = dot(-light, input.normal);//光源へのベクトルと法線ベクトルの内積
	
	//環境光(AMbient)の計算
	//float3 light = normalize(float3(1, -1, 1));//右下奥　向きのライト
	//float diffuse = saturate(dot(-light, input.normal));
	//float brightness = diffuse + 0.3f;//光源へのベクトルと法線ベクトルの内積
	//return float4(brightness, brightness, brightness, 1);//輝度をRGBに代入して出力

	float3 light = normalize(float3(1, -1, 1));//右下奥　向きのライト
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;//光源へのベクトルと法線ベクトルの内積
	//テクスチャマッピングによる色をいったん変数に入れる
	float4 texcolor = float4(tex.Sample(smp, input.uv));
	return float4(texcolor.rgb * brightness,texcolor.a) * color;//輝度をRGBに代入して出力

}