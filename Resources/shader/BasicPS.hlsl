#include "Basic.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	//return float4(tex.Sample(smp,input.uv)) ;
	//return float4 (input.normal,1);

	//拡散反射光(diffuse)の計算
	//float3 light = normalize(float3(1,-1,1));//右下奥　向きのライト
	//float brightness = dot(-light, input.normal);//光源へのベクトルと法線ベクトルの内積
	
	////環境光(AMbient)の計算
 //   float3 light = normalize(float3(1, -1, 1)); //右下奥　向きのライト
 //   float diffuse = saturate(dot(-light, input.normal));
 //   float brightness = diffuse + 0.3f; //光源へのベクトルと法線ベクトルの内積
 //   return float4(brightness, brightness, brightness, 1); //輝度をRGBに代入して出力

 //   float3 light = normalize(float3(1, -1, 1)); //右下奥　向きのライト
 //   float light_diffuse = saturate(dot(-light, input.normal));
 //   float3 shade_color;
 //   shade_color = m_ambient;
 //   shade_color += m_diffuse * light_diffuse;
    
	////テクスチャマッピングによる色をいったん変数に入れる
 //   float4 texcolor = float4(tex.Sample(smp, input.uv));
 //   return float4(texcolor.rgb * shade_color, texcolor.a * m_alpha); //輝度をRGBに代入して出力
	
	////テクスチャマッピング
 //   float4 texcolor = tex.Sample(smp, input.uv);
 //   //return texcolor;
 //   return input.color * texcolor;
	
	//テクスチャマッピング
    float4 texcolor = tex.Sample(smp, input.uv);
	
	
	//光沢度
    const float shininess = 4.0f;
	//頂点から視点への方向ベクトル
    float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	//環境反射光
    float3 ambient = m_ambient;
	//シェーディング
    float4 shadecolor = float4(ambientColor * ambient, m_alpha);
	
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
        if (dirLights[i].active)
        {
			//ライトに向かうベクトルと法線の内積
            float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
			//反射光ベクトル
            float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
			//拡散反射光
            float3 diffuse = dotlightnormal * m_diffuse;
			//鏡面反射光
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//すべて加算する
            shadecolor.rgb += (diffuse + specular) * dirLights[i].lightColor;
			//shadecolor.a = m_alpha;
        }
    }
	
	//シェーディングによる色で描画
    return shadecolor * texcolor;
	
	
}