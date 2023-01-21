#include "Basic.hlsli"


VSOutput main(float4 pos : POSITION,float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	////ピクセルシェーダーに渡す値
	//VSOutput output;
	///*output.svpos = pos;*/
 //   output.svpos = mul(mul(viewProj,mat), pos);
 //   output.normal = mul(mat, normal);
	//output.uv = uv;
	//return output;
	
	

	//右、下、奥の方向を向いたライト
    float3 lightdir = float3(1, -1, 1);
    lightdir = normalize(lightdir);
	//ライトの色（白）
    float3 lightcolor = float3(1, 1, 1);
	//ピクセルシェーダーに渡す値
    VSOutput output;
    output.svpos = mul(mul(viewProj, mat), pos);
	//Lambert反射の計算
    //output.color.rgb = dot(-lightdir, normal) * m_diffuse * lightcolor;
    //output.color.a = m_alpha;
    output.uv = uv;
	
	//環境反射光
    float3 ambient = m_alpha;
	//拡散反射光
    float3 diffuse = dot(-lightdir, normal) * m_ambient;
	//視点座標
    const float3 eye = float3(0, 0, -0);
	//光沢度
    const float shininess = 1.0f;
	//頂点から視点へのベクトル
    float3 eyedir = normalize(eye - pos.xyz);
	//反射光ベクトル
    float3 reflect = normalize(lightdir + 2 * dot(-lightdir, normal) * normal);
	//鏡面反射光
    float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	
    output.color.rgb = (ambient + diffuse + specular) * lightcolor;
    output.color.a = m_alpha;
	
    return output;
}