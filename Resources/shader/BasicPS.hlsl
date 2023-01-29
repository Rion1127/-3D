#include "Basic.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
	//return float4(tex.Sample(smp,input.uv)) ;
	//return float4 (input.normal,1);

	//�g�U���ˌ�(diffuse)�̌v�Z
	//float3 light = normalize(float3(1,-1,1));//�E�����@�����̃��C�g
	//float brightness = dot(-light, input.normal);//�����ւ̃x�N�g���Ɩ@���x�N�g���̓���
	
	////����(AMbient)�̌v�Z
 //   float3 light = normalize(float3(1, -1, 1)); //�E�����@�����̃��C�g
 //   float diffuse = saturate(dot(-light, input.normal));
 //   float brightness = diffuse + 0.3f; //�����ւ̃x�N�g���Ɩ@���x�N�g���̓���
 //   return float4(brightness, brightness, brightness, 1); //�P�x��RGB�ɑ�����ďo��

 //   float3 light = normalize(float3(1, -1, 1)); //�E�����@�����̃��C�g
 //   float light_diffuse = saturate(dot(-light, input.normal));
 //   float3 shade_color;
 //   shade_color = m_ambient;
 //   shade_color += m_diffuse * light_diffuse;
    
	////�e�N�X�`���}�b�s���O�ɂ��F����������ϐ��ɓ����
 //   float4 texcolor = float4(tex.Sample(smp, input.uv));
 //   return float4(texcolor.rgb * shade_color, texcolor.a * m_alpha); //�P�x��RGB�ɑ�����ďo��
	
	////�e�N�X�`���}�b�s���O
 //   float4 texcolor = tex.Sample(smp, input.uv);
 //   //return texcolor;
 //   return input.color * texcolor;
	
	//�e�N�X�`���}�b�s���O
    float4 texcolor = tex.Sample(smp, input.uv);
	
	
	//����x
    const float shininess = 4.0f;
	//���_���王�_�ւ̕����x�N�g��
    float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	//�����ˌ�
    float3 ambient = m_ambient;
	//�V�F�[�f�B���O
    float4 shadecolor = float4(ambientColor * ambient, m_alpha);
	//���s����
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
        if (dirLights[i].active)
        {
			//���C�g�Ɍ������x�N�g���Ɩ@���̓���
            float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
			//���ˌ��x�N�g��
            float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
			//�g�U���ˌ�
            float3 diffuse = dotlightnormal * m_diffuse;
			//���ʔ��ˌ�
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//���ׂĉ��Z����
            shadecolor.rgb += (diffuse + specular) * dirLights[i].lightColor;
			//shadecolor.a = m_alpha;
        }
    }
	//�_����
    for (int i = 0; i < POINTLIGHT_NUM; i++)
    {
        if (pointLights[i].active)
        {
			//���C�g�̃x�N�g��
            float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
			//�x�N�g���̒���
            float d = length(lightv);
			//���K�����A�P�ʃx�N�g���ɂ���
            lightv = normalize(lightv);
			//���������W��
            float atten = 1.0f / (pointLights[i].lightAtten.x + pointLights[i].lightAtten.y * d +
			pointLights[i].lightAtten.z * d * d);
			//���C�g�Ɍ������x�N�g���Ɩ@���̓���
            float3 dotlightnormal = dot(lightv, input.normal);
			//���ˌ��x�N�g��
            float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//�g�U���ˌ�
            float3 diffuse = dotlightnormal * m_diffuse;
			//���ʔ��ˌ�
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//���ׂĉ��Z����
            shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightColor;

        }
    }
	
	//�V�F�[�f�B���O�ɂ��F�ŕ`��
        return shadecolor * texcolor;
	
	
}